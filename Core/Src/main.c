/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "buzzer.h"
#include "driver_oled.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

struct soft_timer{
	uint32_t timeout;
	void *args;
	void (*func)(void *);
};

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern uint8_t songname[3][10][16];
extern uint8_t songcount[3];
extern uint8_t icon[2][16];
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

void keytimeroutfun(void *args);
uint8_t key_state = 0; // 0=释放, 1=按下
uint32_t key_press_start = 0;

struct soft_timer keytimer={~0,NULL,keytimeroutfun};
char* way[2]={"Play in order","Repeat one song"};
uint8_t iscycle=0;//是否是循环播放模式
int songnum=0;//播放乐曲的编号

//计时器时间到了后触发
void keytimeroutfun(void *args)
{
	keytimer.timeout=~0;//把定时器的时间设置为无限大，防止一直累加
	
	if(key_state == 0) // 如果当前是释放状态，说明是按键释放事件
  {
    uint32_t press_duration = HAL_GetTick() - key_press_start;
    
    if(press_duration > 1000) // 长按超过2秒
    {
        iscycle = !iscycle;
        OLED_PrintString(0,0,way[iscycle]);
    }
    else if(press_duration > 50) // 短按，大于50ms避免误触
    {
      // 切换下一曲
      isrnext=1;
			if(iscycle)
			{
				songnum++;
				songnum%=3;
			}
    }
  }

}
//修改超时时间
void mod_timer(struct soft_timer *ptimer,uint32_t timeout)
{
	ptimer->timeout=HAL_GetTick()+timeout;
}
//检查是否超时
void checktimer(void)
{
	if(HAL_GetTick()>=keytimer.timeout)
	{
		//时间到了
		keytimer.func(keytimer.args);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==GPIO_PIN_14)
	{
    // 记录当前按键状态和时间
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)==GPIO_PIN_RESET) // 按下
    {
      key_state = 1;
      key_press_start = HAL_GetTick();
    }
    else // 释放
    {
      key_state = 0;
    }
		mod_timer(&keytimer,30);//延时一下再把按键次数加一，消除按键抖动
	}
	else if (GPIO_Pin==GPIO_PIN_11)
	{
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==GPIO_PIN_SET)//黑暗状态
			speed=2;
		else
			speed=1;
	}
}

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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	OLED_Init();
	OLED_Clear();
	OLED_PrintString(0,0,way[iscycle]);
	OLED_PrintString(0,5,"NO.");
	OLED_PrintElse(2,3,icon,1);
  while (1)
  {
		OLED_PrintSignedVal(3,5,songnum);
		OLED_ClearLine(5,5);
		OLED_PrintElse(5,5,songname[songnum],songcount[songnum]);
		MusicStart(songnum);
		if(iscycle==0)
		{
			songnum++;
			songnum%=3;
		}
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
#ifdef USE_FULL_ASSERT
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
