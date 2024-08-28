#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED     GPIO_NUM_16
#define HIGH    1
#define LOW     0

static const char* TAG = "LOG"; // Necesario para el ESP_LOGI
esp_err_t init_led(void);
esp_err_t blink_led(void);

void app_main(void)
{
    
    init_led();

    while (1)
    {
        blink_led();
    }
    
}

esp_err_t init_led(void)
{
    gpio_reset_pin(LED); //Reseteo del pin a su estado default
    gpio_set_direction(LED, GPIO_MODE_OUTPUT); // Configuramos el pin asociado al LED como salida
    gpio_dump_io_configuration(stdout, 1ULL << LED); // Visualizamos la configuración del LED

    return ESP_OK;
}

esp_err_t blink_led(void)
{
    gpio_set_level(LED, LOW); // Seteamos el nivel del LED en bajo
    vTaskDelay(1000/ portTICK_PERIOD_MS); // Delay de 1000 ms   
    ESP_LOGI(TAG, "El led se encuentra encendido"); 
    gpio_set_level(LED, HIGH); // Seteamos el nivel del LED en alto
    vTaskDelay(1000/ portTICK_PERIOD_MS); // Delay de 1000 ms   
    ESP_LOGI(TAG, "El led se encuentra apagado");

    return ESP_OK;
}