#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT custom_led_driver

static const struct gpio_dt_spec sensor_led =
    GPIO_DT_SPEC_GET(DT_ALIAS(sensor_led), gpios);

LOG_MODULE_REGISTER(custom_led, LOG_LEVEL_INF);

static int channel_get_custom_led(const struct device *dev,
                                  enum sensor_channel chan,
                                  struct sensor_value *val) {
  gpio_pin_set_dt(&sensor_led, 0);
  LOG_INF("Custom Led Channel GET");
  return 0;
}

static int sample_fetch_custom_led(const struct device *dev,
                                   enum sensor_channel chan) {
  gpio_pin_set_dt(&sensor_led, 1);
  LOG_INF("Custom Led Sample Fetch");
  return 0;
}

static DEVICE_API(sensor, custom_led_driver_api) = {

    .channel_get = channel_get_custom_led,
    .sample_fetch = sample_fetch_custom_led,
};

static int custom_led_init(const struct device *dev) {
  if (gpio_pin_configure_dt(&sensor_led, GPIO_OUTPUT_INACTIVE) < 0)
    return 0;

  LOG_INF("Custom Led Driver initialized");
  return 0;
}

// Instance Custom Led
DEVICE_DT_INST_DEFINE(0, custom_led_init, NULL, NULL, NULL, POST_KERNEL, 80,
                      &custom_led_driver_api);
