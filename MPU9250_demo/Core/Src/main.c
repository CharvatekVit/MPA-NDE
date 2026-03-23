/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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
#include "driver_mpu9250_basic.h"

#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DISP_TIME 500
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE BEGIN 0 */
int _write(int file, char const *buf, int n)
{
	/* stdout redirection to UART2 */
	HAL_UART_Transmit(&huart2, (uint8_t*)(buf), n, HAL_MAX_DELAY);
	return n;
}
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
	MX_USART2_UART_Init();
	MX_SPI1_Init();
	/* USER CODE BEGIN 2 */

	printf("--- Start program NUCLEO-F070RB ---\r\n");
	printf("Run inicialization MPU9250...\r\n");

	/* * 1. INICIALIZATION
	 * For SPI interface set MPU9250_INTERFACE_SPI.
	 * Address only for I2C, library requires it.
	 */
	uint8_t res = mpu9250_basic_init(MPU9250_INTERFACE_SPI, 0); // MPU9250_ADDRESS_AD0_LOW
	if (res != 0)
	{
		printf("Error: MPU9250 inicialization failed!\r\n");
		// Optional Error Handler
	}
	else
	{
		printf("MPU9250 succesfully inicialized and prepered.\r\n");
	}
	printf("%x\n", res);

	/* Pause for stabilization */
	HAL_Delay(100);

	/* Measured data variables */
	float g[3];   // Accelerometes (Multiples of gravitational acceleration)
	float dps[3]; // Gyroscope (Degrees Per Second)
	float ut[3];  // Magnetometer (µT)

	/* Time marker */
	uint32_t tick = 0;

	/* --- Manual activation magnetometer bridge through SPI --- */
	printf("Activing I2C Master for automatic magnetometer reading...\r\n");

	uint8_t temp_reg = 0;

	/* 1. Enable I2C Master mode in register USER_CTRL (0x6A) */
	mpu9250_interface_spi_read(0x6A, &temp_reg, 1);
	temp_reg |= 0x20; /* Set bit 5 (I2C_MST_EN) */
	mpu9250_interface_spi_write(0x6A, &temp_reg, 1);

	/* 2. Set I2C Slave 0 address for reading from magnetometer */
	/* Address AK8963 is 0x0C, for reading must be set MSB (0x80) -> 0x8C */
	temp_reg = 0x8C;
	mpu9250_interface_spi_write(0x25, &temp_reg, 1); /* Register I2C_SLV0_ADDR */

	/* 3. Set initial magnetometer register */
	/* Data X axis, register 0x03 in AK8963 */
	temp_reg = 0x03;
	mpu9250_interface_spi_write(0x26, &temp_reg, 1); /* Register I2C_SLV0_REG */

	/* 4. Enable Slave 0 and set reading length to 7 byte */
	/* Bit 7 enables reading (0x80), plus 7 bytes (0x07) -> 0x87 */
	temp_reg = 0x87;
	mpu9250_interface_spi_write(0x27, &temp_reg, 1); /* Register I2C_SLV0_CTRL */

	printf("Bridge has been configurated! Start reading data...\r\n");
	HAL_Delay(100);
	/* -------------------------------------------------------- */

	/* --- TVRDÝ RESTART A PROBUZENÍ MAGNETOMETRU --- */
	printf("Zacinam tvrdou konfiguraci AK8963...\r\n");
	uint8_t reg_val;

	/* 1. ZAKÁZAT I2C BYPASS (Registr INT_PIN_CFG - 0x37) */
	/* Pokud je Bypass zapnutý, interní I2C Master nefunguje! */
	mpu9250_interface_spi_read(0x37, &reg_val, 1);
	reg_val &= ~0x02; // Vynulujeme bit 1 (BYPASS_EN)
	mpu9250_interface_spi_write(0x37, &reg_val, 1);

	/* 2. POVOLIT I2C MASTERA (Registr USER_CTRL - 0x6A) */
	mpu9250_interface_spi_read(0x6A, &reg_val, 1);
	reg_val |= 0x20; // Nastavíme bit 5 (I2C_MST_EN)
	mpu9250_interface_spi_write(0x6A, &reg_val, 1);

	/* 3. NASTAVIT RYCHLOST INTERNÍHO I2C (Registr I2C_MST_CTRL - 0x24) */
	reg_val = 0x0D; // I2C rychlost 400 kHz
	mpu9250_interface_spi_write(0x24, &reg_val, 1);

	/* --- ZÁPIS DO AK8963: PROBUĎ SE A MĚŘ! --- */
	/* K zápisu do magnetometru musíme použít kanál SLV4 */
	reg_val = 0x0C; // I2C adresa AK8963 (Zápis)
	mpu9250_interface_spi_write(0x31, &reg_val, 1); // I2C_SLV4_ADDR

	reg_val = 0x0A; // Registr CNTL1 v AK8963 (Nastavení módu)
	mpu9250_interface_spi_write(0x32, &reg_val, 1); // I2C_SLV4_REG

	reg_val = 0x16; // Hodnota: 16-bit rozlišení, Continuous Mode 2 (100 Hz)
	mpu9250_interface_spi_write(0x33, &reg_val, 1); // I2C_SLV4_DO

	reg_val = 0x80; // Povolit přenos (EN bit)
	mpu9250_interface_spi_write(0x34, &reg_val, 1); // I2C_SLV4_CTRL

	HAL_Delay(50); // Počkáme, až se příkaz pošle a magnetometr se probudí

	/* --- NASTAVENÍ KONTINUÁLNÍHO ČTENÍ DO EXT_SENS_DATA --- */
	/* Nastavíme kanál SLV0, aby neustále četl měření */
	reg_val = 0x8C; // I2C adresa 0x0C + bit čtení (0x80)
	mpu9250_interface_spi_write(0x25, &reg_val, 1); // I2C_SLV0_ADDR

	reg_val = 0x03; // Začít číst od registru HXL (osa X)
	mpu9250_interface_spi_write(0x26, &reg_val, 1); // I2C_SLV0_REG

	reg_val = 0x87; // Povolit čtení (EN) + délka 7 bajtů (X, Y, Z + ST2)
	mpu9250_interface_spi_write(0x27, &reg_val, 1); // I2C_SLV0_CTRL

	printf("Konfigurace dokoncena. Cekam na prvni data...\r\n");
	HAL_Delay(100);
	/* -------------------------------------------------------- */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		/* * 2 READING
		 */
		if (HAL_GetTick() > tick)
		{
			tick = HAL_GetTick() + DISP_TIME;

			//printf("Reading data from sensor...\r\n");
			if (mpu9250_basic_read(g, dps, ut) != 0)
			{
				printf("Error: Sensor data reading failed!\r\n");
			}
			else
			{
				/* 3. WRITE VALUE TO SERIAL */
				//printf("\r\n");
				//printf("A [g]  : X=%d, Y=%d, Z=%d\r\n", (int16_t)(g[0]*1000), (int16_t)(g[1]*1000), (int16_t)(g[2]*1000));
				//printf("G [dps]: X=%d, Y=%d, Z=%d\r\n", (int16_t)(dps[0]*1000), (int16_t)(dps[1]*1000), (int16_t)(dps[2]*1000));
				//printf("M [uT] : X=%d, Y=%d, Z=%d\r\n", (int16_t)(ut[0]*1000), (int16_t)(ut[1]*1000), (int16_t)(ut[2]*1000));
				//printf("\r\n");
			}

			uint8_t mag_raw_data[7];
			mpu9250_interface_spi_read(0x49, mag_raw_data, 7);

			//printf("Raw Mag Data: %02X %02X %02X %02X %02X %02X | ST2: %02X\r\n",
			//       mag_raw_data[0], mag_raw_data[1], mag_raw_data[2],
			//       mag_raw_data[3], mag_raw_data[4], mag_raw_data[5], mag_raw_data[6]);

			int16_t raw_x = ((int16_t)((mag_raw_data[1] << 8) | mag_raw_data[0]) * 15) / 100;
			int16_t raw_y = ((int16_t)((mag_raw_data[3] << 8) | mag_raw_data[2]) * 15) / 100;
			int16_t raw_z = ((int16_t)((mag_raw_data[5] << 8) | mag_raw_data[4]) * 15) / 100;

			printf("M [uT]: X=%d, Y=%d, Z=%d\r\n", raw_x, raw_y, raw_z);
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

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
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
	huart2.Init.BaudRate = 9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
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
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, SPI1_CS_Pin|LED_GREEN_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : SPI1_CS_Pin LED_GREEN_Pin */
	GPIO_InitStruct.Pin = SPI1_CS_Pin|LED_GREEN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
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
