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
  struct sensor_value val;
  sensor_channel_get(custom_led_dev, SENSOR_CHAN_AMBIENT_TEMP, &val);
  shell_fprintf(sh, SHELL_INFO, "Sensor Read,Led off, %d\n", val.val1);
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
SHELL_STATIC_SUBCMD_SET_CREATE(
    sensor_subcmd, SHELL_CMD(fetch, NULL, "Sensor Fetch", cmd_sensor_fetch),
    SHELL_CMD(read, NULL, "Sensor Read", cmd_sensor_read),
    SHELL_CMD(info, NULL, "Sensor Info", cmd_sensor_info),
    SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(sensor, &sensor_subcmd, "Sensor commands", NULL);
