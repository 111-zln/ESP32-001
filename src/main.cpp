#include <Arduino.h>
#include "../bsp/Board.h"
#include "../src/app/tasks.h"
#include "../src/app/app.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void setup()
{
    board_.init();

    app_.init();

    createTasks();
}

void loop()
{
}