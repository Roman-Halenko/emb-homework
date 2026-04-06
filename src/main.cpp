#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>

#define ADC_PIN ADC_CHANNEL_3 // GPIO 4
#define ADC_UNIT ADC_UNIT_1
#define ADC_ATTEN ADC_ATTEN_DB_12
#define ADC_BITWIDTH ADC_BITWIDTH_12
#define ADC_MAX 4095
#define U_REF_MV 3300

const char TAG[] = "ADC";

extern "C" void app_main() {
  int adc_raw_val = 0;
  int U_cali = 0;

  adc_oneshot_unit_handle_t adc_handle;
  adc_oneshot_unit_init_cfg_t init_config = {
    .unit_id = ADC_UNIT,
    .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
  };
  ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

  adc_oneshot_chan_cfg_t config = {
    .atten = ADC_ATTEN,
    .bitwidth = ADC_BITWIDTH,
  };
  adc_cali_handle_t adc_cali_handle;
  adc_cali_curve_fitting_config_t cali_config = {
    .unit_id = ADC_UNIT,
    .chan = ADC_PIN,
    .atten = ADC_ATTEN,
    .bitwidth = ADC_BITWIDTH,
  };
  ESP_ERROR_CHECK(adc_cali_create_scheme_curve_fitting(&cali_config, &adc_cali_handle));
  ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_PIN, &config));

  while(1) {
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_PIN, &adc_raw_val));
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc_cali_handle, adc_raw_val, &U_cali));

    int U_manual = (adc_raw_val * U_REF_MV) / ADC_MAX;
    float error = ((float)(U_cali - U_manual) / U_cali) * 100;

    ESP_LOGI(TAG, "RAW: %d, Calculated: %d, Calibrated: %d, Error: %.2f%%", adc_raw_val, U_manual, U_cali, error);

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
