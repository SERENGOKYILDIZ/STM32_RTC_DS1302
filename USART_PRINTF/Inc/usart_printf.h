#ifndef __USART_PRINTF_H__
#define __USART_PRINTF_H__

#if defined(STM32F103xB) || defined(STM32F103xE)
// STM32F1 series
#include "stm32f1xx_hal.h"
#elif defined(STM32F407xx) || defined(STM32F429xx)
// STM32F4 series
#include "stm32f4xx_hal.h"
#else
#error "Unsupported or unknown STM32 family"
#endif
#include <stdio.h>

void usart_printf_init(UART_HandleTypeDef* huart);
int __io_putchar(int ch);
//int fputc(int ch, FILE *f);

#endif
