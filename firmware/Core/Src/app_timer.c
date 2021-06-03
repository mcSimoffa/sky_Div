#include <stdlib.h>
#include <string.h>
#include "app_timer.h"


struct app_timer_inst_s
{
  uint32_t          period;
  uint32_t          counter;
  app_timer_inst_t  next_inst;
  app_timer_cb      cbf;
  void              *context;
  bool              active;
  bool              restart_mode;
};

app_timer_inst_t first_inst=NULL;
app_timer_inst_t last_inst=NULL;

//-------------------------------------------------------------
app_timer_inst_t app_timer_create(uint32_t period_ms, bool autoRestart, app_timer_cb cb_func, void *cntx)
{
  app_timer_inst_t inst=NULL;
  inst=malloc(sizeof(struct app_timer_inst_s));
  if (inst)
  {
    memset(inst, 0, sizeof(struct app_timer_inst_s));
    inst->period = period_ms;
    inst->cbf = cb_func;
    inst->context = cntx;
    inst->restart_mode = autoRestart;
    if(first_inst)
    {
      last_inst->next_inst=inst;
      last_inst=inst;
    }
    else
      last_inst=first_inst=inst;
  }
  return (inst);
}

//-------------------------------------------------------------
void app_timer_start(app_timer_inst_t inst)
{
  inst->counter = inst->period;
  inst->active = true;
}

//-------------------------------------------------------------
void app_timer_stop(app_timer_inst_t inst)
{
  inst->active = false;
}

//-------------------------------------------------------------
void app_tmr_cb()
{
  app_timer_inst_t act_inst;
  if(last_inst)
  {
    act_inst = first_inst;
    while (act_inst)
    {
      if (act_inst->active)
      {
        if((!act_inst->counter--) && (act_inst->cbf))
        {
          act_inst->cbf(act_inst->context);
          act_inst->counter = act_inst->period;
          if (!act_inst->restart_mode)
            act_inst->active = false;
        }    
      }
     act_inst = act_inst->next_inst;
    } 
  }
}