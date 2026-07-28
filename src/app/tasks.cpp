#include "tasks.h"
#include "app.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static void uiTask(void *pv);

void createTasks()
{
    //ui
    xTaskCreatePinnedToCore(
        uiTask,         // Task函数
        "UI",           // 名字
        4096,           // 栈大小
        nullptr,        // 参数
        2,              // 优先级
        nullptr,        // TaskHandle
        1               // Core1
    );

    //sensor
    xTaskCreatePinnedToCore(
        sensorTask,
        "Sensor",
        4096,
        nullptr,
        1,
        nullptr,
        0
    );

    //wifi
    xTaskCreatePinnedToCore(
        wifiTask,
        "Wifi",
        4096,
        nullptr,
        3,
        nullptr,
        0
    );
    
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
        sensorService.update();

        vTaskDelay(pdMS_TO_TICKS(1000));
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