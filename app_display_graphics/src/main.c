#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>
#include <lvgl_mem.h>
#include <lv_demos.h>
#include <zephyr/logging/log.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
LOG_MODULE_REGISTER(main, LOG_LEVEL);

int main(void)
{
    const struct device *display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

    lv_init();
    if (!device_is_ready(display_dev)) {
        // LOG_ERR("Display not ready");
        return -1;
    }

    display_blanking_off(display_dev);
    lv_demo_widgets();

        lv_demo_benchmark();
    while (1) {
        lv_timer_handler();
        k_msleep(1000);
    }
}
