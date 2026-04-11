#include <driver/ledc.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include "SMA.h"

#define LED_PIN GPIO_NUM_17
#define MOTOR_PIN GPIO_NUM_15
#define POT_PIN ADC_CHANNEL_3 // GPIO 4

#define ADC_UNIT ADC_UNIT_1
#define ADC_ATTEN ADC_ATTEN_DB_12
#define ADC_BITWIDTH ADC_BITWIDTH_12
#define ADC_MAX 4095

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_RESOLUTION LEDC_TIMER_10_BIT
#define LEDC_FREQUENCY 20000 // 20 kHz
#define LEDC_MAX 1023

#define LEDC_MOTOR_CHANNEL LEDC_CHANNEL_0
#define LEDC_LED_CHANNEL LEDC_CHANNEL_1

const char TAG[] = "ADC";

extern "C" void app_main() {

  const size_t SMA_BUFFER = 5;

  SMA<SMA_BUFFER> adc_filter;

  int adc_raw_val = 0;

  // Common timer
  ledc_timer_config_t timer_config = {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty_resolution = LEDC_RESOLUTION,
    .timer_num = LEDC_TIMER,
    .freq_hz = LEDC_FREQUENCY,
    .clk_cfg = LEDC_AUTO_CLK,
    .deconfigure = false
  };
  ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

  // Motor channel
  ledc_channel_config_t motor_channel_config = {
    .gpio_num = MOTOR_PIN,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = LEDC_MOTOR_CHANNEL,
    .timer_sel = LEDC_TIMER,
    .duty = 0,
    .hpoint = 0,
  };
  ESP_ERROR_CHECK(ledc_channel_config(&motor_channel_config));

  // LED channel
  ledc_channel_config_t led_channel_config = {
    .gpio_num = LED_PIN,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = LEDC_LED_CHANNEL,
    .timer_sel = LEDC_TIMER,
    .duty = 0,
    .hpoint = 0
  };
  ESP_ERROR_CHECK(ledc_channel_config(&led_channel_config));

  // Potentiometer input
  adc_oneshot_unit_handle_t adc_handle;
  adc_oneshot_unit_init_cfg_t init_config = {
    .unit_id = ADC_UNIT,
    .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
    .ulp_mode = ADC_ULP_MODE_DISABLE,
  };
  ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

  adc_oneshot_chan_cfg_t oneshot_chan_config = {
    .atten = ADC_ATTEN,
    .bitwidth = ADC_BITWIDTH,
  };
  ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, POT_PIN, &oneshot_chan_config));

  while(1) {
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, POT_PIN, &adc_raw_val));
    uint16_t filtered_value = adc_filter.process(adc_raw_val);
    int dutyCycle = (filtered_value * LEDC_MAX) / ADC_MAX;

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_MOTOR_CHANNEL, dutyCycle);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_MOTOR_CHANNEL);

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_LED_CHANNEL, dutyCycle);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_LED_CHANNEL);

    // ESP_LOGI(TAG, "Pot: %d; Duty: %d; Percent:%d%%", filtered_value, dutyCycle, (dutyCycle * 100) / 1023);

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
