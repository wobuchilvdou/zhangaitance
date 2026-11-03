# ESP32-S3 开发板引脚观察记录

依据实拍照片与商品页资料确认板卡型号为 ESP32-S3-DevKitC-1 兼容板，模组 ESP32-S3-WROOM-1-N16R8（16 MiB Flash / 8 MiB PSRAM）。

## 双 Type-C

- COM 口：连接 USB-Serial/JTAG，用于烧录和串口日志。
- USB 口：原生 USB-OTG，可做 USB 通信。

## 板载外设

- RST、BOOT 按键
- RGB LED（WS2812 风格）

## 当前杜邦线接法

照片中没有外接杜邦线；开发板为裸板状态。

## 建议的默认接线（待用户确认后写死）

根据板上可用 GPIO 与典型电气特性，建议：

| 外设 | 信号 | ESP32-S3 引脚 | 理由 |
|---|---|---|---|
| RCWL-1605 Trig | GPIO 输出 | GPIO4 | 安全通用 IO，无 USB/Flash 冲突 |
| RCWL-1605 Echo | GPIO 输入 | GPIO5 | 同上，5V 输出需分压至 3.3V |
| 738 马达 PWM | GPIO 输出 | GPIO6 | 走 MOS 管栅极，需限流 |
| 预录音语音模块触发 | GPIO 输出 | GPIO7 | 待语音模块型号确认 |

## 仍然阻塞

- 实物当前没有任何杜邦线连接，确认 Trig/Echo 尚未接线。
- Echo 输出为 5V 逻辑，需确认分压电阻是否已经准备。
- 马达供电 5V 来源与公共地连接方式。
