#include <cstdint>

// 占位入口：GPIO 未确认前不调用任何硬件接口。
// 确认后在这里初始化传感器、振动和语音模块，并启动 FreeRTOS 任务。

enum class HapticLevel : uint8_t {
    Off = 0,
    Low,
    Medium,
    High,
    Pulse,
};

struct SensorReading {
    bool valid = false;
    float distance_cm = 0.0F;
};

// 建议接线（待用户确认后写入正式配置）：
// RCWL-1605 Trig -> GPIO4（输出）
// RCWL-1605 Echo -> GPIO5（输入，5V 输出必须分压后接入）
// 738 马达 PWM   -> GPIO6（经 MOS 管栅极）
// 预录音语音触发 -> GPIO7（待模块型号确认）

struct PinConfig {
    int trig_pin = 4;      // provisional: requires user confirmation
    int echo_pin = 5;      // provisional: requires user confirmation
    int motor_pwm_pin = 6; // provisional: requires user confirmation
    int speech_trigger_pin = 7; // provisional: requires user confirmation
};

// 纯逻辑函数，可在无硬件环境下编译并测试。
static HapticLevel level_for_distance(float distance_cm) {
    if (distance_cm <= 50.0F) return HapticLevel::Pulse;
    if (distance_cm <= 100.0F) return HapticLevel::High;
    if (distance_cm <= 150.0F) return HapticLevel::Medium;
    return HapticLevel::Low;
}

int main() {
    // 引脚尚未与实物核对，只做编译期存在性验证，不初始化任何 GPIO。
    static_cast<void>(&level_for_distance);
    static_cast<void>(PinConfig{});
    return 0;
}
