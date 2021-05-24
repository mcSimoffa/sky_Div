#include "stm32f1xx_hal.h"
#include "main.h"
#include "tv_composite.h"
#include <string.h>

#define DISPLAY_RESOLUTION_HOR  640
#define DISPLAY_RESOLUTION_VER  480
#define RESOLUTION_DIVIDER      2
#define BYTES_IN_ROW            (DISPLAY_RESOLUTION_HOR/RESOLUTION_DIVIDER/8)

//------------------------------------------------------------
extern SPI_HandleTypeDef hspi1;
//video-buffer [row][col]
uint8_t buf[DISPLAY_RESOLUTION_VER/RESOLUTION_DIVIDER][BYTES_IN_ROW];

void write_in_buf()
{
  memset(buf,0,sizeof(buf));
  memset(buf+100,255,2000);
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  static uint16_t n_row = 0;
  if (++n_row >= DISPLAY_RESOLUTION_VER)
  n_row = 0;

  uint16_t row_index = n_row/RESOLUTION_DIVIDER;
  HAL_StatusTypeDef res = HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)&buf[row_index][0], BYTES_IN_ROW);
  if(res != HAL_OK)
    Error_Handler();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  asm("nop");
}
