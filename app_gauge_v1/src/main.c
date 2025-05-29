#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <lvgl.h>
#include <lvgl_mem.h>
#include <zephyr/logging/log.h>
// #include "../mods/gaugev1.c"
#include "../mods/gaugeoptimized.c"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

int main(void) {
    const struct device *display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

    if (!device_is_ready(display)) {
        LOG_ERR("Display not ready");
        return 1;
    }

    // Turn on the backlight if needed (optional)
    display_blanking_off(display);

    // lv_example_scale_10();
    lv_example_scale_optimized();

    LOG_INF("Display initialized and text set.");

    while (1) {
        lv_timer_handler();
    }
    return 0;
}
