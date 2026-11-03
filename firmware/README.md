# 固件说明

当前为骨架，等待确认 GPIO 后补充硬件驱动。

## 模块规划

- main.cpp: 启动与任务初始化
- sensor_api/: 超声波抽象，未来支持 UART/I2C/GPIO
- risk_model/: 与模拟器共用距离分级逻辑
- haptics/: 振动 PWM 控制
- speech/: 预录音播放队列
- config/: 阈值存储

## 编译

待选定 ESP-IDF 或 Arduino，未定前不写死编译脚本。
