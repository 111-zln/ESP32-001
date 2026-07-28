#include <Arduino.h>
#include "Board.h"
#include "app/tasks.h"
#include "app/app.h"
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