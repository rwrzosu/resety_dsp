/*
 **
 ** Source file generated on listopada 26, 2025 at 13:47:04.	
 **
 ** Copyright (C) 2011-2025 Analog Devices Inc., All Rights Reserved.
 **
 ** This file is generated automatically based upon the options selected in 
 ** the Pin Multiplexing configuration editor. Changes to the Pin Multiplexing
 ** configuration should be made by changing the appropriate options rather
 ** than editing this file.
 **
 ** Selected Peripherals
 ** --------------------
 ** UART0 (TX)
 **
 ** GPIO (unavailable)
 ** ------------------
 ** PD07
 */

#include <sys/platform.h>
#include <stdint.h>

#define UART0_TX_PORTD_MUX  ((uint16_t) ((uint16_t) 1<<14))

#define UART0_TX_PORTD_FER  ((uint16_t) ((uint16_t) 1<<7))

int32_t adi_initpinmux(void);

/*
 * Initialize the Port Control MUX and FER Registers
 */
int32_t adi_initpinmux(void) {
    /* PORTx_MUX registers */
    *pREG_PORTD_MUX = UART0_TX_PORTD_MUX;

    /* PORTx_FER registers */
    *pREG_PORTD_FER = UART0_TX_PORTD_FER;
    return 0;
}

