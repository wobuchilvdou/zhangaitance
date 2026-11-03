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

// 纯逻辑函数，可在无硬件环境下编译并测试。
static HapticLevel level_for_distance(float distance_cm) {
    if (distance_cm <= 50.0F) return HapticLevel::Pulse;
    if (distance_cm <= 100.0F) return HapticLevel::High;
    if (distance_cm <= 150.0F) return HapticLevel::Medium;
    return HapticLevel::Low;
}

int main() {
    // 硬件初始化前不放任何代码。
    static_cast<void>(&level_for_distance);
    return 0;
}
