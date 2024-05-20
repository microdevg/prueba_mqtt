#include <string.h>
#include "freertos/FreeRTOS.h"
#include "wifi.h"
#include "freertos/task.h"
#include "wifi_credentials.h"  // lo agregue a .gitignore para no subirlo a internet
#include "mqtt.h"

#define WIFI_INIT_MESSAGE               "Inicio conexión WiFi\n"
#define WIFI_ID                         WIFI_CREDENTIALS_ID
#define WIFI_PASS                       WIFI_CREDENTIALS_PASS


#define MQTT_URL                        "mqtt://broker.hivemq.com"
#define MQTT_TOPIC_SUB                  "RETURN"
#define MQTT_TOPIC_PUB                  "DB"


static int enable = 0;




void MQTT_DATA(char* data, char* topic){
    // Procesamos respuesta
    printf("\n[MQTT] %s\n\t->%s\n",topic,data);
}

void MQTT_CONNECTED(){
    printf("\n[MQTT] Se conecto al servidor %s\n",MQTT_URL);
    mqtt_subcribe(MQTT_TOPIC_SUB, 2);
    enable = 1;
}

void MQTT_DISCONNECTED(){ 
    printf("\n[MQTT] Se desconecto del servidor\n");
  
}


void WIFI_CONNECTED(){
    printf("\nConectado al WiFi, ahora conectarme a los servicios MQTT\n");
    vTaskDelay(5000/ portTICK_PERIOD_MS);
    mqtt_init(MQTT_URL,MQTT_CONNECTED,MQTT_DISCONNECTED,MQTT_DATA);
}


void WIFI_DISCONNECTED(){printf("\nDesconectado del Wifi, liberar recursos y entrar en modo bajo consumo\n");}








void app_main(void)
{
    printf("Inicio\n");
    DEBUG_PRINT(WIFI_INIT_MESSAGE);
    wifi_connect(WIFI_ID,WIFI_PASS,WIFI_CONNECTED,WIFI_DISCONNECTED);
    vTaskDelay(15000/ portTICK_PERIOD_MS);
    char buff[100]={0};
    uint32_t counter = 0;
    while(1){
        sprintf(buff,"\ncounter:%ld\n",counter);
        if(enable)mqtt_publish(buff,MQTT_TOPIC_PUB,2,1);
        printf(buff);
        counter++;
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}