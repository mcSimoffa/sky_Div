#ifndef _APP_TIMER_H__
#define _APP_TIMER_H__
#include <stdbool.h>
#include "main.h"

typedef void (*app_timer_cb)(void *context);
typedef enum
{
    STATUS_OK,
    STATUS_FAIL,
} app_timer_status;

typedef struct app_timer_inst_s *app_timer_inst_t;

app_timer_inst_t app_timer_create(uint32_t period_ms, bool autoRestart, app_timer_cb cb_func, void *cntx);
void app_timer_start(app_timer_inst_t inst);
void app_timer_stop(app_timer_inst_t inst);
void app_tmr_cb();

#endif //_APP_TIMER_H__
