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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>		// For exit()
#include <math.h>		// For fabsf(float);
#include <stdbool.h>	// For bool activation_trigger, bool ml_collecting
#include <stdint.h>		// For uints etc.

#include "ads1299.h"
#include "NanoEdgeAI.h"
#include "knowledge.h"
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
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
// =======================
//        Defines
// =======================
#define ML_SAMPLE_COUNT         1024       // Number of samples for ML input
#define INTENSITY_BUFFER_SIZE   10         // Short buffer window for intensity comparison
#define INTENSITY_THRESHOLD     1.3f       // 50% increase threshold

#define ML_TRAINING_MODE		1


#if ML_TRAINING_MODE
#define GRAPH_MODE_ENABLED 0
#define CHUNK_MODE_ENABLED 1
#else
#define GRAPH_MODE_ENABLED 1
#define CHUNK_MODE_ENABLED 0
#endif


// =======================
//     Global Variables
// =======================

// --- ML buffer ---
volatile float ml_buffer[ML_SAMPLE_COUNT * 4];  // Interleaved EMG: CH1_0, CH2_0, ..., CH4_1023
volatile float output_class_buffer[CLASS_NUMBER];
volatile uint16_t ml_index = 0;
volatile bool ml_collecting = false;
volatile uint32_t ml_class = 0;                 // Last ML classification result (0 = none)

// --- Intensity detection ---
volatile float ch1_buffer[INTENSITY_BUFFER_SIZE] = {0};
volatile float ch2_buffer[INTENSITY_BUFFER_SIZE] = {0};
volatile float ch3_buffer[INTENSITY_BUFFER_SIZE] = {0};
volatile float ch4_buffer[INTENSITY_BUFFER_SIZE] = {0};

volatile float ch1_I1 = 0, ch1_I2 = 0;
volatile float ch2_I1 = 0, ch2_I2 = 0;
volatile float ch3_I1 = 0, ch3_I2 = 0;
volatile float ch4_I1 = 0, ch4_I2 = 0;

volatile uint16_t intensity_counter = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
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
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  /***** POWER SUPPLY SEQUENCING ACCORDING TO ADS1299 DATASHEET *****/
  // TODO: WHEN CUSTOM PCB -- Enable all voltage to ADS1299
  // TODO: WHEN CUSTOM PCB -- Delay by 500mS (ensure power lines stable)

  // Enable digital I/O to default state ADS1299
  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ADS1299_NRESET_GPIO_Port, ADS1299_NRESET_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ADS1299_NPWDN_GPIO_Port, ADS1299_NPWDN_Pin, GPIO_PIN_SET);

  // Initialize ADS1299
  ads1299_device_init(&hspi1, 1);
  HAL_Delay(1000);
  HAL_GPIO_WritePin(ADS1299_START_GPIO_Port, ADS1299_START_Pin, GPIO_PIN_SET);

  // Initialize ML
  enum neai_state error_code = neai_classification_init(knowledge);
  if (error_code != NEAI_OK) {
	exit(1);
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 921600;
  huart2.Init.WordLength = UART_WORDLENGTH_9B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_EVEN;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, ADS1299_NPWDN_Pin|ADS1299_NRESET_Pin|PMU_MODE_Pin|PMU_ILIM1_Pin
                          |PMU_ILIM0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ADS1299_START_GPIO_Port, ADS1299_START_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, PMU_EN1_Pin|PMU_EN2_Pin|PMU_EN3_Pin|PMU_EN4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE2 PE3 PE4 PE5
                           PE6 PE7 PE8 PE9
                           PE10 PE11 PE12 PE13
                           PE14 PE15 PE0 PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PC13 PC14 PC15 PC0
                           PC1 PC2 PC3 PC6
                           PC7 PC8 PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0
                          |GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PF0 PF1 PF2 PF3
                           PF4 PF5 PF6 PF7
                           PF8 PF9 PF10 PF11
                           PF12 PF13 PF14 PF15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PH0 PH1 PH3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA2 PA3 PA8
                           PA9 PA10 PA11 PA12
                           PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI1_CS_Pin */
  GPIO_InitStruct.Pin = SPI1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI1_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ADS1299_NDRDY_Pin */
  GPIO_InitStruct.Pin = ADS1299_NDRDY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADS1299_NDRDY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ADS1299_NPWDN_Pin ADS1299_NRESET_Pin PMU_MODE_Pin PMU_ILIM1_Pin
                           PMU_ILIM0_Pin */
  GPIO_InitStruct.Pin = ADS1299_NPWDN_Pin|ADS1299_NRESET_Pin|PMU_MODE_Pin|PMU_ILIM1_Pin
                          |PMU_ILIM0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : ADS1299_START_Pin */
  GPIO_InitStruct.Pin = ADS1299_START_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ADS1299_START_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB1 PB2 PB10 PB11
                           PB12 PB13 PB14 PB15
                           PB4 PB5 PB6 PB7
                           PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PG0 PG1 PG2 PG3
                           PG4 PG5 PG6 PG7
                           PG8 PG9 PG10 PG11
                           PG12 PG13 PG14 PG15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PD8 PD9 PD10 PD11
                           PD12 PD13 PD14 PD15
                           PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PMU_NFAULT_Pin */
  GPIO_InitStruct.Pin = PMU_NFAULT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PMU_NFAULT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PMU_EN1_Pin PMU_EN2_Pin PMU_EN3_Pin PMU_EN4_Pin */
  GPIO_InitStruct.Pin = PMU_EN1_Pin|PMU_EN2_Pin|PMU_EN3_Pin|PMU_EN4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
 * @brief When DRDY falling edge, transmit to read data
 * @param GPIO Pin that interrupt happened
 * @return None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  // If we see NDRDY, we begin reading data
  if(GPIO_Pin == ADS1299_NDRDY_Pin) {
    ads1299_read_data_IT(&hspi1);
  } else {
      __NOP();
  }
}

/**
 * @brief Call back that occurs when ADS is fully read from
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	// 1. De-assert chip select
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);

    // 2. Extract and check ADS1299 status bits
    uint32_t status = (RX_BUFF[0] << 16) | (RX_BUFF[1] << 8) | RX_BUFF[2];
    if (status != 0b110000000000000000000000) {
        uint8_t fail_msg[] = {'F', 'A', 'I', 'L'};
        HAL_UART_Transmit(&huart2, fail_msg, sizeof(fail_msg), HAL_MAX_DELAY);
        return;
    }

    // 3. Parse 4 EMG channels from ADS1299 (3 bytes per channel)
    float ch[4];
    for (int i = 0; i < 4; i++) {
        uint32_t raw = (RX_BUFF[3 + i * 3] << 16) |
                       (RX_BUFF[4 + i * 3] << 8) |
                       (RX_BUFF[5 + i * 3]);
        ch[i] = ads1299_data_to_float(raw);
    }

    // 4. Update intensity buffers (absolute value)
    ch1_buffer[intensity_counter] = fabsf(ch[0]);
    ch2_buffer[intensity_counter] = fabsf(ch[1]);
    ch3_buffer[intensity_counter] = fabsf(ch[2]);
    ch4_buffer[intensity_counter] = fabsf(ch[3]);
    intensity_counter++;

    bool activation_triggered = false;

    if (intensity_counter >= INTENSITY_BUFFER_SIZE) {
        // Shift intensities: I1 <- I2, I2 <- new average
        ch1_I1 = ch1_I2; ch2_I1 = ch2_I2; ch3_I1 = ch3_I2; ch4_I1 = ch4_I2;
        ch1_I2 = ch2_I2 = ch3_I2 = ch4_I2 = 0.0f;

        for (int i = 0; i < INTENSITY_BUFFER_SIZE; i++) {
            ch1_I2 += ch1_buffer[i];
            ch2_I2 += ch2_buffer[i];
            ch3_I2 += ch3_buffer[i];
            ch4_I2 += ch4_buffer[i];
        }

        ch1_I2 /= INTENSITY_BUFFER_SIZE;
        ch2_I2 /= INTENSITY_BUFFER_SIZE;
        ch3_I2 /= INTENSITY_BUFFER_SIZE;
        ch4_I2 /= INTENSITY_BUFFER_SIZE;

        // Compare for significant intensity increase
        if ((ch1_I2 >= INTENSITY_THRESHOLD * ch1_I1) ||
            (ch2_I2 >= INTENSITY_THRESHOLD * ch2_I1) ||
            (ch3_I2 >= INTENSITY_THRESHOLD * ch3_I1) ||
            (ch4_I2 >= INTENSITY_THRESHOLD * ch4_I1)) {
            activation_triggered = true;
        }

        intensity_counter = 0;
    }

    // 5. Handle ML buffer activation
    if (activation_triggered && !ml_collecting) {
        ml_collecting = true;
        ml_index = 0;
    }

    // 6. Collect into ML buffer
    if (ml_collecting) {
        ml_buffer[ml_index++] = ch[0];
        ml_buffer[ml_index++] = ch[1];
        ml_buffer[ml_index++] = ch[2];
        ml_buffer[ml_index++] = ch[3];

        if (ml_index >= ML_SAMPLE_COUNT * 4) {
            // Run classification
        	__disable_irq();
            uint16_t class_id = 0;
            neai_classification((float *)ml_buffer, (float *)output_class_buffer, &class_id);
            ml_class = (uint32_t)class_id;
            __enable_irq();

#if CHUNK_MODE_ENABLED
            // Transmit full buffer + classification result
            uint32_t header = 0x53594E43;  // "SYNC"
            HAL_UART_Transmit(&huart2, (uint8_t*)&header, sizeof(header), HAL_MAX_DELAY);
            HAL_UART_Transmit(&huart2, (uint8_t*)ml_buffer, sizeof(float) * ML_SAMPLE_COUNT * 4, HAL_MAX_DELAY);
            HAL_UART_Transmit(&huart2, (uint8_t*)&ml_class, sizeof(ml_class), HAL_MAX_DELAY);
#endif

            ml_index = 0;
            ml_collecting = false;
        }
    }

#if GRAPH_MODE_ENABLED
    // 7. Transmit single sample for live plotting
    float tx_buffer[6];
    tx_buffer[0] = *(float*)&(uint32_t){0x53594E43};  // "SYNC" as float
    tx_buffer[1] = ch[0];
    tx_buffer[2] = ch[1];
    tx_buffer[3] = ch[2];
    tx_buffer[4] = ch[3];
    tx_buffer[5] = *(float*)&ml_class;

    HAL_UART_Transmit(&huart2, (uint8_t*)tx_buffer, sizeof(tx_buffer), HAL_MAX_DELAY);

    // Reset classification until next result
    ml_class = 15;
#endif
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
