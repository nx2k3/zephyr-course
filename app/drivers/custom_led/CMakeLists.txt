if(CONFIG_CUSTOM_LED_DRIVER)
    target_sources(app PRIVATE custom_led_driver.c custom_led_driver.h)
endif()
