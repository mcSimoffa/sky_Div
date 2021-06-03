#include <string.h>
#include <stdio.h>
#include "ssd1306.h"
#include "ssd1306_tests.h"
//---------------------------------------------------------------
void altitude_display(int16_t altitude, bool unitKilometrs)
{
#define ALT_START_ROW 15
  char outbuf[8];
  if (unitKilometrs)
    sprintf(outbuf,"%.1f", (float)altitude/1000);
  else
    sprintf(outbuf,"%04d", altitude);

  // centering
  uint8_t  pexel_len=0;
  for (uint8_t i=0; outbuf[i]; i++)
    pexel_len += Font_12x22.withtable[outbuf[i]-Font_12x22.min_code];
  uint8_t col_start = (SSD1306_WIDTH-pexel_len)/2;

  ssd1306_SetCursor(col_start, ALT_START_ROW);
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

//----------------------------------------------------------------
void battery_bar(uint8_t level, bool isFlash)
{
//define to customizing
#define COL_START         70
#define BAT_ROW_START     0
#define BAT_BAR_HIGH      6
#define BAT_BAR_THICKNESS 3
#define BAT_BAR_SPACE     1
#define MAX_LEVEL         3
#define BOUNDARY_FLASH_FREQUENCY_DIVIDER         4

#define BAT_ROW_END  (BAT_ROW_START + BAT_BAR_HIGH - 1)
  if (level <= MAX_LEVEL)
  {
    uint8_t col_end;
    for (uint8_t i=0; i<level*(BAR_THICKNESS + BAR_SPACE); i++)
    {
      if (i%(BAR_THICKNESS + BAR_SPACE)< BAR_THICKNESS)
        ssd1306_Line(COL_START-i ,BAT_ROW_START, COL_START-i, BAT_ROW_END, White);
      else
        ssd1306_Line(COL_START-i ,BAT_ROW_START, COL_START-i, BAT_ROW_END, Black);
    }
  
    //boundary of battery drawing
    col_end = COL_START-MAX_LEVEL*(BAT_BAR_THICKNESS+BAT_BAR_SPACE)+BAT_BAR_SPACE;
    static uint8_t boundary_flashing = BOUNDARY_FLASH_FREQUENCY_DIVIDER;
    static SSD1306_COLOR boundary_color = White;
    if (isFlash)
    {
      if  (--boundary_flashing == 0)
      {
        boundary_color = (boundary_color==Black)? White:Black;
        boundary_flashing = BOUNDARY_FLASH_FREQUENCY_DIVIDER;
      }
    }
    else 
      boundary_color = White;
    SSD1306_VERTEX battery_bound[9]=
    {
      {.x=COL_START,  .y=BAT_ROW_START},
      {.x=col_end,    .y=BAT_ROW_START},
      {.x=col_end,    .y=BAT_ROW_START+1},
      {.x=col_end-1,  .y=BAT_ROW_START+1},
      {.x=col_end-1,  .y=BAT_ROW_END-1},
      {.x=col_end,    .y=BAT_ROW_END-1},
      {.x=col_end,    .y=BAT_ROW_END},
      {.x=COL_START,  .y=BAT_ROW_END},
      {.x=COL_START,  .y=BAT_ROW_START}
    };
    ssd1306_Polyline(&battery_bound[0], sizeof(battery_bound)/sizeof(SSD1306_VERTEX), boundary_color);
  }
}