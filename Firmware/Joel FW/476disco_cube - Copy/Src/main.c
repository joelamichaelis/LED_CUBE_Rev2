
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
#include "usb_device.h"

/* USER CODE BEGIN Includes */
#define NUM_TLCS    12
#include "usbd_cdc_if.h"
#include <string.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim5;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM5_Init(void);
                                    
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                
                                

/* USER CODE BEGIN PFP */
void spi_convert(uint16_t *data16, uint8_t *data8);
void tlc_clear(uint16_t *data16);
void tlc_set(uint16_t *data16, int number, uint16_t brightness);
void tlc_update(uint16_t *data16, uint8_t *data8);
void intToStr(uint8_t *numVal, uint8_t *strVal);
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

//lyr global variable indicates active cube layer.
uint8_t lyr = 0;

// Data Arrays
uint16_t data16[16*NUM_TLCS];
uint8_t data8[24*NUM_TLCS];

// Data Pointers
uint8_t *data8Ptr = &data8[0];
uint16_t *data16Ptr = &data16[0];

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

		// Old mapping. May need Updating.	
	int red[64] = {155,146,177,186,180,189,183,192,159,150,129,138,132,141,135,144,114,123,117,126,120,109,103,112,162,171,165,174,168,105,99,108,34,43,37,46,40,13,7,16,82,91,85,94,88,9,3,12,50,59,53,62,56,20,26,17,66,75,69,78,72,24,30,21};
	int grn[64] = {148,154,145,178,187,181,190,184,152,158,149,130,139,133,142,136,121,115,124,118,127,101,110,104,169,163,172,166,175,97,106,100,41,35,44,38,47,5,14,8,89,83,92,86,95,1,10,4,57,51,60,54,63,28,19,25,73,67,76,70,79,32,23,29};
	int blu[64] = {156,147,153,185,179,188,182,191,160,151,157,137,131,140,134,143,113,122,116,125,119,128,102,111,161,170,164,173,167,176,98,107,33,42,36,45,39,48,6,15,81,90,84,93,87,96,2,11,49,58,52,61,55,64,27,18,65,74,68,77,71,80,31,22};
	
	//uint8_t myBuf[20] = "Hello World \r\n";
	
	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_SPI1_Init();
  MX_USB_DEVICE_Init();
  MX_TIM2_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
	
	//set indicates power off (pin is open drain); reset indicates power on (pin is grounded)
	// This pin referencing is slightly redundant. L8 needs to be reset since it's toggled during tim5ocstart (158)
	HAL_GPIO_WritePin(L8_EN_GPIO_Port, L8_EN_Pin, GPIO_PIN_RESET);			
	HAL_GPIO_WritePin(L7_EN_GPIO_Port, L7_EN_Pin, GPIO_PIN_SET);		
	HAL_GPIO_WritePin(L6_EN_GPIO_Port, L6_EN_Pin, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(L5_EN_GPIO_Port, L5_EN_Pin, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(L4_EN_GPIO_Port, L4_EN_Pin, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(L3_EN_GPIO_Port, L3_EN_Pin, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(L2_EN_GPIO_Port, L2_EN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(L1_EN_GPIO_Port, L1_EN_Pin, GPIO_PIN_SET);		

	
	HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_2);	//
	HAL_TIM_Base_Start_IT(&htim5);
	HAL_TIM_OC_Start(&htim5, TIM_CHANNEL_3);	//


	HAL_SPI_Init(&hspi1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		
		
	// Used for debugging purposes below.
	uint8_t k = 0;
	//uint8_t *kptr = &k;
	//uint8_t kstr[4];
	//uint8_t *kstrptr =&kstr[0];
		
	//Illuminates LEDs one at a time
	for (k = 0;k<16*NUM_TLCS;k++)
	{
	//remove later. Leave for now for example.
	//CDC_Transmit_FS(myBuf,strlen((char *)myBuf));	
	tlc_clear(data16Ptr);
	tlc_set(data16Ptr,k, 4095);
	tlc_update(data16Ptr, data8Ptr);	
		HAL_Delay(10);
		
	// Transmits and converts the current LED active over VCP!
	//intToStr(kptr,kstrptr);
	//CDC_Transmit_FS(kstrptr,strlen((char *)kstrptr));
	}
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV8;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV8;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the main internal regulator output voltage 
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/8000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
  sConfigOC.Pulse = 1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim2);

}

/* TIM5 init function */
static void MX_TIM5_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 0;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 16383;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_OC_Init(&htim5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  sConfigOC.Pulse = 16383;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim5);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, L3_EN_Pin|L5_EN_Pin|L4_EN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, L1_EN_Pin|L6_EN_Pin|L7_EN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(L2_EN_GPIO_Port, L2_EN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(XLAT_GPIO_Port, XLAT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(L8_EN_GPIO_Port, L8_EN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : L3_EN_Pin L5_EN_Pin L4_EN_Pin */
  GPIO_InitStruct.Pin = L3_EN_Pin|L5_EN_Pin|L4_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : L1_EN_Pin L6_EN_Pin L7_EN_Pin */
  GPIO_InitStruct.Pin = L1_EN_Pin|L6_EN_Pin|L7_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : L2_EN_Pin */
  GPIO_InitStruct.Pin = L2_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(L2_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : XLAT_Pin */
  GPIO_InitStruct.Pin = XLAT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(XLAT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : L8_EN_Pin */
  GPIO_InitStruct.Pin = L8_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(L8_EN_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void tlc_set(uint16_t *data16, int number, uint16_t brightness){
	
	*(data16 + number) = brightness;
	
}

void tlc_clear(uint16_t *data16){
	
		for(int i=0; i<24*NUM_TLCS;i++){
		*(data16Ptr + i) = 0;
	}
	
}

void tlc_update(uint16_t *data16, uint8_t *data8){
	
	spi_convert(data16Ptr, data8Ptr);
	
	HAL_SPI_Transmit(&hspi1, data8, 24*NUM_TLCS, 10);
	HAL_GPIO_WritePin(XLAT_GPIO_Port, XLAT_Pin, GPIO_PIN_SET);
	// short delay (approx 3us)
	for(uint16_t dummyVar =1;dummyVar<15;dummyVar++);
	HAL_GPIO_WritePin(XLAT_GPIO_Port, XLAT_Pin, GPIO_PIN_RESET);
	
}

void spi_convert(uint16_t *data16, uint8_t *data8){
	
	int index8 = 0;
	int index16 = 0;
	int i = 0;
	
 while (index8 < 24*NUM_TLCS){
 switch (i) {
      
      case 0:
				*(data8 + index8) = *(data16 + index16);
				index8++;
        i++;
        break;

      case 1: 
				*(data8 + index8) = ( (( (*(data16 + (index16 + 1))<<4) & 0xF0) >> 4) | ( ((*(data16 + index16)>>8) & 0x0F ) << 4));
				index8++;
        i++;
        break;

      case 2:
				*(data8 + index8) = (*(data16 + (index16 + 1))>>4);
				index8++;
				index16 = index16 + 2;
        i=0;
        break;}
			}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	
	if(lyr==0){ 
		HAL_GPIO_TogglePin(L8_EN_GPIO_Port, L8_EN_Pin);	
		HAL_GPIO_TogglePin(L1_EN_GPIO_Port, L1_EN_Pin);}
	
	if(lyr==1){ 
		HAL_GPIO_TogglePin(L1_EN_GPIO_Port, L1_EN_Pin);	
		HAL_GPIO_TogglePin(L2_EN_GPIO_Port, L2_EN_Pin);}
	
	if(lyr==2){ 
		HAL_GPIO_TogglePin(L2_EN_GPIO_Port, L2_EN_Pin);	
		HAL_GPIO_TogglePin(L3_EN_GPIO_Port, L3_EN_Pin);}
		
	if(lyr==3){ 
		HAL_GPIO_TogglePin(L3_EN_GPIO_Port, L3_EN_Pin);	
		HAL_GPIO_TogglePin(L4_EN_GPIO_Port, L4_EN_Pin);}
		
	if(lyr==4){ 
		HAL_GPIO_TogglePin(L4_EN_GPIO_Port, L4_EN_Pin);	
		HAL_GPIO_TogglePin(L5_EN_GPIO_Port, L5_EN_Pin);}

	if(lyr==5){ 
		HAL_GPIO_TogglePin(L5_EN_GPIO_Port, L5_EN_Pin);	
		HAL_GPIO_TogglePin(L6_EN_GPIO_Port, L6_EN_Pin);}	

	if(lyr==6){ 
		HAL_GPIO_TogglePin(L6_EN_GPIO_Port, L6_EN_Pin);	
		HAL_GPIO_TogglePin(L7_EN_GPIO_Port, L7_EN_Pin);}

	if(lyr==7){ 
		HAL_GPIO_TogglePin(L7_EN_GPIO_Port, L7_EN_Pin);	
		HAL_GPIO_TogglePin(L8_EN_GPIO_Port, L8_EN_Pin);}			
	
	lyr++;
	if(lyr==8) lyr = 0;
}

void CDC_ReceiveCallBack(uint8_t *buf, uint32_t len){
	CDC_Transmit_FS(buf, len);
}

//function defined specifically to convert integers 0~192 into strings
void intToStr(uint8_t *numVal, uint8_t *strVal)
{
	*(strVal) = (*(numVal)/100) + '0';
	*(strVal + 1) = ((*(numVal)%100)/10) + '0';
	*(strVal + 2) = (*(numVal)%10) + '0';
	*(strVal + 3) = '\n';
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
