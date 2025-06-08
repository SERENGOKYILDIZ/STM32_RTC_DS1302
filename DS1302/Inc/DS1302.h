/*
 * DS1302.h
 *
 *  Created on: Jun 8, 2025
 *      Author: Eren
 */

#ifndef DS1302_INC_DS1302_H_
#define DS1302_INC_DS1302_H_

#include "main.h"

#define GET_BIT(value, bit) (((value) >> (bit)) & 0x01)

typedef struct{
	const GPIO_TypeDef* port;
	uint16_t pin;
}GpioPin;

typedef struct{
	GpioPin CE_Pin;
	GpioPin IO_Pin;
	GpioPin SCLK_Pin;
}DS1302_HandleTypeDef;


void ds1302_init(DS1302_HandleTypeDef* handel);
uint8_t ds1302_readByte(DS1302_HandleTypeDef* handel, uint8_t address);

#endif /* DS1302_INC_DS1302_H_ */
