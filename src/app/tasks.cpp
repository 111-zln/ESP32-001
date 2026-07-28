#include "tasks.h"
#include "app.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static void uiTask(void *pv);

void createTasks()
{
    xTaskCreatePinnedToCore(
        uiTask,         // Task函数
        "UI",           // 名字
        4096,           // 栈大小
        nullptr,        // 参数
        2,              // 优先级
        nullptr,        // TaskHandle
        1               // Core1
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