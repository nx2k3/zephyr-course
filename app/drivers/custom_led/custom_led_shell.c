#include "custom_led_driver.h"
#include <sys/errno.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>
static const struct device *custom_led_dev =
    DEVICE_DT_GET(DT_NODELABEL(custom_led0));

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(argc);
  ARG_UNUSED(argv);
  sensor_sample_fetch(custom_led_dev);
  shell_info(sh, "Sensor Sample Fetch executed\nLed set");
  return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(argc);
  ARG_UNUSED(argv);
  struct custom_led_driver_data *data = custom_led_dev->data;
  struct sensor_value val;
  sensor_channel_get(custom_led_dev, SENSOR_CHAN_AMBIENT_TEMP, &val);
  shell_fprintf(sh, SHELL_INFO, "Sensor Read,Led off,blink count %d\n",
                data->blink_count);
  return 0;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv) {
  ARG_UNUSED(argc);
  ARG_UNUSED(argv);
  shell_fprintf(sh, SHELL_INFO, "Device Name: %s,Ready state %s\n",
                custom_led_dev->name,
                device_is_ready(custom_led_dev) ? "ON" : "OFF");
  return 0;
}

static int cmd_sensor_set_value(const struct shell *sh, size_t argc,
                                char **argv) {
  int error_code;
  long value_recvieved = shell_strtol(argv[1], 10, &error_code);
  if (value_recvieved < 0 || value_recvieved > 100) {
    shell_fprintf_error(
        sh, "The value must be in range of 0 to 100,it can't be %ld\n",
        value_recvieved);
    return -EINTR;
  }
  shell_fprintf_info(sh, "Setting the blink counter value to %ld\n",
                     value_recvieved);
  custom_led_set_blink_counter(custom_led_dev, value_recvieved);
  return 0;
}
SHELL_STATIC_SUBCMD_SET_CREATE(
    sensor_subcmd, SHELL_CMD(fetch, NULL, "Sensor Fetch", cmd_sensor_fetch),
    SHELL_CMD(read, NULL, "Sensor Read", cmd_sensor_read),
    SHELL_CMD(info, NULL, "Sensor Info", cmd_sensor_info),
    SHELL_CMD_ARG(set, NULL, "Sensor Set Value", cmd_sensor_set_value, 2, 0),
    SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(sensor, &sensor_subcmd, "Sensor commands", NULL);
