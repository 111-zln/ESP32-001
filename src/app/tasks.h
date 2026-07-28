#pragma once



void createTasks();


//把功能转化为task，这样就能并行
static void uiTask(void *pv);
static void sensorTask(void *pv);
static void wifiTask(void *pv);