/*
 * init_cgu.h
 *
 * Initialize CGU programmatically.
 *
 * File Version 2.2.0.0
 *
 * Copyright (c) 2011-2015 Analog Devices, Inc. All Rights Reserved.
 */

#ifndef INIT_CGU_H
#define INIT_CGU_H

#include "init_platform.h"

#ifdef CONFIG_CGU

#include <sys/platform.h>
#include <bfrom.h>
#include <stddef.h>

/* PLL Multiplier and Divisor Selections (Required Value, Bit Position) */

#define MSEL   ((20uL << BITP_CGU_CTL_MSEL)   & BITM_CGU_CTL_MSEL)   /* PLL Multiplier Select [1-127]: PLLCLK = ((CLKIN x MSEL/DF+1)) = 1000MHz(max) */

#define CSEL   ((1uL  << BITP_CGU_DIV_CSEL)   & BITM_CGU_DIV_CSEL)   /* Core Clock Divisor Select [1-31]: (CLKIN x MSEL/DF+1)/CSEL = 500MHz(max)     */
#define SYSSEL ((2uL  << BITP_CGU_DIV_SYSSEL) & BITM_CGU_DIV_SYSSEL) /* System Clock Divisor Select [1-31]: (CLKIN x MSEL/DF+1)/SYSSEL = 250MHz(max) */
#define S0SEL  ((2uL  << BITP_CGU_DIV_S0SEL)  & BITM_CGU_DIV_S0SEL)  /* SCLK0 Divisor Select [1-7]: SYSCLK/S0SEL = 125MHz(max)                       */
#define S1SEL  ((2uL  << BITP_CGU_DIV_S1SEL)  & BITM_CGU_DIV_S1SEL)  /* SCLK1 Divisor Select [1-7]: SYSLCK/S1SEL = 125MHz(max)                       */
#define DSEL   ((2uL  << BITP_CGU_DIV_DSEL)   & BITM_CGU_DIV_DSEL)   /* DDR Clock Divisor Select [1-31]: (CLKIN x MSEL/DF+1)/DSEL = 250MHz(max)      */
//#define DSEL   ((3uL  << BITP_CGU_DIV_DSEL)   & BITM_CGU_DIV_DSEL)   /* DDR Clock Divisor Select [1-31]: (CLKIN x MSEL/DF+1)/DSEL = 166MHz(max)      */
#define OSEL   ((4uL  << BITP_CGU_DIV_OSEL)   & BITM_CGU_DIV_OSEL)   /* OUTCLK Divisor Select [1-127]: (CLKIN x MSEL/DF+1)/OSEL = 125MHz(max)        */

#endif /* CONFIG_CGU */

#endif /* INIT_CGU_H */
