#pragma
#include "Arduino.h"
#include <WebServer.h>
#include "data.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "display_service.h"

//mqtt连接
void mqtt_connect();

void mqtt_loop();

void publishStatus();//心跳上报