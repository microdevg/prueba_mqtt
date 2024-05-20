#ifndef _MQTT_H
#define _MQTT_H
#include "base.h"




esp_err_t mqtt_init(const char* uri, esp_callback_t conn, esp_callback_t disconn,callback_data_t data_received);

esp_err_t mqtt_publish(const char* msg, const char* topic,int qos, int retain);

esp_err_t mqtt_publish_on_queue(const char* msg, const char* topic,int qos, int retain);

esp_err_t mqtt_subcribe( char* topic, int qos);

#endif