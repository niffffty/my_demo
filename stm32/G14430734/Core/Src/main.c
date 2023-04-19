/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "led.h"
#include "lcd.h"
#include "my_pwm.h"
#include "stdio.h"


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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int src_flag=0;	//屏幕标志位
char text[30];

extern int R;
extern int K;
extern int R_K;
extern int pinlv;
extern int change_num;
extern int my;
extern int led_flag;

int adc_value=0;	//adc采集值
int palse=0;	//占空比

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_TIM17_Init();
  MX_TIM2_Init();
  MX_TIM16_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
	led_Disp(0x00);
//	led_Disp(0x01);
	
	LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,400);
	
	if(src_flag==0){
		led_Disp(0x01);
	}
	HAL_ADC_Start(&hadc2);
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  adc_value=HAL_ADC_GetValue(&hadc2);
	
	if(src_flag==0){//数据界面
//		led_Disp(0x01);
		LCD_DisplayStringLine(Line0, (uint8_t *)"                    ");
		LCD_DisplayStringLine(Line1, (uint8_t *)"        DATA        ");
		LCD_DisplayStringLine(Line2, (uint8_t *)"                    ");
		if(pinlv==0){//低频
			LCD_DisplayStringLine(Line3, (uint8_t *)"     M=L            ");
		}else if(pinlv==1){//高频
			LCD_DisplayStringLine(Line3, (uint8_t *)"     M=H            ");
		}
		sprintf(text,"     P=%d%%         ",palse);
		LCD_DisplayStringLine(Line4, (uint8_t *)text);
		
		sprintf(text,"     V=%d         ",adc_value);
		LCD_DisplayStringLine(Line5, (uint8_t *)text);
		
		LCD_DisplayStringLine(Line6, (uint8_t *)"                    ");
		LCD_DisplayStringLine(Line7, (uint8_t *)"                    ");
		LCD_DisplayStringLine(Line8, (uint8_t *)"                    ");
		LCD_DisplayStringLine(Line9, (uint8_t *)"                    ");
	}else if(src_flag==1){
		LCD_DisplayStringLine(Line0, (uint8_t *)"                    ");
		LCD_DisplayStringLine(Line1, (uint8_t *)"        PARA        ");
		LCD_DisplayStringLine(Line2, (uint8_t *)"                    ");
		sprintf(text,"     R=%d            ",R);
		LCD_DisplayStringLine(Line3, (uint8_t *)text);
		sprintf(text,"     K=%d            ",K);
		LCD_DisplayStringLine(Line4, (uint8_t *)text);
		LCD_DisplayStringLine(Line5, (uint8_t *)"                    ");
		LCD_DisplayStringLine(Line6, (uint8_t *)"                    ");
		LCD_DisplayStringLine(Line7, (uint8_t *)"                    ");
		LCD_DisplayStringLine(Line8, (uint8_t *)"                    ");
		LCD_DisplayStringLine(Line9, (uint8_t *)"                    ");
	}else if(src_flag==2){
		LCD_DisplayStringLine(Line0, (uint8_t *)"                    ");
		LCD_DisplayStringLine(Line1, (uint8_t *)"        RECD        ");
		LCD_DisplayStringLine(Line2, (uint8_t *)"                    ");
		sprintf(text,"     N=%d           ",change_num);
		LCD_DisplayStringLine(Line3, (uint8_t *)text);
		LCD_DisplayStringLine(Line4, (uint8_t *)"     MH=110.8       ");
		LCD_DisplayStringLine(Line5, (uint8_t *)"     ML=38.2        ");
		LCD_DisplayStringLine(Line6, (uint8_t *)"                    ");
		LCD_DisplayStringLine(Line7, (uint8_t *)"                    ");
		LCD_DisplayStringLine(Line8, (uint8_t *)"                    ");
		LCD_DisplayStringLine(Line9, (uint8_t *)"                    ");
	}
	
	key_proc();
	pwm_proc();
	
	
	
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
