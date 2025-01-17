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
#include "memorymap.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdbool.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
unsigned int numero_cicli;
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

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

//Queste sono variabili che useremo
//Abituiamoci ad inserire commenti
	unsigned int cc=2000;
	unsigned int attesa= 1;
  unsigned int il_clock_impostato,  ripetizioni;
  char led_acceso=1;
  cc = 0xff;


//Un indirizzo che possiamo usare per un attimo: 0x2407fff8



//Cominciamo ad abituarci ad evitare l'uso dei numeri, quando . Usiamo invece definizioni o
//dichiarazioni di variabili e/o costanti
	#define interruttore_lampadina  GPIOB->BSRR
	#define accendi_lampadina (1<<0)
	#define spegni_lampadina (1<<(0+16))

//Useremo tra poco per misurare il tempo
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk ; //Abilitiamo il conta cicli macchina
	DBGMCU->APB1LFZ1 |= DBGMCU_APB1LFZ1_DBG_TIM6_Msk; //Stoppiamo TIM6 al breakpoint
	DWT->CYCCNT=0;  //Azzeriamo il contacicli macchina per comodità

	il_clock_impostato = HAL_RCC_GetSysClockFreq ()  ;
	ripetizioni = il_clock_impostato / 12;
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
  MX_USART3_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  cc &= 1<<3;
  USART3->CR1 |=USART_CR1_TCIE;

  TIM6 -> CNT = 0;
  TIM6 -> CR1 |= TIM_CR1_CEN;

  //CR  -> Control register
  //CEN -> Control Enable
  //CNT -> Counter

  /* USER CODE END 2 */


  //set scaler ( il counter conta CLOCK/SCALER)
  TIM6->PSC = 1000;

  //set the end timer
  TIM6->ARR = 60000;
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  TIM6->CR1 = TIM_CR1_CEN;
  while (1)
  {
	  while( TIM6->SR )
	  {
		  // GPIOB = led
		  // BSRR = registro per attivarlo
		  // BR0 reset , BS0 turn on
		  TIM6->SR = 0;
		  if (GPIOB->IDR&1<<0){
			  GPIOB ->BSRR |= GPIO_BSRR_BR0;
		  }
		  else{
			  GPIOB ->BSRR |= GPIO_BSRR_BS0;
		  }
	  }
  }
//  {
//    /* USER CODE END WHILE */
//	  if( DWT->CYCCNT > il_clock_impostato && ~(GPIOB->IDR&1<<0)){
//		  DWT->CYCCNT = 0;
//		  GPIOB ->BSRR |= GPIO_BSRR_BS0;
//	  }
//	  if( DWT->CYCCNT > il_clock_impostato && (GPIOB->IDR&1<<0)){
//	  		DWT->CYCCNT = 0;
//	  		GPIOB ->BSRR |= GPIO_BSRR_BR0;
//	  	  }
//
//    /* USER CODE BEGIN 3 */
//  }
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
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
