#include <errno.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/led.h>
#include <zephyr/init.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>

#define STARTUP_LED_COUNT 18
#define STARTUP_BRIGHTNESS 20

static const struct device *const led_dev = DEVICE_DT_GET_ANY(ti_lp5018);

static int led_startup_init(void)
{
    if (!device_is_ready(led_dev))
    {
        LOG_ERR("LED device not ready");
        return -ENODEV;
    }

    for (int led_idx = 0; led_idx < STARTUP_LED_COUNT; led_idx++)
    {
        int ret = led_on(led_dev, led_idx);
        if (ret < 0)
        {
            LOG_ERR("Failed to enable LED %d (%d)", led_idx, ret);
            return ret;
        }

        ret = led_set_brightness(led_dev, led_idx, STARTUP_BRIGHTNESS);
        if (ret < 0)
        {
            LOG_ERR("Failed to set LED %d brightness (%d)", led_idx, ret);
            return ret;
        }
    }

    LOG_INF("Set %d white LEDs to %d%% brightness", STARTUP_LED_COUNT, STARTUP_BRIGHTNESS);
    return 0;
}

SYS_INIT(led_startup_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
