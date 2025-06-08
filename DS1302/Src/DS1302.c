/*
 * DS1302.c
 *
 *  Created on: Jun 8, 2025
 *      Author: Eren
 */
#include <DS1302.h>

void ds1302_init(DS1302_HandleTypeDef* handel)
{
	HAL_GPIO_WritePin(handel->IO_Pin.port, handel->IO_Pin.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(handel->CE_Pin.port, handel->CE_Pin.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_RESET);

	GPIO_InitTypeDef gpioInit={
			.Pin = handel->IO_Pin.pin,
			.Mode = GPIO_MODE_OUTPUT_PP,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_FREQ_LOW
	};

	HAL_GPIO_Init(handel->IO_Pin.port, &gpioInit);

	gpioInit.Pin = handel->CE_Pin.pin;
	HAL_GPIO_Init(handel->CE_Pin.port, &gpioInit);

	gpioInit.Pin = handel->SCLK_Pin.pin;
	HAL_GPIO_Init(handel->SCLK_Pin.port, &gpioInit);
}
