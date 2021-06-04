/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "stm32l4xx_it.h"
#include "app_timer.h"
#include "oring_buf.h"
#include "menu_lib.h"
#include "ssd1306_tests.h"
#include "BMPXX80.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */
bool update_flag = false;
uint8_t _hour, _minute, new_minute, new_hour;
menu_def_t main_mnu_h, units_mnu_h, time_mnu_h, min_mnu_h;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
static void create_main_menu();
static void create_units_menu();
static void create_time_menu();
static void create_minute_menu();


static void handle_main_menu();
static void handle_units_menu();
static void handle_time_menu();
static void handle_minute_menu();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  float pressure_groung = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  HAL_RTCEx_EnableBypassShadow(&hrtc);
  button_timers_init();
  float null_level_pa =101000;
#ifdef BME280
  BME280_Init(&hi2c2, BME280_TEMPERATURE_16BIT, BME280_PRESSURE_ULTRALOWPOWER, BME280_HUMINIDITY_STANDARD, BME280_NORMALMODE );
  BME280_SetConfig(BME280_STANDBY_MS_10, BME280_FILTER_X8 );
#endif
  ssd1306_Init();
  ssd1306_Fill(Black);
  create_main_menu();
  create_units_menu();
  create_time_menu();
  create_minute_menu();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //float temp_grad = 0;
  while (1)
  {
    while (!update_flag)
    {
    //sleep
    }
    update_flag = false;
    uint32_t got_event;
    if(oring_buf_get(&got_event))
    {
      switch (got_event)
      {
        case EVENT_CENTER_PRESSED:
          handle_main_menu();
          break;

        case EVENT_UP_PRESSED:
          printf("Up pressed\r\n");
          break;

        case EVENT_DOWN_PRESSED:
          printf("Down pressed\r\n");
          break;
      }

      printf("event %d\r\n", got_event);
    }
    float _tmpr;
    int32_t _pressure;
    float _humin;
    BME280_ReadTemperatureAndPressureAndHuminidity(&_tmpr, &_pressure, &_humin);
    float altitude = 44330*(1.0-pow(_pressure/null_level_pa, 0.1903));
    ssd1306_Fill(Black);
    tempr_display((uint8_t)_tmpr);
    time_display(_hour, _minute);
    altitude_display((int16_t)altitude, true);
    static uint8_t k=0;
    progress_bar(FIT_LEFT, k);
    progress_bar(FIT_RIGHT, k);
    if(++k>8)
      k=0;

    static uint8_t bl=0;
    battery_bar(bl, false);
    if(++bl>3)
      bl=0;

    ssd1306_UpdateScreen();
    printf("\ralttitude = %.2f m. Temperature = %.2f grad C Time %d:%d",  altitude,_tmpr,_hour,_minute);    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_I2C2;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00300B29;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /** I2C Fast mode Plus enable
  */
  HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x10707DBC;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BLUE_BUTTON_Pin */
  GPIO_InitStruct.Pin = BLUE_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BLUE_BUTTON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BUTTON_UP_Pin BUTTON_DOWN_Pin BUTTON_CENTER_Pin */
  GPIO_InitStruct.Pin = BUTTON_UP_Pin|BUTTON_DOWN_Pin|BUTTON_CENTER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LED2_Pin */
  GPIO_InitStruct.Pin = LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

}

/* USER CODE BEGIN 4 */
void pass_to_main_context(uint8_t hour, uint8_t min)
{
  _hour=hour;
  _minute=min;
  update_flag = true;
}

//------------------------------------------------
static void create_main_menu()
{
  main_mnu_h = menu_create(8,0);
  menu_add(main_mnu_h, 0, "units", handle_units_menu);
  menu_add(main_mnu_h, 1, "freefall mc", NULL);
  menu_add(main_mnu_h, 2, "deployment he", NULL);
  menu_add(main_mnu_h, 3, "contrast", NULL);
  menu_add(main_mnu_h, 4, "shift_texts", NULL);
  menu_add(main_mnu_h, 5, "recorder jump", NULL);
  menu_add(main_mnu_h, 6, "language", NULL);
  menu_add(main_mnu_h, 7, "<-", NULL);
}

//------------------------------------------------
static void create_units_menu()
{
  units_mnu_h = menu_create(5,0);
  menu_add(units_mnu_h, 0, "year", NULL);
  menu_add(units_mnu_h, 1, "month", NULL);
  menu_add(units_mnu_h, 2, "time", handle_time_menu);
  menu_add(units_mnu_h, 3, "serial", NULL);
  menu_add(units_mnu_h, 4, "<<-", NULL);
}

//------------------------------------------------
static void create_time_menu()
{
  static char labels_hours[24][4];
  time_mnu_h = menu_create(24, _hour);
  for(uint8_t i=0; i<24; i++)
  {
    snprintf(&labels_hours[i][0], 4 ,"%d", i);
    menu_add(time_mnu_h, i, &labels_hours[i][0], handle_minute_menu);
  }
}

//------------------------------------------------
static void create_minute_menu()
{
  static char labels_mins[60][4];
  min_mnu_h = menu_create(60, _minute);
  for(uint8_t i=0; i<60; i++)
  {
    snprintf(&labels_mins[i][0], 4 ,"%d", i);
    menu_add(min_mnu_h, i, &labels_mins[i][0], NULL);
  }
}

//-------------------------------------------
static uint32_t handle_usual_menu(menu_def_t menu_inst, char *logo)
{
#define SET_SHIFT_TEXT 4 //in pixels
  SSD1306_COLOR color;
  FontDef  font = Font_6x8;
  uint32_t got_event, activ_e;
  char  text[16];
  bool mnu_leave_flag = false;
  do
  {
    // out to display a text of menu
    ssd1306_Fill(Black);
    activ_e = menu_get_active_elem(menu_inst);
    uint8_t n=0;
    uint8_t col= SET_SHIFT_TEXT * 2;
    int8_t  shift_text = -SET_SHIFT_TEXT;
    for(int32_t i=(int32_t)activ_e-2; i<(int32_t)activ_e+3; i++)
    {
      uint8_t row = font.FontHeight*n++;
      if (i==activ_e)
      {
        color = Black;
        shift_text *= -1;
      }
      else
       color = White;
 
      ssd1306_SetCursor(col, row);
      col += shift_text;
      ssd1306_WriteString(menu_get_string(menu_inst, i), font, color);
      if ((logo) && (i==activ_e))
      ssd1306_WriteString(logo, font, White);
    }
    ssd1306_UpdateScreen();
  
    // wait keypress
    while(!oring_buf_get(&got_event)) //wait any key press
    {
      //TODO should sleep
    }
    switch (got_event)
    {
      case EVENT_CENTER_PRESSED:
        mnu_leave_flag = true;
        break;

      case EVENT_UP_PRESSED:
        menu_list_up(menu_inst);
        break;

      case EVENT_DOWN_PRESSED:
        menu_list_down(menu_inst);
        break;
    }
  } while (!mnu_leave_flag);
  
  // exit to next menu for example
  return (activ_e);
}

//-------------------------------------------
static void handle_main_menu()
{
  do
  {
    printf("\r\nenter to 'main' menu");
    if (handle_usual_menu(main_mnu_h, NULL) != 7)
      menu_enter_to_submenu(main_mnu_h);
    else
      break;
  } while(1);
}

//-------------------------------------------
static void handle_units_menu()
{
  do
  {
    printf("\r\nenter to 'units' menu");
    if (handle_usual_menu(units_mnu_h, NULL) != 4)
      menu_enter_to_submenu(units_mnu_h);
    else
      break;
  } while(1);
}

//-------------------------------------------
static void handle_time_menu()
{
  printf("\r\nenter to 'time-hours' menu");
  menu_set_active_elem(time_mnu_h, _hour);
  RTC_TimeTypeDef   new_time;
  new_time.Hours = handle_usual_menu(time_mnu_h, "   HR");
  menu_enter_to_submenu(time_mnu_h);
  new_time.Minutes = new_minute;
  new_time.Seconds = 0;
  new_time.TimeFormat = RTC_FORMAT_BIN;
  HAL_RTC_SetTime(&hrtc, &new_time, RTC_FORMAT_BIN);
}

//-------------------------------------------
static void handle_minute_menu()
{
  printf("\r\nenter to 'time-minutes' menu");
  menu_set_active_elem(min_mnu_h, _minute);
  new_minute = handle_usual_menu(min_mnu_h, "   MIN");
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
