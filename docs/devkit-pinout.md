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

## 定案接线

已采用此前原型接法：Trig=GPIO5，Echo=GPIO18，马达 IN=GPIO4，超声波与马达 VCC 均为 3V3。
详见 wiring.md。语音触发预留 GPIO6，待模块型号确认。
