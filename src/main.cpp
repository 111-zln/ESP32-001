#include <Arduino.h>
#include "Board.h"
#include "app/tasks.h"
#include "app/app.h"
#include "sensor_service.h"
#include "wifi_service.h"

Board board_;
App   app_(board_);

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== BOOT ===");

    board_.init();
    loadConfig();
    sensorService.init(board_.bme_);
    wifiService.init();   // ← 必须
    app_.init();
    createTasks();

    Serial.println("=== BOOT OK ===");
}

void loop()
{
    delay(1000);
}