#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>

void createTasks();
extern TaskHandle_t sensorTaskHandle;