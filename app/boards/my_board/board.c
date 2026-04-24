#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(my_board, LOG_LEVEL_INF);

void board_early_init_hook(void) {
    /* Some function to run befoer boot */
    LOG_INF("Board Initialized\n");
}