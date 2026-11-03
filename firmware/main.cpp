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

// 正式接线（采用此前原型验证过的接法，3.3V 供电无需分压）：
// RCWL-1605 VCC -> 3V3
// RCWL-1605 Trig -> GPIO5（输出）
// RCWL-1605 Echo -> GPIO18（输入，模块 3.3V 供电时 Echo 为 3.3V 电平）
// RCWL-1605 GND -> GND
// 738 马达 IN -> GPIO4（输出，经 MOS 管栅极，高电平触发）
// 738 马达 VCC -> 3V3（5V 针脚未供电；后续上电池再评估切 5V）
// 预录音语音触发 -> GPIO6（待模块型号确认）

struct PinConfig {
    int trig_pin = 5;
    int echo_pin = 18;
    int motor_pwm_pin = 4;
    int speech_trigger_pin = 6; // provisional: speech module model unconfirmed
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
