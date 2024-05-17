#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "debug.h"
#include "freertos/task.h"

#define WIFI_INIT_MESSAGE               "Inicio conexión WiFi\n"


void app_main(void)
{
    printf("Inicio\n");
    DEBUG_PRINT(WIFI_INIT_MESSAGE);

    while(1){
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}