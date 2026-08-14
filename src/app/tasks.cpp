#include "app/tasks.h"
#include "app.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"


static TimerHandle_t sensorTimer = nullptr;

TaskHandle_t sensorTaskHandle = nullptr;
static TaskHandle_t uiTaskHandle     = nullptr;
static TaskHandle_t wifiTaskHandle   = nullptr;

void createTasks()
{
    //ui task
    xTaskCreatePinnedToCore(
        uiTask,         // Task函数
        "UI",           // 名字
        4096,           // 栈大小
        nullptr,        // 参数
        2,              // 优先级
        &uiTaskHandle,  // TaskHandle
        1               // Core1
    );
 
    //sensor task
    xTaskCreatePinnedToCore(
        sensorTask,
        "Sensor",
        4096,
        nullptr,
        1,
        &sensorTaskHandle,
        0
    );

    //wifi task
    xTaskCreatePinnedToCore(
        wifiTask,
        "Wifi",
        4096,
        nullptr,
        3,
        &wifiTaskHandle,
        0
    );


    //sensor timer
    sensorTimer = xTimerCreate(
        "SensorTimer",              // 名字
        pdMS_TO_TICKS(1000),        // 周期
        pdTRUE,                     // 自动循环
        nullptr,                    // 用户参数
        sensorTimerCallback         // 回调函数
    );
    xTimerStart(sensorTimer, 0);
}

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
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);//睡觉，直到有人通知我

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
