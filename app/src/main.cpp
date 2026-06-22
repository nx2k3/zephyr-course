#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

static const struct device *custom_led =
    DEVICE_DT_GET(DT_NODELABEL(custom_led0));
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int custom_led_channel_get() {
  struct sensor_value val;
  int error_code =
      sensor_channel_get(custom_led, SENSOR_CHAN_AMBIENT_TEMP, &val);
  LOG_INF("Get Channel. Error Code: %d", error_code);
  return 0;
}

int custom_led_sample_fetch() {
  int error_code = sensor_sample_fetch(custom_led);
  LOG_INF("Sample Fetch. Error Code: %d", error_code);
  return 0;
}

int main(void) {
  if (!device_is_ready(custom_led))
    return 0;
  while (1) {
    custom_led_channel_get();
    k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    custom_led_sample_fetch();
    k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
  }
  return 0;
}
