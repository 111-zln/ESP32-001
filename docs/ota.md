# OTA 固件升级

## 1. OTA 目的

本项目使用 WiFi 实现 OTA。

这样设备不需要每次升级都通过 USB 连接电脑烧录。

---

## 2. OTA 入口

WebServer 提供：

```text
GET /update
```

网页：

```text
ESP32 OTA

[选择固件]

[Upload]
```

---

## 3. OTA 上传

提交：

```text
POST /update
```

文件：

```text
firmware.bin
```

---

## 4. OTA 写入流程

```text
浏览器
  ↓
上传 firmware.bin
  ↓
handleOTAUpload()
  ↓
UPLOAD_FILE_START
  ↓
Update.begin()
  ↓
UPLOAD_FILE_WRITE
  ↓
Update.write()
  ↓
UPLOAD_FILE_END
  ↓
Update.end(true)
  ↓
ESP.restart()
```

---

## 5. OTA 状态

### 开始

```cpp
state_ = WifiState::OTAUpdating;
```

### 成功

```cpp
state_ = WifiState::OTASuccess;
```

然后：

```cpp
ESP.restart();
```

### 失败

```cpp
state_ = WifiState::OTAFailed;
```

---

## 6. OTA 与 WiFi 的关系

OTA 不单独创建 Task。

OTA 运行在：

```text
WiFi Task
    ↓
WifiService
    ↓
WebServer
    ↓
OTA
```

因此 OTA 是 WiFi 功能的一部分。

---

## 7. OTA 当前限制

当前实现主要用于开发阶段。

还没有实现：

- 用户认证
- 固件版本管理
- 签名验证
- HTTPS
- 回滚策略
- OTA 失败后的高级恢复策略

正式产品化时再根据项目要求增加。

---

## 8. OTA 使用方式

1. ESP32 连接 WiFi。
2. 查看 ESP32 IP。
3. 电脑连接同一个局域网。
4. 浏览器访问：

```text
http://ESP32_IP/update
```

5. 选择新的 `firmware.bin`。
6. 点击 Upload。
7. 等待 ESP32 自动重启。
8. 新固件启动。

---

## 9. 当前 OTA 定位

当前 OTA 已经具备基本闭环：

```text
WiFi
 ↓
WebServer
 ↓
上传固件
 ↓
写入 Flash
 ↓
校验结束
 ↓
重启
```

因为目前没有实际硬件测试，OTA 的最终可靠性仍需要接上实物以后验证。
