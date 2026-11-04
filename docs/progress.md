# 阶段 1 进度记录

## 2026-11-04

### 已完成

- 固件烧录验证：仅写 app0（0x10000），完整 16 MiB Flash 原始备份已保留在 `D:\Desktop\具身智能\11.3\ESP32-S3原始备份_2026-11-03\esp32s3_flash_16MB.bin`（SHA256 50cbd1ba811eff345beae8ca6709cb7c888911abe9f32374021adfdde36a5027）。
- 超声波实测：RCWL-1605 输出真实距离（约 30–135 cm），四级分级（CRITICAL / WARNING / INFO / SAFE）随距离切换，串口日志可读。
- 马达修复：Arduino-ESP32 3.x 的 `ledcWrite()` 第一个参数是引脚号，不是通道号。原代码 `ledcWrite(MOTOR_CH=0, duty)` 把 PWM 输出到 GPIO0，马达在 GPIO4 所以不震。改为 `ledcWrite(PIN_MOTOR, duty)` 后实机验证通过（PWM 持续输出和数字电平闪烁都能震）。
- 正式固件（无调试代码）已烧录：315795 字节，超声波与马达闭环可用。

### 阻塞项

- 语音模块：型号未知，触发引脚预留 GPIO6，等待正反面照片确认型号、触发逻辑与供电。
- 阈值：CLOSE=50 / WARN=100 / INFO=150 cm 为临时值，需要实测校准。
- 超声波偶发 timeout（SENSOR_FAULT timeout），之后自动恢复；可能是 RCWL-1605 精度限制或电气噪声，待换模块或加滤波。

### 下一步

1. 确认语音模块型号，接 GPIO6 实测触发。
2. 实测校准距离阈值。
3. 阶段 2：拍照上传 + 云端 AI 识别。
