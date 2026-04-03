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

int current_state = GO;
int blink_count = 0;

static bool IRAM_ATTR alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {

  current_state = (current_state + 1) % 5;

  switch (current_state) {
    case GO:
      led_state.green = 1;
      led_state.yellow = 0;
      led_state.red = 0;
      break;
    case GO_BLINK:
      // #TODO: implement blinking
      led_state.green = 0;
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

  gptimer_handle_t gptimer = NULL;
  gptimer_config_t timer_config = {
    .clk_src = GPTIMER_CLK_SRC_DEFAULT,
    .direction = GPTIMER_COUNT_UP,
    .resolution_hz = 1 * 1000 * 1000,
  };

  gptimer_new_timer(&timer_config, &gptimer);

  gptimer_alarm_config_t alarm_config = {
    .reload_count = 0,
    .alarm_count = PHASE_DURATION_S[current_state] * 1000000,
    .flags.auto_reload_on_alarm = false,
  };

  gptimer_set_alarm_action(gptimer, &alarm_config);
  gptimer_event_callbacks_t cbs = {
    .on_alarm = alarm_cb,
  };
  gptimer_register_event_callbacks(gptimer, &cbs, NULL);
  gptimer_enable(gptimer);
  gptimer_start(gptimer);
}
