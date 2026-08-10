#include <Arduino.h>
#include "Board.h"
#include "app/tasks.h"
#include "app/app.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sensor_service.h"

void setup()
{
    Serial.begin(115200);

    // 1. 硬件初始化
    board_.init();

    // 2. 读取保存的配置
    loadConfig();

    // 3. Service 初始化
    sensorService.init(board_.bme_, board_.co2_);
    wifiService.init();

    // 4. App 初始化
    app_.init();

    // 5. 创建 FreeRTOS Tasks
    createTasks();
}

void loop()
{
    vTaskDelete(nullptr);
}