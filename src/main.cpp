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