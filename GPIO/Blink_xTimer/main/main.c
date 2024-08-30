#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#define LED GPIO_NUM_16

static const char* TAG = "LOG";
esp_err_t init_led(void);
esp_err_t blink_led(void);
uint8_t led_level = 0;
uint8_t timer_interval = 1000;

TimerHandle_t xTimer;
void init_timer(void);
void vTimerCallback(TimerHandle_t xTimer);


void app_main(void)
{
    init_led();
    init_timer();   
}

/*Se llama a esta función cuando el timer expira*/
void vTimerCallback(TimerHandle_t xTimer)
{
    uint32_t timer_count = 0;
    const uint32_t maximum_expiry_count = 20;

    configASSERT(xTimer != NULL); // Verificación de que xTimer no sea nulo
    timer_count = (uint32_t)pvTimerGetTimerID(xTimer); // Obtenemos las veces que el timer ha expirado
    ESP_LOGI(TAG, "El timer ha expirado %ld veces", timer_count++);

    // Si el timer ha expirado 20 veces detenerlo
    if(timer_count >= maximum_expiry_count)
    {
        xTimerStop(xTimer, 0);
        ESP_LOGI(TAG, "El timer se ha detenido después de haber expirado %ld veces.", maximum_expiry_count);
    }
    else
    {
        vTimerSetTimerID(xTimer, (void *)timer_count);
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
    led_level = !led_level;
    gpio_set_level(LED, led_level);
    return ESP_OK;
}

void init_timer(void)
{
    ESP_LOGI(TAG, "Inicio de la configuración del timer");

    xTimer = xTimerCreate("TimerLED", // Nombre para el debug, no usado por el kernel
                          pdMS_TO_TICKS(timer_interval), // El periodo del timer en ticks
                          pdTRUE, // El timer se reanudará automáticamente cuando expire
                          (void *)0, // ID es usado para almacenar la cantidad de veces que el timer ha expirado 
                          vTimerCallback //El timer llama a esta función cuando expira
                          );
    
    if (xTimer == NULL)
    {
        ESP_LOGE(TAG, "El timer no fue creado");
    }
    else 
    {
        // Iniciar el timer 
        if(xTimerStart(xTimer, 0) != pdPASS)
        {
            ESP_LOGE(TAG, "El timer no pudo ser activado");
        }
    }
}

