#ifndef CUSTOM_LED_DRIVER_H_
#define CUSTOM_LED_DRIVER_H_
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
struct custom_led_driver_data {
  int blink_count;
};

void custom_led_set_blink_counter(const struct device *dev, int count_value);
#endif
