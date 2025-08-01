/*
 * DS1302.c
 *
 *  Created on: Jun 8, 2025
 *      Author: Eren
 */
#include <DS1302.h>

static void DelayUs(uint32_t us)
{
	for(uint32_t i=0;i<us*(HAL_RCC_GetHCLKFreq() / 1000000)/3;i++);
}

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

void ds1302_enableWriteMode(DS1302_HandleTypeDef* handel)
{
	GPIO_InitTypeDef gpioInit={
			.Pin = handel->IO_Pin.pin,
			.Mode = GPIO_MODE_OUTPUT_PP,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_FREQ_LOW
	};

	HAL_GPIO_Init(handel->IO_Pin.port, &gpioInit);
}

void ds1302_enableReadMode(DS1302_HandleTypeDef* handel)
{
	GPIO_InitTypeDef gpioInit={
			.Pin = handel->IO_Pin.pin,
			.Mode = GPIO_MODE_INPUT,
			.Pull = GPIO_NOPULL
	};

	HAL_GPIO_Init(handel->IO_Pin.port, &gpioInit);
}

uint8_t ds1302_readByte(DS1302_HandleTypeDef* handel, uint8_t address)
{
	uint8_t data = 0x00;
	address |= 0x01;

	HAL_GPIO_WritePin(handel->CE_Pin.port, handel->CE_Pin.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(handel->IO_Pin.port, handel->IO_Pin.pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(handel->CE_Pin.port, handel->CE_Pin.pin, GPIO_PIN_SET);

	//send the address
	for(int i=0;i<8;i++)
	{
		HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_RESET);
		DelayUs(1);
		HAL_GPIO_WritePin(handel->IO_Pin.port, handel->IO_Pin.pin,
				GET_BIT(address, i) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		DelayUs(1);
		HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_SET);
		DelayUs(1);
	}

	ds1302_enableReadMode(handel);

	//read the data byte
	for(int i=0;i<8;i++)
	{
		HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_RESET);
		DelayUs(1);
		if(HAL_GPIO_ReadPin(handel->IO_Pin.port, handel->IO_Pin.pin) == GPIO_PIN_SET)
		{
			data |= (1<<i);
		}
		DelayUs(1);
		HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_SET);
		DelayUs(1);
	}

	HAL_GPIO_WritePin(handel->CE_Pin.port, handel->CE_Pin.pin, GPIO_PIN_RESET);
	DelayUs(1);
	ds1302_enableWriteMode(handel);

	return data;
}

void ds1302_writeByte(DS1302_HandleTypeDef* handel, uint8_t data, uint8_t address)
{
	HAL_GPIO_WritePin(handel->CE_Pin.port, handel->CE_Pin.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(handel->IO_Pin.port, handel->IO_Pin.pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(handel->CE_Pin.port, handel->CE_Pin.pin, GPIO_PIN_SET);

	//send the address
	for(int i=0;i<8;i++)
	{
		HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_RESET);
		DelayUs(1);
		HAL_GPIO_WritePin(handel->IO_Pin.port, handel->IO_Pin.pin,
				GET_BIT(address, i) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		DelayUs(1);
		HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_SET);
		DelayUs(1);
	}

	//send the data in the address
	for(int i=0;i<8;i++)
	{
		HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_RESET);
		DelayUs(1);
		HAL_GPIO_WritePin(handel->IO_Pin.port, handel->IO_Pin.pin,
				GET_BIT(data, i) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		DelayUs(1);
		HAL_GPIO_WritePin(handel->SCLK_Pin.port, handel->SCLK_Pin.pin, GPIO_PIN_SET);
		DelayUs(1);
	}

	HAL_GPIO_WritePin(handel->CE_Pin.port, handel->CE_Pin.pin, GPIO_PIN_RESET);
}

int ds1302_getSecond(DS1302_HandleTypeDef* handel)
{
	uint8_t sec_byte = ds1302_readByte(handel, DS1302_SECONDS);
	return BCDTODEC(sec_byte);
}

int ds1302_getMinute(DS1302_HandleTypeDef* handel)
{
	uint8_t min_byte = ds1302_readByte(handel, DS1302_MINUTES);
	return BCDTODEC(min_byte);
}

Hour ds1302_getHour(DS1302_HandleTypeDef* handel)
{
	uint8_t hour_data_raw = ds1302_readByte(handel, DS1302_HOURS);
	Hour hour_data;
	int hour = hour_data_raw & 0b00001111;
	if((hour_data_raw & 0b10000000)>>7){
		hour_data.meridiem = (((hour_data_raw & 0b00010000)>>5) ? PM : AM);
		int hour10 = (hour_data_raw & 0b00010000) >> 4;
		hour_data.hour = hour10*10+hour;
	}
	else{
		hour_data.meridiem = NONE;
		int hour10 = (hour_data_raw & 0b00110000) >> 4;
		hour_data.hour = hour10*10+hour;
	}

	return hour_data;
}

int ds1302_getDate(DS1302_HandleTypeDef* handel)
{
	uint8_t date_byte = ds1302_readByte(handel, DS1302_DATE);
	return BCDTODEC(date_byte);
}

int ds1302_getMonth(DS1302_HandleTypeDef* handel)
{
	uint8_t month_byte = ds1302_readByte(handel, DS1302_MONTH);
	return BCDTODEC(month_byte);
}

int ds1302_getYear(DS1302_HandleTypeDef* handel)
{
	uint8_t year_byte = ds1302_readByte(handel, DS1302_YEAR);
	return BCDTODEC(year_byte);
}

DaysEnum ds1302_getDay(DS1302_HandleTypeDef* handel)
{
	uint8_t day_byte = ds1302_readByte(handel, DS1302_DAY);
	return BCDTODEC(day_byte);
}

DS1302_TimeRecord ds1302_getDateTime(DS1302_HandleTypeDef* handel)
{
	DS1302_TimeRecord datetime;
	datetime.sec=ds1302_getSecond(handel);
	datetime.min=ds1302_getMinute(handel);
	datetime.hour=ds1302_getHour(handel);
	datetime.date=ds1302_getDate(handel);
	datetime.month=ds1302_getMonth(handel);
	datetime.year=ds1302_getYear(handel);
	datetime.day=ds1302_getDay(handel);
	return datetime;
}

void ds1302_updateDateTime(DS1302_HandleTypeDef* handel, DS1302_TimeRecord* datetime)
{
	datetime->sec=ds1302_getSecond(handel);
	datetime->min=ds1302_getMinute(handel);
	datetime->hour=ds1302_getHour(handel);
	datetime->date=ds1302_getDate(handel);
	datetime->month=ds1302_getMonth(handel);
	datetime->year=ds1302_getYear(handel);
	datetime->day=ds1302_getDay(handel);
}

char* DS1302_getDateTime_ISO(DS1302_HandleTypeDef* handle, DS1302_TimeRecord datetime)
{
    static char buffer[50];
    sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
            datetime.year + 2000,
            datetime.month,
            datetime.date,
            datetime.hour.hour,
            datetime.min,
            datetime.sec);
    return buffer;
}


bool ds1302_setSecond(DS1302_HandleTypeDef* handel, uint8_t second)
{
	if(second > 59) return false;
	ds1302_writeByte(handel, DECTOBCD(second), DS1302_SECONDS);
	return true;
}

bool ds1302_setMinute(DS1302_HandleTypeDef* handel, uint8_t minute)
{
	if(minute > 59) return false;
	ds1302_writeByte(handel, DECTOBCD(minute), DS1302_MINUTES);
	return true;
}

bool ds1302_setHour(DS1302_HandleTypeDef* handel, Hour hour)
{
	if((hour.hour > 23 && hour.meridiem==NONE) || (hour.hour > 12 && hour.meridiem!=NONE)) return false;
	uint8_t hour_data = DECTOBCD(hour.hour);
	if(hour.meridiem != NONE)
	{
		hour_data |= 0b10000000;
		if(hour.meridiem == PM)
			hour_data |= 0b00100000;
	}
	ds1302_writeByte(handel, hour_data, DS1302_HOURS);
	return true;
}

bool ds1302_setDate(DS1302_HandleTypeDef* handel, uint8_t date)
{
	if(date > 31) return false;
	ds1302_writeByte(handel, DECTOBCD(date), DS1302_DATE);
	return true;
}

bool ds1302_setMonth(DS1302_HandleTypeDef* handel, uint8_t month)
{
	if(month > 12) return false;
	ds1302_writeByte(handel, DECTOBCD(month), DS1302_MONTH);
	return true;
}

bool ds1302_setYear(DS1302_HandleTypeDef* handel, uint8_t year)
{
	if(year > 99) return false;
	ds1302_writeByte(handel, DECTOBCD(year), DS1302_YEAR);
	return true;
}

bool ds1302_setDay(DS1302_HandleTypeDef* handel, DaysEnum day)
{
	if(day > 7) return false;
	ds1302_writeByte(handel, day, DS1302_DAY);
	return true;
}

bool ds1302_setDateTime(DS1302_HandleTypeDef* handel, DS1302_TimeRecord datetime)
{
	if(!ds1302_setSecond(handel, datetime.sec)) return false;
	if(!ds1302_setMinute(handel, datetime.min)) return false;
	if(!ds1302_setHour(handel, datetime.hour)) return false;
	if(!ds1302_setDate(handel, datetime.date)) return false;
	if(!ds1302_setMonth(handel, datetime.month)) return false;
	if(!ds1302_setYear(handel, datetime.year)) return false;
	if(!ds1302_setDay(handel, datetime.day)) return false;
	return true;
}
