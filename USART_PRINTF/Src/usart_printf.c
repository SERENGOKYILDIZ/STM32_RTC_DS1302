#include "usart_printf.h"

static UART_HandleTypeDef* usart_printf_uart = NULL;

void usart_printf_init(UART_HandleTypeDef* huart)
{
    usart_printf_uart = huart;
}

int __io_putchar(int ch)
{
    if (usart_printf_uart)
        HAL_UART_Transmit(usart_printf_uart, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

// In some toolchains, fputc() or putchar() routing may be preferred instead of __io_putchar.
/*
int fputc(int ch, FILE *f)
{
    return __io_putchar(ch);
}
*/
