// ESP32-S3 Blind Assist — Stage 1: distance -> risk -> haptics + speech trigger
// Board: ESP32-S3-DevKitC-1 compatible, N16R8 (16MB flash, 8MB PSRAM)
// Wiring: docs/wiring.md
//   RCWL-1605: VCC=3V3 GND=GND Trig=GPIO5 Echo=GPIO18
//   738 motor: IN=GPIO4 (MOS, active high) VCC=3V3
//   Speech trigger (model unconfirmed): GPIO6

#include <Arduino.h>

// ---- Provisional thresholds: calibrate with real hardware before trusting ----
constexpr uint8_t PIN_TRIG = 5;
constexpr uint8_t PIN_ECHO = 18;
constexpr uint8_t PIN_MOTOR = 4;
constexpr uint8_t PIN_SPEECH = 6;

constexpr float CLOSE_CM = 50.0f;    // CRITICAL
constexpr float WARN_CM = 100.0f;    // WARNING
constexpr float INFO_CM = 150.0f;    // INFO; beyond -> SAFE
constexpr uint32_t ECHO_TIMEOUT_US = 25000;  // ~4.3 m max
constexpr uint32_t SAMPLE_MS = 100;
constexpr uint8_t MEDIAN_N = 5;

// Motor PWM (LEDC)
constexpr uint32_t MOTOR_FREQ_HZ = 1000;
constexpr uint8_t MOTOR_CH = 0;
constexpr uint8_t MOTOR_RES_BITS = 8;
constexpr uint8_t DUTY_OFF = 0;
constexpr uint8_t DUTY_LOW = 100;    // ~39%
constexpr uint8_t DUTY_MED = 155;    // ~61%
constexpr uint8_t DUTY_HIGH = 205;   // ~80%
constexpr uint8_t DUTY_MAX = 255;

enum Level : uint8_t { LV_SAFE, LV_INFO, LV_WARN, LV_CRIT };

static uint32_t s_readings[MEDIAN_N];
static uint8_t  s_ridx = 0;
static bool     s_buf_full = false;
static bool     s_sensor_fault = false;
static uint32_t s_last_fault_log = 0;
static uint32_t s_motor_window = 0;
static bool     s_motor_burst_on = true;

static float read_distance_cm() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(4);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  uint32_t us = pulseIn(PIN_ECHO, HIGH, ECHO_TIMEOUT_US);
  if (us == 0) return -1.0f;
  return us * 0.0343f / 2.0f;
}

static bool median(float &out) {
  if (!s_buf_full) return false;
  uint32_t tmp[MEDIAN_N];
  memcpy(tmp, s_readings, sizeof(tmp));
  for (uint8_t i = 1; i < MEDIAN_N; i++) {
    uint32_t k = tmp[i];
    int8_t j = i - 1;
    while (j >= 0 && tmp[j] > k) { tmp[j + 1] = tmp[j]; j--; }
    tmp[j + 1] = k;
  }
  out = tmp[MEDIAN_N / 2] * 0.001f;
  return true;
}

static Level classify(float cm) {
  if (cm <= CLOSE_CM) return LV_CRIT;
  if (cm <= WARN_CM) return LV_WARN;
  if (cm <= INFO_CM) return LV_INFO;
  return LV_SAFE;
}

static void motor_apply(Level lv) {
  uint8_t duty = DUTY_OFF;
  switch (lv) {
    case LV_CRIT: duty = DUTY_MAX; break;
    case LV_WARN: duty = DUTY_HIGH; break;
    case LV_INFO: duty = DUTY_MED; break;
    default: duty = DUTY_OFF; break;
  }
  ledcWrite(MOTOR_CH, duty);
}

// CRITICAL uses interrupting bursts so it feels distinct from steady HIGH.
static void motor_update(uint32_t now, Level lv) {
  if (lv == LV_CRIT) {
    if (now - s_motor_window >= 100) {
      s_motor_window = now;
      s_motor_burst_on = !s_motor_burst_on;
    }
    ledcWrite(MOTOR_CH, s_motor_burst_on ? DUTY_MAX : DUTY_OFF);
  } else {
    s_motor_burst_on = true;
    motor_apply(lv);
  }
}

static void speech_trigger(Level lv, bool fault) {
  // Placeholder until speech module model is confirmed:
  // short GPIO pulse, count encodes severity.
  uint8_t pulses = fault ? 2 : (lv == LV_CRIT ? 4 : lv == LV_WARN ? 3 : lv == LV_INFO ? 2 : 0);
  for (uint8_t i = 0; i < pulses; i++) {
    digitalWrite(PIN_SPEECH, HIGH);
    delay(30);
    digitalWrite(PIN_SPEECH, LOW);
    delay(40);
  }
}

static const char *level_name(Level lv) {
  switch (lv) {
    case LV_CRIT: return "CRITICAL";
    case LV_WARN: return "WARNING";
    case LV_INFO: return "INFO";
    default: return "SAFE";
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_SPEECH, OUTPUT);
  digitalWrite(PIN_TRIG, LOW);
  digitalWrite(PIN_SPEECH, LOW);
  // Arduino-ESP32 core 3.x replaced ledcSetup/ledcAttachPin with ledcAttach.
  ledcAttach(PIN_MOTOR, MOTOR_FREQ_HZ, MOTOR_RES_BITS);
  ledcWrite(MOTOR_CH, DUTY_OFF);
  Serial.println("blind_assist stage1 boot");
}

void loop() {
  static uint32_t next_sample = 0;
  static Level last = LV_SAFE;
  uint32_t now = millis();
  if (now < next_sample) return;
  next_sample = now + SAMPLE_MS;

  float cm = read_distance_cm();
  if (cm < 0) {
    s_sensor_fault = true;
    if (now - s_last_fault_log >= 3000) {
      s_last_fault_log = now;
      Serial.println("SENSOR_FAULT timeout");
    }
    return;
  }
  s_last_fault_log = now;
  if (s_sensor_fault) {
    s_sensor_fault = false;
    Serial.println("SENSOR_RECOVERED");
  }

  s_readings[s_ridx] = (uint32_t)(cm * 1000.0f);
  s_ridx = (s_ridx + 1) % MEDIAN_N;
  if (s_ridx == 0) s_buf_full = true;

  float dist;
  if (!median(dist)) return;
  Level lv = classify(dist);

  if (lv != last) {
    Serial.printf("dist=%.1fcm level=%s\n", dist, level_name(lv));
    speech_trigger(lv, false);
    last = lv;
  }
  motor_update(now, lv);
}
