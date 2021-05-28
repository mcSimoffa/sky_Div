#include <string.h>
#include <stdio.h>
#include "ssd1306.h"
#include "ssd1306_tests.h"

void grid1(uint8_t blank)
{
for (uint8_t i=0; i < 127; i+=blank)
  ssd1306_Line(i, 0, i, 63, White);

for (uint8_t i=0; i < 63; i+=blank)
  ssd1306_Line(0, i, 127, i, White);
}

void print_high(uint16_t altitude) 
{
  char outbuf[4];
  if (altitude >= 10000)
    sprintf(outbuf,"%dk", altitude/1000);
  else if (altitude > 1000)
    sprintf(outbuf,"%dk%d", altitude/1000, (altitude%1000)/100);
  else if (altitude > 100)
    sprintf(outbuf,"%d", altitude);
  else if (altitude > 10)
    sprintf(outbuf,"%d ", altitude);
  else if (altitude > 1)
    sprintf(outbuf," %d ", altitude);
  else 
    sprintf(outbuf,"GND", altitude);

  ssd1306_SetCursor(54, 4);
  ssd1306_WriteString(outbuf, Font_6x8, White);
  ssd1306_UpdateScreen();
}

void print_calState(bool state)
{
  char outbuf[4];
  if (state)
    sprintf(outbuf,"CAL");
  else
    sprintf(outbuf,"NOT");

  ssd1306_SetCursor(54, 4);
  ssd1306_WriteString(outbuf, Font_6x8, White);
  ssd1306_UpdateScreen();
}

