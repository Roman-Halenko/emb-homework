#include <driver/gpio.h>
#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include "SMA.h"

#define LED_PIN GPIO_NUM_17
#define ADC_PIN ADC_CHANNEL_3 // GPIO 4
#define ADC_UNIT ADC_UNIT_1
#define ADC_ATTEN ADC_ATTEN_DB_12
#define ADC_BITWIDTH ADC_BITWIDTH_12

const char TAG[] = "ADC";

extern "C" void app_main() {
  const int SWITCH_POINT = 2600; // The greater value, the darker it must be to LED turn on
  const int THRESHOLD = 100;
  const size_t SMA_BUFFER = 8;

  SMA<SMA_BUFFER> adc_filter;

  int adc_raw_val = 0;
  bool led_on = false;

  gpio_config_t gpio_conf = {
    .pin_bit_mask = (1ULL << LED_PIN),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
  };
  ESP_ERROR_CHECK(gpio_config(&gpio_conf));
  ESP_ERROR_CHECK(gpio_set_level(LED_PIN, 0));

  adc_oneshot_unit_handle_t adc_handle;
  adc_oneshot_unit_init_cfg_t init_config = {
    .unit_id = ADC_UNIT,
    .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
    .ulp_mode = ADC_ULP_MODE_DISABLE,
  };
  ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

  adc_oneshot_chan_cfg_t config = {
    .atten = ADC_ATTEN,
    .bitwidth = ADC_BITWIDTH,
  };
  ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_PIN, &config));

  while(1) {
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_PIN, &adc_raw_val));
    uint16_t filtered_value = adc_filter.process(adc_raw_val);

    if(filtered_value <= SWITCH_POINT - THRESHOLD && led_on) {
      ESP_ERROR_CHECK(gpio_set_level(LED_PIN, 0));
      led_on = false;
    } else if (filtered_value >= SWITCH_POINT + THRESHOLD && !led_on) {
      ESP_ERROR_CHECK(gpio_set_level(LED_PIN, 1));
      led_on = true;
    }

    // ESP_LOGI(TAG, "RAW: %d; SMA: %d;", adc_raw_val, filtered_value);

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
