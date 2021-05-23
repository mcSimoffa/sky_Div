#include "stm32f1xx_hal.h"
#include "main.h"
#include "tv_composite.h"

#define PIXELS_IN_ROW   160
#define ROWS_IN_SCREEN  120
#define BYTES_IN_ROW    (PIXELS_IN_ROW/8)

//------------------------------------------------------------
extern SPI_HandleTypeDef hspi1;
//video-buffer [row][col]
uint8_t buf[ROWS_IN_SCREEN][BYTES_IN_ROW];

void new_row()
{
  if(HAL_SPI_Transmit_DMA  (&hspi1, (uint8_t*)buf, 1000) != HAL_OK)
    Error_Handler();
}