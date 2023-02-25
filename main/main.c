#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "nvs_flash.h"

#include "peripherals.h"
#include "wifi.h"
#include "socket_client.h"

extern QueueHandle_t queue;

void app_main(void)
{
    queue = xQueueCreate(100, sizeof(struct Msg));
    if (queue == NULL) {
        printf("Can't create queue\n");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        esp_restart();
    }

    setup_buttons();

    xTaskCreate(analogue_read_t, "Analogue read", 4096, NULL, 5, NULL);
    xTaskCreate(buttons_read_t, "Buttons read", 4096, NULL, 5, NULL);

    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_init_sta();
    tcp_client(); 
}