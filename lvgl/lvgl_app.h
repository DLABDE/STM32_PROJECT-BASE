/**
 * @file lvgl_app.h
 * user LVGL app
 */

#ifndef _LVGL_APP_H
#define _LVGL_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"


void lvgl_tick(uint32_t tick);
void lvgl_app_init(void);
void lvgl_app(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*_LVGL_APP_H*/
