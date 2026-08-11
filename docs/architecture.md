# 项目软件架构

## 1. 项目简介

本项目是一款基于 ESP32 的遥控器设备。

当前软件主要负责：

- LCD 图形界面
- 按键输入
- BME280 环境数据采集
- CO2 数据采集
- WiFi 配网
- WiFi 连接
- WiFi 连接失败重试
- Web 配网
- OTA 固件升级
- RGB 状态指示

空调实际控制功能由另一位开发人员负责，目前尚未接入本软件主流程。

---

## 2. 软件分层

项目采用以下结构：

```text
Application
    │
    ├── App / Page
    │
Service
    │
    ├── SensorService
    └── WifiService
    │
Driver
    │
    ├── BME280
    ├── CO2
    ├── LCD
    ├── RGB
    └── Button
    │
HAL
    │
    ├── I2C
    ├── SPI
    ├── UART
    └── GPIO
    │
Hardware
    │
    └── ESP32
```

---

## 3. Board

`Board` 负责统一管理硬件对象。

主要包含：

- SPI
- I2C
- UART
- RGB
- LCD
- BME280
- CO2
- 按键

初始化入口：

```cpp
board_.init();
```

Board 不负责业务逻辑，只负责硬件层初始化。

---

## 4. App

App 负责用户界面和页面切换。

主要页面：

```text
MainMenuPage
TempPage
WifiPage
BatteryPage
SwitchPage
```

页面通过 `switch_Page()` 切换。

```cpp
app_.switch_Page(&wifiPage);
```

App 不直接执行底层 WiFi 操作，而是通过 `WifiService` 请求。

---

## 5. SensorService

SensorService 负责：

- BME280 温度
- BME280 湿度
- BME280 气压
- CO2

Sensor Task 被唤醒以后调用：

```cpp
sensorService.update();
```

数据最终保存到：

```cpp
g_data
```

---

## 6. WifiService

WifiService 负责：

- WiFi 初始化
- WiFi 扫描
- AP 热点
- WebServer
- WiFi 配置
- WiFi 连接
- 连接超时
- Retry
- OTA

WiFi 通过 Queue 接收 UI 发来的命令。

---

## 7. FreeRTOS

当前使用三个主要 Task：

| Task | Core | Priority | 功能 |
|---|---:|---:|---|
| UI | 1 | 2 | UI、按键 |
| Sensor | 0 | 1 | 传感器 |
| WiFi | 0 | 3 | WiFi、Web、OTA |

另外使用：

- Queue：任务之间传递 WiFi 命令
- EventGroup：保存 WiFi 连接状态
- Task Notification：唤醒 Sensor Task
- Software Timer：每秒触发传感器采集

---

## 8. 数据流

```text
Button
  ↓
App
  ↓
Page
  ↓
Service
  ↓
Driver
  ↓
Hardware
```

传感器：

```text
BME280 / CO2
      ↓
Sensor Driver
      ↓
SensorService
      ↓
g_data
      ↓
TempPage
      ↓
LCD
```

WiFi：

```text
Button
  ↓
WifiPage
  ↓
WifiService request
  ↓
Queue
  ↓
WiFi Task
  ↓
WifiService
  ↓
ESP32 WiFi
```

---

## 9. 设计原则

1. UI 不直接操作 WiFi 底层。
2. UI 不直接读取传感器硬件。
3. Service 负责业务逻辑。
4. Driver 负责硬件驱动。
5. Board 负责硬件对象和硬件初始化。
6. FreeRTOS Task 负责后台周期性工作。
7. OTA 由 WifiService 统一管理。
