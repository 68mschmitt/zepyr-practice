#include "misc/lv_color.h"
#include "misc/lv_palette.h"
#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <lvgl.h>
#include <lvgl_mem.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static lv_style_t style_scaleNoLine;

void draw_gauge(void) {
  lv_style_init(&style_scaleNoLine);

  lv_style_set_arc_width(&style_scaleNoLine, 0);
  lv_style_set_line_width(&style_scaleNoLine, 0);

  //Scale
  lv_obj_t * scale = lv_scale_create(lv_screen_active());
  lv_scale_set_label_show(scale, true);
  lv_scale_set_mode(scale, LV_SCALE_MODE_ROUND_INNER);     //Round, ticks on inside
  lv_scale_set_draw_ticks_on_top(scale, true);

  //Ticks
  lv_scale_set_total_tick_count(scale, 5);
  lv_scale_set_major_tick_every(scale, 4);
  lv_obj_set_style_length(scale, 10, LV_PART_INDICATOR);   //Major ticks

  //Main style
  lv_obj_set_style_bg_color(scale, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(scale, LV_RADIUS_CIRCLE, LV_PART_MAIN | LV_STATE_DEFAULT);

  //Add style remove outer border line of scale
  lv_obj_add_style(scale, &style_scaleNoLine, LV_PART_MAIN);

  //Add sections to color blue/green/red, width 15
  static lv_style_t style_first_section;
  lv_style_init(&style_first_section);
  lv_style_set_arc_color(&style_first_section, lv_palette_main(LV_PALETTE_RED));
  lv_style_set_arc_width(&style_first_section, 10);

  static lv_style_t style_mid_section;
  lv_style_init(&style_mid_section);
  lv_style_set_arc_color(&style_mid_section, lv_palette_main(LV_PALETTE_GREEN));
  lv_style_set_arc_width(&style_mid_section, 10);

  static lv_style_t style_end_section;
  lv_style_init(&style_end_section);
  lv_style_set_arc_color(&style_end_section, lv_palette_main(LV_PALETTE_RED));
  lv_style_set_arc_width(&style_end_section, 10);

  lv_scale_section_t * section_first = lv_scale_add_section(scale);
  lv_scale_section_set_range(section_first, 0, 5);
  lv_scale_section_set_style(section_first, LV_PART_MAIN, &style_first_section);
  lv_scale_section_set_style(section_first, LV_PART_INDICATOR, &style_first_section);
  lv_scale_section_set_style(section_first, LV_PART_ITEMS, &style_first_section);

  lv_scale_section_t * section_mid = lv_scale_add_section(scale);
  lv_scale_section_set_range(section_mid, 5, 20);
  lv_scale_section_set_style(section_mid, LV_PART_MAIN, &style_mid_section);
  lv_scale_section_set_style(section_mid, LV_PART_INDICATOR, &style_mid_section);
  lv_scale_section_set_style(section_mid, LV_PART_ITEMS, &style_mid_section);

  lv_scale_section_t * section_end = lv_scale_add_section(scale);
  lv_scale_section_set_range(section_end, 20, 30);
  lv_scale_section_set_style(section_end, LV_PART_MAIN, &style_end_section);
  lv_scale_section_set_style(section_end, LV_PART_INDICATOR, &style_end_section);
  lv_scale_section_set_style(section_end, LV_PART_ITEMS, &style_end_section);

  //Range and angle / rotation
  lv_scale_set_range(scale, 0, 30);
  lv_scale_set_rotation(scale, 120);
  lv_scale_set_angle_range(scale, 300);

  //Size and alignment
  lv_obj_set_size(scale, 240, 240);
  lv_obj_center(scale);
}

int main(void) {
    const struct device *display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

    if (!device_is_ready(display)) {
        LOG_ERR("Display not ready");
        return 1;
    }

    // Turn on the backlight if needed (optional)
    display_blanking_off(display);

    draw_gauge();

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
