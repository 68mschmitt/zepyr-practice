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

    if (!device_is_ready(display_dev)) {
        // LOG_ERR("Display not ready");
        return -1;
    }

#ifdef CONFIG_LV_Z_MEM_POOL_SYS_HEAP
    lvgl_print_heap_info(false);
#else
    LOG_INF("LVGL running in malloc mode");
#endif

    display_blanking_off(display_dev);

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_center(label);

    int i = 0;

    while (1) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d", i++);
        lv_label_set_text(label, buf);

        lv_timer_handler();
    }
}
