#include <stdio.h>
#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "sdkconfig.h"

#include "esp_chip_info.h"
#include "esp_system.h"
#include "esp_adc_cal.h"
#include "esp_pm.h"
#include "esp_log.h"

#include "driver/gpio.h"
#include "driver/adc.h"

#include "peripherals.h"
#include "wifi.h"
#include "socket_client.h"

// GPIO settings
#define GPIO_NUM_BUTTON_LEFT   23
#define GPIO_NUM_BUTTON_RIGHT  26
#define GPIO_NUM_BUTTON_SWITCH 21
#define INPUT_SEL              ((1UL << GPIO_NUM_BUTTON_LEFT) | (1UL << GPIO_NUM_BUTTON_RIGHT)| (1UL << GPIO_NUM_BUTTON_SWITCH))

// Queue for peripherals data
QueueHandle_t queue;

// Read joystick and rheostat analogue inputs
void analogue_read_t()
{
    static const adc_bits_width_t width  = ADC_WIDTH_BIT_DEFAULT;

    static const adc_channel_t rheostat_channel = ADC_CHANNEL_7;
    static const adc_atten_t rheostat_atten = ADC_ATTEN_DB_11;

    static const adc_channel_t joystick_channel = ADC_CHANNEL_6;
    static const adc_atten_t joystick_atten = ADC_ATTEN_DB_11;

    adc1_config_width(width);
    adc1_config_channel_atten(rheostat_channel, rheostat_atten);
    adc1_config_channel_atten(joystick_channel, joystick_atten);

    struct Msg rheostat_msg = {RHEOSTAT, 0};
    struct Msg joystick_msg = {JOYSTICK, 0};
    while (1) {
        rheostat_msg.payload = adc1_get_raw((adc1_channel_t)rheostat_channel);
        joystick_msg.payload = adc1_get_raw((adc1_channel_t)joystick_channel);

        xQueueSend(queue, &rheostat_msg, 1);
        xQueueSend(queue, &joystick_msg, 1);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void setup_buttons()
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = INPUT_SEL;
    io_conf.pull_up_en = 1;
    io_conf.pull_down_en = 0;
    gpio_config(&io_conf);
}

void buttons_read_t()
{
    struct Msg r_btn_msg = {BUTTON_R, 0};
    struct Msg l_btn_msg = {BUTTON_L, 0};
    struct Msg switch_msg = {SWITCH, 0};

    while (1) {
        l_btn_msg.payload = gpio_get_level(GPIO_NUM_BUTTON_LEFT);
        xQueueSend(queue, &l_btn_msg, NULL);

        r_btn_msg.payload = gpio_get_level(GPIO_NUM_BUTTON_RIGHT);
        xQueueSend(queue, &r_btn_msg, NULL);

        switch_msg.payload = gpio_get_level(GPIO_NUM_BUTTON_SWITCH);
        xQueueSend(queue, &switch_msg, NULL);

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
