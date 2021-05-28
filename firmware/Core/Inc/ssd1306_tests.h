#ifndef __SSD1306_TEST_H__
#define __SSD1306_TEST_H__
#include <stdbool.h>
#include "ssd1306.h"

void print_high(uint16_t altitude);
void print_calState(bool state);
void grid1(uint8_t blank);

#endif // __SSD1306_TEST_H__
