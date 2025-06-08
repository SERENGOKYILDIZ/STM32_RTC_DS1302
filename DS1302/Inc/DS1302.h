/*
 * DS1302.h
 *
 *  Created on: Jun 8, 2025
 *      Author: Eren
 */

#ifndef DS1302_INC_DS1302_H_
#define DS1302_INC_DS1302_H_

#include "main.h"

#define DS1302_SECONDS 				0X80
#define DS1302_MINUTES  			0X82
#define DS1302_HOURS  				0X84
#define DS1302_DATE		  			0X86
#define DS1302_MONTH	  			0X88
#define DS1302_DAY		  			0X8A
#define DS1302_YEAR		  			0X8C
#define DS1302_CLOCK_BUST  			0XBE
#define DS1302_CLOCK_BUST_WRITE 	0XBE
#define DS1302_CLOCK_BUST_READ  	0XBF
#define DS1302_RAMSTART  			0XC0
#define DS1302_RAMEND	  			0XFC
#define DS1302_RAM_BUST 			0XFE
#define DS1302_RAM_BUST_WRITE 		0XFE
#define DS1302_RAM_BUST_READ 		0XFF

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
void ds1302_writeByte(DS1302_HandleTypeDef* handel, uint8_t data, uint8_t address);

#endif /* DS1302_INC_DS1302_H_ */
