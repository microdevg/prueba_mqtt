# Prueba MQTT usando driver Wifi con callback



Utilizando un driver WiFi con soporte para callback llamo a `mqtt_init()` para iniciar la configuración del cliente MQTT.
```
esp_err_t mqtt_init(const char* uri,
                    esp_callback_t conn,
                    esp_callback_t disconn,
                    callback_data_t data_received);


```

Entre los parámetros ingresados tenemos:
- __uri__ : direccion del broker MQTT a utilizar.
- __conn__:  Callback que se llama al lograr la conexión con el servidor MQTT.
- __discon__: Callback que se llama al desconectarse del servidor.
- __data_received: Callback que se llama cuando se reciben datos.

El callback ``data_received` tiene el siguiente formato:
```
// Formato   void function(char*,char*)

// Ejemplo de función
void MQTT_DATA(char* data, char* topic){
    // Procesamos respuesta
    printf("\n[MQTT] %s\n\t->%s\n",topic,data);
}

```