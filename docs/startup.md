# 系统启动流程

## 1. main.cpp

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

## 2. 启动顺序

```text
ESP32 上电
    ↓
setup()
    ↓
Serial 初始化
    ↓
Board 初始化
    ↓
读取保存配置
    ↓
SensorService 初始化
    ↓
WifiService 初始化
    ↓
App 初始化
    ↓
创建 FreeRTOS Tasks
    ↓
系统进入 FreeRTOS 多任务运行
```

---

## 3. Board 初始化

```cpp
board_.init();
```

Board 初始化：

```text
SPI
I2C
UART
GPIO
RGB
LCD
Buttons
BME280
CO2
```

---

## 4. Sensor 初始化

```cpp
sensorService.init(board_.bme_, board_.co2_);
```

SensorService 保存 BME280 和 CO2 Driver 的引用。

CO2 启动：

```cpp
co2_->start();
```

只执行一次。

---

## 5. WiFi 初始化

```cpp
wifiService.init();
```

主要完成：

```text
WiFi.mode(WIFI_AP_STA)
       ↓
创建 Queue
       ↓
创建 EventGroup
```

---

## 6. App 初始化

```cpp
app_.init();
```

默认页面：

```text
MainMenuPage
```

---

## 7. 创建 Task

```cpp
createTasks();
```

创建：

```text
UI Task
Sensor Task
WiFi Task
Sensor Timer
```

---

## 8. 启动后的运行关系

```text
                 ESP32
                   │
             FreeRTOS Scheduler
                   │
       ┌───────────┼───────────┐
       ↓           ↓           ↓
    UI Task    Sensor Task  WiFi Task
       │           │           │
       ↓           ↓           ↓
      App       Sensor      WifiService
       │        Service         │
       ↓           │            ↓
      LCD          ↓        WiFi/Web/OTA
                  g_data
                    │
                    ↓
                  TempPage
```
