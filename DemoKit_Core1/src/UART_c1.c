#include "UART_c1.h"
#include "stdint.h"
#include <stdio.h>
#include <stdarg.h>

#include <cdefBF609.h>


void UART_printf (const char *format, ...)
{
	int ret;
	uint16_t hsize;

	char buff [256];

	va_list args;

	va_start(args, format);
	int size = vsprintf( buff , format, args);
	va_end(args);

	unsigned int i = 0;
	while (buff[i] != 0){
		while ((*pREG_UART0_STAT & ENUM_UART_STAT_THR_EMPTY) == 0);
		*pREG_UART0_THR = buff[i];
		i++;
	}

	return ;
}


#ifdef _MISRA_RULES
#pragma diag(pop)
#endif /* _MISRA_RULES */
