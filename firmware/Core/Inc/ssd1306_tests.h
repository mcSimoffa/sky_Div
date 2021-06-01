#ifndef __SSD1306_TEST_H__
#define __SSD1306_TEST_H__

#include <stdbool.h>
#include "ssd1306.h"
typedef enum
{
  FIT_LEFT,
  FIT_RIGHT,
  BATTERY
} bar_type_t;

void print_high(float altitude);
void print_calState(bool state);
void print_tempr(float tempr);

void progress_bar(bar_type_t _type, uint8_t level);

#endif // __SSD1306_TEST_H__
