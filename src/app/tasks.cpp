#include "app/tasks.h"
#include "app.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

static TimerHandle_t sensorTimer = nullptr;

TaskHandle_t sensorTaskHandle = nullptr;
static TaskHandle_t uiTaskHandle   = nullptr;
static TaskHandle_t wifiTaskHandle = nullptr;

/* ========== 任务函数（定义在 createTasks 之前） ========== */

static void uiTask(void *pv)
{
    while (1)
    {
        app_.update();
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

static void sensorTask(void *pv)
{
    while(1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        sensorService.update();
    }
}

static void wifiTask(void *pv)
{
    while (1)
    {
        wifiService.update();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

static void sensorTimerCallback(TimerHandle_t timer)
{
    if(sensorTaskHandle != nullptr)
    {
        xTaskNotifyGive(sensorTaskHandle);
    }
}

/* ========== 任务创建 ========== */

void createTasks()
{
    xTaskCreatePinnedToCore(uiTask,     "UI",     4096, nullptr, 2, &uiTaskHandle,   1);
    xTaskCreatePinnedToCore(sensorTask, "Sensor", 4096, nullptr, 1, &sensorTaskHandle, 0);
    xTaskCreatePinnedToCore(wifiTask,   "Wifi",   8192, nullptr, 3, &wifiTaskHandle, 0);

    sensorTimer = xTimerCreate("SensorTimer", pdMS_TO_TICKS(1000), pdTRUE, nullptr, sensorTimerCallback);
    xTimerStart(sensorTimer, 0);
}