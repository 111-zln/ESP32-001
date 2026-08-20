#include <Arduino.h>
#include "Board.h"
#include "app/tasks.h"
#include "app/app.h"
#include "sensor_service.h"
#include "wifi_service.h"

/* 全局对象放在这里，确保 board_ 先于 app_ 构造 */
Board board_;
App   app_(board_);

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n========== BOOT START ==========");

    Serial.println("[1] board_.init()");
    board_.init();

    Serial.println("[2] loadConfig()");
    loadConfig();

    Serial.println("[3] sensorService.init()");
    sensorService.init(board_.bme_);

    Serial.println("[4] wifiService.init()");
    wifiService.init();          // ← 之前漏了这行，wifiTask 会崩溃

    Serial.println("[5] app_.init()");
    app_.init();

    Serial.println("[6] createTasks()");
    createTasks();

    Serial.println("========== BOOT FINISHED ==========");
}

void loop()
{
    delay(1000);   // Arduino 主循环空转，所有工作交给 FreeRTOS 任务
}