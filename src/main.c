#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gptimer.h"
#include "driver/gpio.h"

#define GREEN_LED 15
#define YELLOW_LED 16
#define RED_LED 17

enum TRAFFIC_LIGHTS_STATE {
  GO,
  GO_BLINK,
  PREPARE_TO_STOP,
  STOP,
  PREPARE_TO_GO
};
const unsigned int PHASE_DURATION_S[5] = {5, 2, 2, 5, 2};

struct LED_STATE {
  unsigned int green;
  unsigned int yellow;
  unsigned int red;
};
struct LED_STATE led_state = {1, 0, 0};

const uint64_t BLINK_TIMER_PERIOD = 500000; // 0.5s
int current_state = GO;

static bool IRAM_ATTR blink_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
  if (current_state == GO_BLINK) {
    led_state.green = !led_state.green;
    gpio_set_level(GREEN_LED, led_state.green);
  }
  return false;
}

static bool IRAM_ATTR alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {

  current_state = (current_state + 1) % 5;

  switch (current_state) {
    case GO:
      led_state.green = 1;
      led_state.yellow = 0;
      led_state.red = 0;
      break;
    case GO_BLINK:
      break;
    case PREPARE_TO_STOP:
      led_state.green = 0;
      led_state.yellow = 1;
      break;
    case STOP:
      led_state.yellow = 0;
      led_state.red = 1;
      break;
    case PREPARE_TO_GO:
      led_state.yellow = 1;
      break;
    default:
      break;
  }

  gpio_set_level(GREEN_LED, led_state.green);
  gpio_set_level(YELLOW_LED, led_state.yellow);
  gpio_set_level(RED_LED, led_state.red);

  gptimer_alarm_config_t alarm_config = {
    .alarm_count = edata->alarm_value + PHASE_DURATION_S[current_state] * 1000000,
  };
  gptimer_set_alarm_action(timer, &alarm_config);

  return false;
}

void app_main() {

  gpio_config_t gpio_conf = {
    .pin_bit_mask = (1ULL << GREEN_LED) | (1ULL << YELLOW_LED) | (1ULL << RED_LED),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
  };
  gpio_config(&gpio_conf);

  // Initialize LED's
  gpio_set_level(GREEN_LED, led_state.green);
  gpio_set_level(YELLOW_LED, led_state.yellow);
  gpio_set_level(RED_LED, led_state.red);

  // Main timer
  gptimer_handle_t gptimer = NULL;
  gptimer_config_t timer_config = {
    .clk_src = GPTIMER_CLK_SRC_DEFAULT,
    .direction = GPTIMER_COUNT_UP,
    .resolution_hz = 1 * 1000 * 1000,
  };
  gptimer_alarm_config_t alarm_config = {
    .reload_count = 0,
    .alarm_count = PHASE_DURATION_S[current_state] * 1000000,
    .flags.auto_reload_on_alarm = false,
  };
  gptimer_event_callbacks_t cbs = {
    .on_alarm = alarm_cb,
  };

  ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));
  ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));
  ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
  ESP_ERROR_CHECK(gptimer_enable(gptimer));
  ESP_ERROR_CHECK(gptimer_start(gptimer));

  // Blink timer
  gptimer_handle_t blink_timer = NULL;
  gptimer_config_t blink_timer_config = {
      .clk_src = GPTIMER_CLK_SRC_DEFAULT,
      .direction = GPTIMER_COUNT_UP,
      .resolution_hz = 1 * 1000 * 1000,
  };
  gptimer_alarm_config_t blink_alarm_config = {
      .reload_count = 0,
      .alarm_count = BLINK_TIMER_PERIOD,
      .flags.auto_reload_on_alarm = true,
  };
  gptimer_event_callbacks_t blink_cbs = {
      .on_alarm = blink_alarm_cb,
  };

  ESP_ERROR_CHECK(gptimer_new_timer(&blink_timer_config, &blink_timer));
  ESP_ERROR_CHECK(gptimer_set_alarm_action(blink_timer, &blink_alarm_config));
  ESP_ERROR_CHECK(gptimer_register_event_callbacks(blink_timer, &blink_cbs, NULL));
  ESP_ERROR_CHECK(gptimer_enable(blink_timer));
  ESP_ERROR_CHECK(gptimer_start(blink_timer));

  while (1) {
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
