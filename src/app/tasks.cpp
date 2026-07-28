#include "tasks.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

static void uiTask(void *pv);
static void sensorTask(void *pv);
static void wifiTask(void *pv);

void createTasks()
{
    //xTaskCreatePinnedToCore();

    //xTaskCreatePinnedToCore();

    //xTaskCreatePinnedToCore();
}

static void uiTask(void *pv)
{
    while (1)
    {
        
    }
}

static void sensorTask(void *pv)
{
    while (1)
    {
        
    }
}