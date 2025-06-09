/*
 * DS1302.h
 *
 *  Created on: Jun 8, 2025
 *      Author: Eren
 */

#ifndef DS1302_INC_DS1302_H_
#define DS1302_INC_DS1302_H_

#include "main.h"
#include <stdbool.h>

//--------------------------------------------- DEFINES ---------------------------------------------//
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

#define GET_BIT(value, bit) 	(((value) >> (bit)) & 0x01)
#define DECTOBCD(value) 		(((value / 10) << 4) | (value % 10))
#define BCDTODEC(value) 		((value & 0b00001111)+(((value & 0b11110000) >> 4)*10))
//---------------------------------------------------------------------------------------------------//

//--------------------------------------------- STRUCTS ---------------------------------------------//
typedef struct{
	const GPIO_TypeDef* port;
	uint16_t pin;
}GpioPin;


typedef enum{AM,PM,NONE}MeridiemEnum;

typedef enum{MON=1, TUE, WED, THU, FRI, SAT, SUN}DaysEnum;

typedef struct{
	int hour;
	MeridiemEnum meridiem;
}Hour;

typedef struct{
	int sec;
	int min;
	Hour hour;
	int date;
	int month;
	int year;
	DaysEnum day;
}DS1302_TimeRecord;

typedef struct{
	GpioPin CE_Pin;
	GpioPin IO_Pin;
	GpioPin SCLK_Pin;
}DS1302_HandleTypeDef;
//---------------------------------------------------------------------------------------------------//

//--------------------------------------------- FUNCTIONS ---------------------------------------------//

//Initialization
void ds1302_init(DS1302_HandleTypeDef* handel);

//Byte Operations
uint8_t ds1302_readByte(DS1302_HandleTypeDef* handel, uint8_t address);
void ds1302_writeByte(DS1302_HandleTypeDef* handel, uint8_t data, uint8_t address);

//GET Operations
int ds1302_getSecond(DS1302_HandleTypeDef* handel);
int ds1302_getMinute(DS1302_HandleTypeDef* handel);
Hour ds1302_getHour(DS1302_HandleTypeDef* handel);
int ds1302_getDate(DS1302_HandleTypeDef* handel);
int ds1302_getMonth(DS1302_HandleTypeDef* handel);
int ds1302_getYear(DS1302_HandleTypeDef* handel);
DaysEnum ds1302_getDay(DS1302_HandleTypeDef* handel);
DS1302_TimeRecord ds1302_getDateTime(DS1302_HandleTypeDef* handel);
void ds1302_updateDateTime(DS1302_HandleTypeDef* handel, DS1302_TimeRecord* datetime);

//SET Operations
bool ds1302_setSecond(DS1302_HandleTypeDef* handel, uint8_t second);
bool ds1302_setMinute(DS1302_HandleTypeDef* handel, uint8_t minute);
bool ds1302_setHour(DS1302_HandleTypeDef* handel, Hour hour);
bool ds1302_setDate(DS1302_HandleTypeDef* handel, uint8_t date);
bool ds1302_setMonth(DS1302_HandleTypeDef* handel, uint8_t month);
bool ds1302_setYear(DS1302_HandleTypeDef* handel, uint8_t year);
bool ds1302_setDay(DS1302_HandleTypeDef* handel, DaysEnum day);
bool ds1302_setDateTime(DS1302_HandleTypeDef* handel, DS1302_TimeRecord datetime);
//--------------------------------------------------------------------------------------------------------///

#endif /* DS1302_INC_DS1302_H_ */
