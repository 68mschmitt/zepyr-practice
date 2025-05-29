#include "lv_api_map_v8.h"
#include "widgets/arc/lv_arc.h"
#include "widgets/label/lv_label.h"

static lv_obj_t *label;

int32_t map_value_to_angle(int32_t value, int32_t min_val, int32_t max_val, int32_t start_angle_deg, int32_t end_angle_deg)
{
    if (value < min_val) value = min_val;
    if (value > max_val) value = max_val;

    float ratio = (float)(value - min_val) / (max_val - min_val);
    return (int32_t)(start_angle_deg + ratio * (end_angle_deg - start_angle_deg));
}

static void set_angle(void * obj, int32_t v)
{
    int32_t angle = map_value_to_angle(v, 0, 35, 130, 410); // 280 deg sweep
    lv_arc_set_angles((lv_obj_t *)obj, angle, angle+1);
    angle=0;
    lv_label_set_text_fmt(label, "%d\npsi", v);
}

void lv_example_scale_optimized(void)
{
    /*Create an Arc*/
    lv_obj_t * arc = lv_arc_create(lv_screen_active());
    lv_obj_set_size(arc, 150, 150);
    lv_arc_set_bg_angles(arc, 130, 410);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
    lv_obj_center(arc);

    label = lv_label_create(lv_scr_act());
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_40, 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, arc);
    lv_anim_set_exec_cb(&a, set_angle);
    lv_anim_set_duration(&a, 2000);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);    /*Just for the demo*/
    lv_anim_set_repeat_delay(&a, 500);
    lv_anim_set_values(&a, 0, 35);
    lv_anim_start(&a);
}
