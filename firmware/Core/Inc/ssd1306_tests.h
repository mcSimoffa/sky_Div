#ifndef __SSD1306_TEST_H__
#define __SSD1306_TEST_H__

#include <stdbool.h>
#include "ssd1306.h"
typedef enum
{
  FIT_LEFT,
  FIT_RIGHT
} bar_type_t;

void altitude_display(int16_t altitude);
void tempr_display(int8_t tempr);

void progress_bar(bar_type_t _type, uint8_t level);

void battery_bar(int8_t level);
/*---------------------------------------------
function for display time like HH:MM
it should launck once per second for bliking ':'
correctly
parameter hours and mins in RTC_FORMAT_BIN
---------------------------------------------- */
void time_display(uint8_t _hour, uint8_t _min);
#endif // __SSD1306_TEST_H__
