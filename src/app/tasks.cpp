#include "app/tasks.h"
#include "app.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

static TimerHandle_t sensorTimer = nullptr;
TaskHandle_t sensorTaskHandle = nullptr;

static void uiTask(void *pv)
{
    while (1) {
        app_.update();
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

static void sensorTask(void *pv)
{
    while(1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        sensorService.update();
    }
}

static void wifiTask(void *pv)
{
    while (1) {
        wifiService.update();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

static void sensorTimerCallback(TimerHandle_t timer)
{
    if(sensorTaskHandle != nullptr)
        xTaskNotifyGive(sensorTaskHandle);
}

void createTasks()
{
    xTaskCreate(uiTask,     "UI",     8192, NULL, 1, NULL);
    xTaskCreate(sensorTask, "Sensor", 4096, NULL, 1, &sensorTaskHandle);
    xTaskCreate(wifiTask,   "Wifi",   8192, NULL, 1, NULL);

    sensorTimer = xTimerCreate("SensorTimer", pdMS_TO_TICKS(1000), pdTRUE, NULL, sensorTimerCallback);
    xTimerStart(sensorTimer, 0);
}