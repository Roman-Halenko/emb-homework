#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gptimer.h"

#define GREEN_LED
#define YELLOW_LED
#define RED_LED

static bool example_timer_on_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {

  // 1. solid GREEN for 5 sec
  // 2. blinking GREEN for 2 sec (500ms)
  // 3. solid YELLOW for 2 sec
  // 4. solid RED for 5 sec
  // 5. solid RED + YELLOW for 2 sec

  gptimer_alarm_config_t alarm_config = {
    .alarm_count = edata->alarm_value + 1000000, // Next alarm in 1s from the current alarm
  };
  // Update the alarm value
  gptimer_set_alarm_action(timer, &alarm_config);
  return false;
}

void app_main() {

  gptimer_handle_t gptimer = NULL;
  gptimer_config_t timer_config = {
    .clk_src = GPTIMER_CLK_SRC_DEFAULT,
    .direction = GPTIMER_COUNT_UP,
    .resolution_hz = 1 * 1000 * 1000,
  };

  gptimer_new_timer(&timer_config, &gptimer);

  gptimer_alarm_config_t alarm_config = {
    .reload_count = 0,                  // When the alarm event occurs, the timer will automatically reload to 0
    .alarm_count = 1000000,             // Set the actual alarm period, since the resolution is 1us, 1000000 represents 1s
    .flags.auto_reload_on_alarm = true, // Enable auto-reload function
  };

  // Set the timer's alarm action
  gptimer_set_alarm_action(gptimer, &alarm_config);
  gptimer_event_callbacks_t cbs = {
      .on_alarm = example_timer_on_alarm_cb, // Call the user callback function when the alarm event occurs
  };
  gptimer_register_event_callbacks(gptimer, &cbs, NULL);
  gptimer_enable(gptimer);
  gptimer_start(gptimer);

  while(1) {
  }
}
