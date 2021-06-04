#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "menu_lib.h"
struct menu_def_s
{
  uint32_t  items;
  uint32_t  active_item;
  char      *txtLabel[1];
};

//-------------------------------------------------
menu_def_t menu_create(uint32_t items, uint32_t active)
{
  menu_def_t  ret_p = NULL;
  uint32_t inst_size = sizeof(struct menu_def_s)+sizeof(char*)*(items-1);
  ret_p = malloc(inst_size);
  if (ret_p)
  {
    memset(ret_p, 0, inst_size);
    ret_p->items = items;
    if(active<items)
      ret_p->active_item = active;
  }
  return (ret_p);
}

//-------------------------------------------------------------
bool menu_add(menu_def_t instance, uint32_t num, char *text)
{
  bool retval = false;
  if (num < instance->items)
  {
    instance->txtLabel[num]=text;
    retval = true;
  }
  return(retval);
}

//-------------------------------------------------------------
uint32_t menu_list_down(menu_def_t instance)
{
  if(instance->active_item + 1 < instance->items)
    (instance->active_item)++;
  return (instance->active_item);
}

//-------------------------------------------------------------
uint32_t menu_list_up(menu_def_t instance)
{
  if(instance->active_item > 0)
    (instance->active_item)--;
  return (instance->active_item);
}

//-------------------------------------------------------------
uint32_t menu_get_active_elem(menu_def_t instance)
{
  return (instance->active_item);
}

//-------------------------------------------------------------
char *menu_get_string(menu_def_t instance, int32_t elem_num)
{
  char *p_str = NULL;
  if((elem_num<instance->items) && (elem_num >= 0))
    p_str = instance->txtLabel[elem_num];
  
  return (p_str);
}