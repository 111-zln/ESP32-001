# FreeRTOS 任务说明

## 1. Task 总览

| Task | Core | Priority | 周期/机制 | 作用 |
|---|---:|---:|---|---|
| UI Task | Core 1 | 2 | 20 ms | 页面、按键、显示 |
| Sensor Task | Core 0 | 1 | Notification | 传感器 |
| WiFi Task | Core 0 | 3 | 10 ms | WiFi、Web、OTA |

---

## 2. UI Task

```cpp
static void uiTask(void *pv)
{
    while (1)
    {
        app_.update();
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
```

负责：

- 按键
- 页面逻辑
- 页面切换
- LCD 刷新

---

## 3. Sensor Task

```cpp
static void sensorTask(void *pv)
{
    while(1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        sensorService.update();
    }
}
```

Sensor Task 默认阻塞。

Timer 每 1 秒通知一次：

```cpp
xTaskNotifyGive(sensorTaskHandle);
```

流程：

```text
Timer
 ↓
Notification
 ↓
Sensor Task 唤醒
 ↓
sensorService.update()
 ↓
再次阻塞
```

---

## 4. WiFi Task

```cpp
static void wifiTask(void *pv)
{
    while (1)
    {
        wifiService.update();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
```

负责：

- Queue
- WiFi 状态
- WebServer
- OTA
- 连接超时

---

## 5. Queue

WiFi 使用 Queue：

```cpp
enum class WifiCommand
{
    Scan,
    StartConfig,
    Retry
};
```

UI 调用：

```cpp
wifiService.requestScan();
wifiService.requestStartConfig(index);
wifiService.requestRetry();
```

实际由 WiFi Task 执行。

---

## 6. EventGroup

WiFi 使用：

```cpp
WIFI_CONNECTING_BIT
WIFI_CONNECTED_BIT
```

用于保存连接状态。

---

## 7. WiFi 超时

连接超时时间：

```text
15 秒
```

超过以后：

```text
Connecting
    ↓
Timeout
    ↓
disconnect()
    ↓
Failed
    ↓
等待 Retry
```

---

## 8. Task 分配

```text
ESP32
│
├── Core 0
│   ├── WiFi Task  Priority 3
│   └── Sensor Task Priority 1
│
└── Core 1
    └── UI Task     Priority 2
```

---

## 9. 当前 FreeRTOS 机制

- [x] Task
- [x] Task Priority
- [x] Core Pinning
- [x] Queue
- [x] EventGroup
- [x] Task Notification
- [x] Software Timer
- [x] WiFi Timeout
- [x] WiFi Retry
