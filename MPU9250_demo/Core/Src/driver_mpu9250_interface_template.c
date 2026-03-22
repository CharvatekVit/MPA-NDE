/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_mpu9250_interface_template.c
 * @brief     driver mpu9250 interface template source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-08-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/08/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

/* Includes */
#include "driver_mpu9250_interface.h"

#include "stm32g0xx_hal.h"
#include "main.h"
#include <stdio.h>
#include <stdarg.h>

/* Defines */
#define MPU9250_CS_LOW()  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, 0)
#define MPU9250_CS_HIGH() HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, 1)

/* Extern global variables */
extern SPI_HandleTypeDef hspi1;

/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t mpu9250_interface_iic_init(void)
{
	return 0;
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t mpu9250_interface_iic_deinit(void)
{
	return 0;
}

/**
 * @brief      interface iic bus read
 * @param[in]  addr iic device write address
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t mpu9250_interface_iic_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
	return 0;
}

/**
 * @brief     interface iic bus write
 * @param[in] addr iic device write address
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t mpu9250_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
	return 0;
}

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t mpu9250_interface_spi_init(void)
{
	/* SPI is inicialized in main.c. Here only setting CS pin. */
	MPU9250_CS_HIGH();
	return 0;
}

/**
 * @brief  interface spi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 spi deinit failed
 * @note   none
 */
uint8_t mpu9250_interface_spi_deinit(void)
{   
	return 0;
}

/**
 * @brief      interface spi bus read
 * @param[in]  reg register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t mpu9250_interface_spi_read(uint8_t reg, uint8_t *buf, uint16_t len)
{
	/* MSB is set because I want to read */
	uint8_t tx_reg = reg | 0x80;

	MPU9250_CS_LOW();

	/* Send request */
	if (HAL_SPI_Transmit(&hspi1, &tx_reg, 1, 1000) != HAL_OK)
	{
		MPU9250_CS_HIGH();
		return 1;
	}

	/*  Read data */
	if (HAL_SPI_Receive(&hspi1, buf, len, 1000) != HAL_OK)
	{
		MPU9250_CS_HIGH();
		return 1;
	}

	MPU9250_CS_HIGH();
	return 0;
}

/**
 * @brief     interface spi bus write
 * @param[in] reg register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t mpu9250_interface_spi_write(uint8_t reg, uint8_t *buf, uint16_t len)
{
	/* MSB is clear because I want to write */
	    uint8_t tx_reg = reg & 0x7F;

	    MPU9250_CS_LOW();

	    /* Send address selected register */
	    if (HAL_SPI_Transmit(&hspi1, &tx_reg, 1, 1000) != HAL_OK)
	    {
	        MPU9250_CS_HIGH();
	        return 1;
	    }

	    /* Input data to selected register */
	    if (HAL_SPI_Transmit(&hspi1, buf, len, 1000) != HAL_OK)
	    {
	        MPU9250_CS_HIGH();
	        return 1;
	    }

	    MPU9250_CS_HIGH();
	    return 0;
}

/**
 * @brief     interface delay ms
 * @param[in] ms time
 * @note      none
 */
void mpu9250_interface_delay_ms(uint32_t ms)
{
	HAL_Delay(ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt format data
 * @note      none
 */
void mpu9250_interface_debug_print(const char *const fmt, ...)
{
	va_list args;

	/* Process printf with variable number of arguments */
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

/**
 * @brief     interface receive callback
 * @param[in] type irq type
 * @note      none
 */
void mpu9250_interface_receive_callback(uint8_t type)
{
	switch (type)
	{
	case MPU9250_INTERRUPT_MOTION :
	{
		mpu9250_interface_debug_print("mpu9250: irq motion.\n");

		break;
	}
	case MPU9250_INTERRUPT_FIFO_OVERFLOW :
	{
		mpu9250_interface_debug_print("mpu9250: irq fifo overflow.\n");

		break;
	}
	case MPU9250_INTERRUPT_FSYNC_INT :
	{
		mpu9250_interface_debug_print("mpu9250: irq fsync int.\n");

		break;
	}
	case MPU9250_INTERRUPT_DMP :
	{
		mpu9250_interface_debug_print("mpu9250: irq dmp\n");

		break;
	}
	case MPU9250_INTERRUPT_DATA_READY :
	{
		mpu9250_interface_debug_print("mpu9250: irq data ready\n");

		break;
	}
	default :
	{
		mpu9250_interface_debug_print("mpu9250: irq unknown code.\n");

		break;
	}
	}
}

/**
 * @brief     interface dmp tap callback
 * @param[in] count tap count
 * @param[in] direction tap direction
 * @note      none
 */
void mpu9250_interface_dmp_tap_callback(uint8_t count, uint8_t direction)
{
	switch (direction)
	{
	case MPU9250_DMP_TAP_X_UP :
	{
		mpu9250_interface_debug_print("mpu9250: tap irq x up with %d.\n", count);

		break;
	}
	case MPU9250_DMP_TAP_X_DOWN :
	{
		mpu9250_interface_debug_print("mpu9250: tap irq x down with %d.\n", count);

		break;
	}
	case MPU9250_DMP_TAP_Y_UP :
	{
		mpu9250_interface_debug_print("mpu9250: tap irq y up with %d.\n", count);

		break;
	}
	case MPU9250_DMP_TAP_Y_DOWN :
	{
		mpu9250_interface_debug_print("mpu9250: tap irq y down with %d.\n", count);

		break;
	}
	case MPU9250_DMP_TAP_Z_UP :
	{
		mpu9250_interface_debug_print("mpu9250: tap irq z up with %d.\n", count);

		break;
	}
	case MPU9250_DMP_TAP_Z_DOWN :
	{
		mpu9250_interface_debug_print("mpu9250: tap irq z down with %d.\n", count);

		break;
	}
	default :
	{
		mpu9250_interface_debug_print("mpu9250: tap irq unknown code.\n");

		break;
	}
	}
}

/**
 * @brief     interface dmp orient callback
 * @param[in] orientation dmp orientation
 * @note      none
 */
void mpu9250_interface_dmp_orient_callback(uint8_t orientation)
{
	switch (orientation)
	{
	case MPU9250_DMP_ORIENT_PORTRAIT :
	{
		mpu9250_interface_debug_print("mpu9250: orient irq portrait.\n");

		break;
	}
	case MPU9250_DMP_ORIENT_LANDSCAPE :
	{
		mpu9250_interface_debug_print("mpu9250: orient irq landscape.\n");

		break;
	}
	case MPU9250_DMP_ORIENT_REVERSE_PORTRAIT :
	{
		mpu9250_interface_debug_print("mpu9250: orient irq reverse portrait.\n");

		break;
	}
	case MPU9250_DMP_ORIENT_REVERSE_LANDSCAPE :
	{
		mpu9250_interface_debug_print("mpu9250: orient irq reverse landscape.\n");

		break;
	}
	default :
	{
		mpu9250_interface_debug_print("mpu9250: orient irq unknown code.\n");

		break;
	}
	}
}
