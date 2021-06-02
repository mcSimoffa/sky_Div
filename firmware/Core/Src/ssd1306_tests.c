#include <string.h>
#include <stdio.h>
#include "ssd1306.h"
#include "ssd1306_tests.h"
//---------------------------------------------------------------
void altitude_display(int16_t altitude)
{
#define alt_start_col 9
#define alt_start_row 10
  char outbuf[10];
  sprintf(outbuf,"%04d", altitude);
  ssd1306_SetCursor(alt_start_col, alt_start_row);
  ssd1306_WriteString(outbuf, Font_12x22, White);
}

//---------------------------------------------------------------
void tempr_display(int8_t tempr) 
{
#define tmpr_start_col 33
#define tmpr_start_row 0
  char outstr[8];
  snprintf(outstr,6,"%d~C", tempr);
  ssd1306_SetCursor(tmpr_start_col, tmpr_start_row);
  ssd1306_WriteString(outstr, Font_6x8, White);
}

//---------------------------------------------------------------
void time_display(uint8_t _hour, uint8_t _min)
{
#define time_start_col 2
#define time_start_row 0
  static bool blink = false;
  char blink_char = blink ? ':':' ';
  char outstr[8];
  snprintf(outstr,6,"%02d%c%02d", _hour, blink_char, _min);
  blink = !blink;
  ssd1306_SetCursor(time_start_col, time_start_row);
  ssd1306_WriteString(outstr, Font_6x8, White);
}

//---------------------------------------------------------------
void progress_bar(bar_type_t _type, uint8_t level)
{
#define ROW_START     39
#define BAR_WIDTH     7
#define BAR_THICKNESS 3
#define BAR_SPACE     1
#define BAR_DIRECTION -1 // 1=Down? -1=up

  uint8_t col_start, col_end;
  if (_type == FIT_LEFT)
  {
    col_start = 0;
    col_end = BAR_WIDTH - 1;
  }
  else
  {
    col_start = SSD1306_WIDTH - BAR_WIDTH - 1;
    col_end = SSD1306_WIDTH - 1;
  }
  
  for (uint8_t i=0; i<level*(BAR_THICKNESS + BAR_SPACE); i++)
  {
    if (i%(BAR_THICKNESS + BAR_SPACE)< BAR_THICKNESS)
      ssd1306_Line(col_start, ROW_START+BAR_DIRECTION*i, col_end, ROW_START+BAR_DIRECTION*i, White);
    else
      ssd1306_Line(col_start, ROW_START+BAR_DIRECTION*i, col_end, ROW_START+BAR_DIRECTION*i, Black);
  }

}

//---------------------------------------------------------------
//level -1 -0 bar and blinking
//level 0 - 0 bar
//level 1 - 1 bar
//level 2 - 2 bar
//level 3 - 3 bar
void battery_bar(int8_t level)
{
#define COL_START         70
#define BAT_ROW_START     0
#define BAT_BAR_HIGH      5
#define BAT_BAR_THICKNESS 3
#define BAT_BAR_SPACE     1

#define BAT_ROW_END  (BAT_ROW_START + BAT_BAR_HIGH - 1)
  for (uint8_t i=0; i<level*(BAR_THICKNESS + BAR_SPACE); i++)
  {
    if (i%(BAR_THICKNESS + BAR_SPACE)< BAR_THICKNESS)
      ssd1306_Line(COL_START-i ,BAT_ROW_START, COL_START-i, BAT_ROW_END, White);
    else
      ssd1306_Line(COL_START-i ,BAT_ROW_START, COL_START-i, BAT_ROW_END, Black);
  }
}