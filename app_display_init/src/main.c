#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <lvgl.h>
#include <lvgl_mem.h>
#include <lv_demos.h>
#include <zephyr/logging/log.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
LOG_MODULE_REGISTER(main, LOG_LEVEL);

#define GP_0_PINS DT_NODELABEL(gpio0) // GPIO controller 0
#define GP_1_PINS DT_NODELABEL(gpio1) // GPIO controller 0
#define VCC_CTRL_PIN 13 // vcc control
#define CS_PIN 20 // cs pin
#define RESET_PIN 17 // rst

int main(void)
{
    const struct device *display_dev;
    lv_obj_t *hello_world_label;
    lv_obj_t *count_label;

    display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (!device_is_ready(display_dev)) {
        LOG_ERR("Device not ready, aborting test");
        return 0;
    }

    hello_world_label = lv_label_create(lv_screen_active());

    lv_label_set_text(hello_world_label, "Hello world!");
	lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);

	count_label = lv_label_create(lv_screen_active());
	lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, 0, 0);

	lv_timer_handler();
	display_blanking_off(display_dev);


    while (1) {
        k_msleep(10);
        lv_timer_handler();
    }
}
