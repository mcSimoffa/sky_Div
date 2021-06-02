#include <string.h>
#include <stdio.h>
#include "ssd1306.h"
#include "ssd1306_tests.h"

void print_high(float altitude) 
{
  char outbuf[10];
  if (altitude >= 10000)
    sprintf(outbuf,"%dk ", (int)(altitude/1000));
  else if (altitude > 1000)
    sprintf(outbuf,"%dk%d ",  (int)(altitude/1000), ((int)altitude%1000)/100);
  else if (altitude > 100)
    sprintf(outbuf,"%d ",  (int)altitude);
  else if (altitude > 10)
    sprintf(outbuf,"%d ",  (int)altitude);
  else if (altitude >= 1)
    sprintf(outbuf,"%.2f ", altitude);
  else 
    sprintf(outbuf,"GND    ", altitude);
 // ssd1306_SetCursor(36, 4);
 // ssd1306_WriteString(outbuf, Font_6x8, White);
  ssd1306_UpdateScreen();
}

void print_tempr(float tempr) 
{
  char outbuf[10];
   snprintf(outbuf, 9, "T=%.2f", tempr);
  // ssd1306_SetCursor(36, 12);
 // ssd1306_WriteString(outbuf, Font_6x8, White);
  ssd1306_UpdateScreen();
}

void print_calState(bool state)
{
  char outbuf[4];
  if (state)
    sprintf(outbuf,"CAL");
  else
    sprintf(outbuf,"NOT");
  ssd1306_Fill(Black);
  ssd1306_SetCursor(54, 4);
  ssd1306_WriteString(outbuf, Font_12x22, White);
  ssd1306_UpdateScreen();
}


