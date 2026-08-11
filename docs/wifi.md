# WiFi 功能说明

## 1. WiFi 作用

本项目中的 WiFi 主要用途：

1. WiFi 配网
2. 连接用户指定 WiFi
3. WebServer
4. OTA 固件升级

当前没有使用 MQTT。

---

## 2. WiFi 状态

```cpp
enum class WifiState
{
    Idle,
    Scanning,
    ListReady,
    APStarted,
    WaitingPassword,
    Connecting,
    Connected,
    Failed,
    OTAReady,
    OTAUpdating,
    OTASuccess,
    OTAFailed
};
```

---

## 3. WiFi 扫描

进入 WiFi 页面：

```text
WifiPage::onEnter()
        ↓
requestScan()
        ↓
Queue
        ↓
WiFi Task
        ↓
scan()
        ↓
WiFi.scanNetworks()
        ↓
保存前 10 个 WiFi
        ↓
ListReady
```

---

## 4. 选择 WiFi

用户使用：

- UP
- DOWN
- OK

选择目标 WiFi。

OK：

```cpp
wifiService.requestStartConfig(index);
```

然后：

```text
StartConfig
   ↓
选择 SSID
   ↓
启动 ESP32 AP
   ↓
启动 WebServer
   ↓
WaitingPassword
```

---

## 5. ESP32 AP

默认：

```text
SSID: ESP32_Remoter
Password: 12345678
```

ESP32 启动热点后显示 AP IP。

用户连接 ESP32 热点，然后访问 IP。

---

## 6. Web 配网

网页显示当前选择的 SSID：

```text
ESP32 WiFi Config
SSID
Password
Connect
```

提交以后：

```text
/savе
 ↓
读取 password
 ↓
保存 SSID/PWD
 ↓
WiFi.begin()
 ↓
Connecting
```

---

## 7. WiFi 连接

连接成功：

```text
Connecting
    ↓
WL_CONNECTED
    ↓
Connected
    ↓
保存 CONNECTED_BIT
```

同时 RGB：

```text
绿色
```

表示连接成功。

---

## 8. 连接失败

连接超过 15 秒：

```text
Connecting
    ↓
15 秒超时
    ↓
Failed
```

RGB：

```text
红色闪烁 3 次
```

用户按 OK：

```text
Retry
 ↓
重新 WiFi.begin()
 ↓
Connecting
```

---

## 9. RGB 状态

当前只使用三种颜色：

| 颜色 | 含义 |
|---|---|
| 蓝色 | WiFi 扫描 |
| 绿色 | WiFi 连接成功 |
| 红色 | WiFi 连接失败 |

不增加更多颜色，避免用户无法理解颜色含义。

---

## 10. WebServer

WebServer 使用：

```cpp
WebServer server_{80};
```

当前主要接口：

```text
GET /
GET /update
POST /update
```

---

## 11. OTA

WiFi 连接成功以后，可以通过 Web 页面上传固件。

流程：

```text
电脑
 ↓
连接同一 WiFi
 ↓
访问 ESP32 IP
 ↓
/update
 ↓
选择 firmware.bin
 ↓
POST /update
 ↓
Update.begin()
 ↓
Update.write()
 ↓
Update.end()
 ↓
ESP.restart()
```

---

## 12. OTA 状态

```text
OTAReady
   ↓
OTAUpdating
   ↓
OTASuccess
   ↓
重启
```

失败：

```text
OTAUpdating
   ↓
OTAFailed
```

---

## 13. WiFi Task

WiFi Task 每 10ms 调用：

```cpp
wifiService.update();
```

负责：

- 处理 Queue
- 处理 WebServer
- 检查 WiFi 连接
- 检查连接超时
- OTA 上传

---

## 14. 注意事项

目前 OTA 没有做复杂的安全认证。

因此该 OTA 机制适合：

- 局域网
- 开发环境
- 内部设备

如果产品正式量产，建议后续增加：

- OTA 页面认证
- 固件版本检查
- 固件完整性校验
- 更安全的升级方式
