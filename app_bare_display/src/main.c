#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <lvgl.h>
#include <lvgl_mem.h>
#include <lv_demos.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

int main(void) {
    const struct device *display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

    if (!device_is_ready(display)) {
        LOG_ERR("Display not ready");
        return 1;
    }

    // Turn on the backlight if needed (optional)
    display_blanking_off(display);

    lv_example_scale_10();

#if CONFIG_LV_USE_DEMO_TRANSFORM
    lv_demo_transform();
#endif
#if CONFIG_LV_USE_DEMO_RENDER
    k_timepoint_t next_scene_switch;
    lv_demo_render_scene_t cur_scene = LV_DEMO_RENDER_SCENE_FILL;

    lv_demo_render(cur_scene, 255);
    next_scene_switch =
        sys_timepoint_calc(K_SECONDS(3));
#endif

    LOG_INF("Display initialized and text set.");

    while (1) {
        lv_timer_handler();       // Handle LVGL internal updates
        k_msleep(20);             // Give LVGL time to update

#if CONFIG_LV_USE_DEMO_RENDER

        if (sys_timepoint_expired(next_scene_switch)) {
            cur_scene = (cur_scene + 1) % 6;
            lv_demo_render(cur_scene, 255);
            next_scene_switch = sys_timepoint_calc(
                    K_SECONDS(3));
        }
#endif
    }
    return 0;
}
