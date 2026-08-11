# 项目交接说明

## 1. 项目平台

当前项目使用：

```text
ESP32
PlatformIO
Arduino Framework
```

主要依赖：

```text
U8g2
Adafruit BME280
ArduinoJson
PubSubClient
```

注意：当前代码实际没有使用 MQTT 功能，`PubSubClient` 是否保留可根据后续需求决定。

---

## 2. 项目当前功能

### 已完成

- [x] Board 硬件初始化
- [x] GPIO
- [x] SPI
- [x] I2C
- [x] UART
- [x] LCD
- [x] 按键
- [x] RGB
- [x] BME280
- [x] CO2
- [x] FreeRTOS Task
- [x] Sensor Timer
- [x] Task Notification
- [x] WiFi 扫描
- [x] WiFi 配网
- [x] AP
- [x] WebServer
- [x] WiFi 连接
- [x] 15 秒连接超时
- [x] Retry
- [x] OTA 基础功能

### 尚未完成

- [ ] 空调实际控制
- [ ] 空调通信协议
- [ ] 空调控制 Driver
- [ ] 空调 Service
- [ ] 空调状态反馈
- [ ] 完整实物测试
- [ ] OTA 长时间稳定性测试
- [ ] 产品级 OTA 安全机制

---

## 3. 空调控制

空调控制模块由另一位开发人员负责。

当前 UI 已经预留：

```text
Power
Temperature
Fan Speed
```

App 中已经存在：

```cpp
g_data.airPower
g_data.targetTemp
g_data.fanSpeed
```

但是目前这些变量只是 UI 状态，并没有真正控制空调硬件。

后续接入时建议：

```text
SwitchPage
    ↓
AirService
    ↓
Air Driver
    ↓
实际通信接口
    ↓
空调
```

不要直接在 `SwitchPage` 中写硬件通信代码。

---

## 4. 主要目录

建议保持：

```text
src/
│
├── main.cpp
│
├── Board.cpp
├── Board.h
│
├── app/
│   ├── app.cpp
│   ├── app.h
│   ├── tasks.cpp
│   └── tasks.h
│
├── wifi/
│   ├── wifi_service.cpp
│   └── wifi_service.h
│
├── sensor_service.cpp
├── sensor_service.h
│
├── display_service.cpp
├── display_service.h
│
└── drivers/
    ├── RGB
    ├── LCD
    ├── BME280
    ├── CO2
    └── Button
```

---

## 5. main.cpp

当前启动入口：

```cpp
void setup()
{
    Serial.begin(115200);

    board_.init();

    loadConfig();

    sensorService.init(board_.bme_, board_.co2_);

    wifiService.init();

    app_.init();

    createTasks();
}

void loop()
{
    vTaskDelete(nullptr);
}
```

---

## 6. 重要全局对象

```text
board_
app_
wifiService
sensorService
```

这些对象之间关系：

```text
main
 │
 ├── board_
 │
 ├── sensorService
 │
 ├── wifiService
 │
 └── app_
```

---

## 7. FreeRTOS

### UI

```text
Core 1
Priority 2
20ms
```

### Sensor

```text
Core 0
Priority 1
Notification
```

### WiFi

```text
Core 0
Priority 3
10ms
```

---

## 8. 修改 WiFi 时注意

不要从 UI Task 直接调用大量 WiFi 底层操作。

推荐：

```cpp
wifiService.requestXXX();
```

然后：

```text
Queue
 ↓
WiFi Task
 ↓
WifiService
```

这样可以避免 UI Task 被 WiFi 操作阻塞。

---

## 9. 修改传感器时注意

Sensor Task 使用：

```cpp
ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
```

不要在 UI Task 中直接频繁读取 BME280 / CO2。

Sensor 数据通过：

```cpp
g_data
```

提供给 UI。

---

## 10. 接手项目后的第一步

接手人员应该先：

1. 编译项目。
2. 确认 ESP32 型号。
3. 确认 PlatformIO 配置。
4. 检查 Pins.hpp。
5. 检查 Board 初始化。
6. 检查 FreeRTOS Task。
7. 检查 WiFi。
8. 检查 OTA。
9. 最后再开发空调控制。

---

## 11. 当前项目风险

### 风险 1：没有完整实物测试

当前很多功能主要完成代码实现，最终仍需要硬件验证。

重点测试：

- WiFi 扫描
- WiFi 配网
- WiFi 超时
- Retry
- OTA
- LCD
- 传感器
- RGB

### 风险 2：OTA 安全

当前 OTA 没有认证。

只适合受控局域网。

### 风险 3：空调控制未完成

空调控制是当前主要未完成业务功能。

---

## 12. 交接原则

后续开发人员优先保持现有架构。

新增功能建议遵循：

```text
UI
 ↓
Service
 ↓
Driver
 ↓
HAL
 ↓
Hardware
```

不要把所有功能全部写到 `main.cpp`。

不要把底层硬件通信直接写进 Page。

不要让 UI Task 承担长时间阻塞操作。

---

## 13. 项目当前定位

当前项目已经具备一个比较完整的 ESP32 应用软件框架。

已经完成的是：

```text
硬件抽象
+
Driver
+
Service
+
App
+
FreeRTOS
+
WiFi
+
OTA
```

当前最大的功能缺口：

```text
空调实际控制
```

因此下一阶段重点应该放在空调控制模块，而不是继续堆叠 WiFi 功能。
