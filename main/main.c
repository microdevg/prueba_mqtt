#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "debug.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "wifi_credentials.h"  // lo agregue a .gitignore para no subirlo a internet


#define WIFI_INIT_MESSAGE               "Inicio conexión WiFi\n"
#define WIFI_ID                         WIFI_CREDENTIALS_ID
#define WIFI_PASS                       WIFI_CREDENTIALS_PASS
#define RCONN_TRY_MAX                   5
uint8_t reconnect_try = 0;

static void event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    // WiFi esta listo para conectarse a una red en modo STATION.
    if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START){
        printf("WiFi modo station listo para conectarse a red\n");
        esp_wifi_connect();
    }else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED){
        if(reconnect_try < RCONN_TRY_MAX){
            printf("Wifi desconectado, intentando reconexion:%u / %u\n",reconnect_try,RCONN_TRY_MAX);
            reconnect_try ++;
            esp_wifi_connect();
        }else{
            printf("Fallaron los intentos de reconexion\n");
        }
           
    }

    if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP){
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        printf( "IP:" IPSTR , IP2STR(&event->ip_info.ip));

    }


    if (event_id == WIFI_EVENT_STA_CONNECTED){
    printf("WiFi conectado\n");
    reconnect_try = 0;
    }
     if (event_id == WIFI_EVENT_STA_DISCONNECTED){
    printf("WiFi desconectado\n");
    }

}

static esp_err_t wifi_connect(const char* ID_NAME, const char * PASS){
    esp_err_t err = ESP_OK;
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
     // Configuración por defecto.
    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    // Esta es la primera funcion que debe llamarse para el manejo de WiFi
    ESP_ERROR_CHECK(esp_wifi_init(&config));

    esp_event_handler_instance_t instance_any_id;

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));


    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_ID,
            .password = WIFI_PASS,
            /* Authmode threshold resets to WPA2 as default if password matches WPA2 standards (pasword len => 8).
             * If you want to connect the device to deprecated WEP/WPA networks, Please set the threshold value
             * to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and set the password with length and format matching to
             * WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.
             */
            .threshold.authmode = WIFI_AUTH_WPA_WPA2_PSK ,   // Opcional    WIFI_AUTH_OPEN
        //    .sae_pwe_h2e = WPA3_SAE_PWE_HUNT_AND_PECK,
          //  .sae_h2e_identifier = EXAMPLE_H2E_IDENTIFIER,
        },
    };


    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    printf( "wifi_init_softap finished. SSID:%s password:%s \n",
             WIFI_ID, WIFI_PASS);
    
    return err;
}









void app_main(void)
{
    printf("Inicio\n");
    DEBUG_PRINT(WIFI_INIT_MESSAGE);
     esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_connect(WIFI_ID,WIFI_PASS);

    while(1){
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}