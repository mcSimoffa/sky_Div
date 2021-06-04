#ifndef __MENU_LIB_H__
#define __MENU_LIB_H__

#include <stddef.h>
#include "main.h"

typedef struct  menu_def_s * menu_def_t;

//-------------------------------------------------
menu_def_t menu_create(uint32_t items, uint32_t active);

//-------------------------------------------------------------
bool menu_add(menu_def_t instance, uint32_t num, char *text);

//-------------------------------------------------------------
uint32_t menu_list_down(menu_def_t instance);

//-------------------------------------------------------------
uint32_t menu_list_up(menu_def_t instance);

//-------------------------------------------------------------
uint32_t menu_get_active_elem(menu_def_t instance);

//-------------------------------------------------------------
char *menu_get_string(menu_def_t instance, int32_t elem_num);

#endif  //__MENU_LIB_H__