/*
 * system.c
 *
 * ADSP-BF60x system
 *
 * File Version 2.2.0.1
 *
 * Copyright (c) 2011-2015 Analog Devices, Inc. All Rights Reserved.
 */

/*****************************************************************************
 Include Files
******************************************************************************/

#include "system.h"

/*****************************************************************************
 Functions
******************************************************************************/


/****************************************************************************
 Name:          get_pllclk_hz
 Description:   get current PLL clock frequency in Hz
 Input:         uint32_t ulRegCgu0Ctl
 Return:        uint32_t PLLCLK [Hz]
*****************************************************************************/
uint32_t get_pllclk_hz(uint32_t ulRegCgu0Ctl)
{
    uint32_t ulMsel;
    uint32_t ulPllClkHz;
    STRUCT_ROM_SYSCTRL pllclk;

    if (ulRegCgu0Ctl == 0uL) {
        rom_SysControl( BITM_ROM_SYSCTRL_CGU_READ | BITM_ROM_SYSCTRL_CGU_CTL, &pllclk, NULL );
    }
    else {
        pllclk.ulCGU_CTL  = ulRegCgu0Ctl;
    }

    ulMsel = ( (pllclk.ulCGU_CTL & BITM_CGU_CTL_MSEL) >> BITP_CGU_CTL_MSEL );
    if ( ulMsel == 0uL ) {
    	ulMsel = 128uL;
    }
    ulPllClkHz = ulMsel * (uint32_t)CLKIN_Hz;

    return ( ulPllClkHz >> (BITM_CGU_CTL_DF & pllclk.ulCGU_CTL) );
} /* get_pllclk_hz */


/****************************************************************************
 Name:          get_pllclk_ns
 Description:   get current PLL clock frequency in ns
 Input:         uint32_t ulRegCgu0Ctl
 Return:        uint32_t PLLCLK [ns]
*****************************************************************************/
uint32_t get_pllclk_ns(uint32_t ulRegCgu0Ctl)
{
    uint32_t ulMsel;
    uint32_t ulPllClkNs;
    STRUCT_ROM_SYSCTRL pllclk;

    if (ulRegCgu0Ctl == 0uL) {
        rom_SysControl( BITM_ROM_SYSCTRL_CGU_READ | BITM_ROM_SYSCTRL_CGU_CTL, &pllclk, NULL );
    }
    else {
        pllclk.ulCGU_CTL = ulRegCgu0Ctl;
    }

    ulMsel = ( (pllclk.ulCGU_CTL & BITM_CGU_CTL_MSEL) >> BITP_CGU_CTL_MSEL );
    if ( ulMsel == 0uL ) {
    	ulMsel = 128uL;
    }
    ulPllClkNs = (uint32_t)CLKIN_ns / ulMsel;

    return ( ulPllClkNs << (BITM_CGU_CTL_DF & pllclk.ulCGU_CTL) );
} /* get_pllclk_ns */


/****************************************************************************
 Name:          get_cclk_hz
 Description:   get current core clock frequency in Hz
 Input:         none
 Return:        uint32_t CCLK [Hz]
*****************************************************************************/
uint32_t get_cclk_hz(void)
{
	STRUCT_ROM_SYSCTRL cclk;
	uint32_t ulCclk_hz;

	rom_SysControl( BITM_ROM_SYSCTRL_CGU_READ | BITM_ROM_SYSCTRL_CGU_CTL |
			        BITM_ROM_SYSCTRL_CGU_DIV | BITM_ROM_SYSCTRL_CGU_STAT,
			        &cclk, NULL );

	if ((cclk.ulCGU_STAT & BITM_CGU_STAT_PLLBP) != 0uL) {
		ulCclk_hz = (uint32_t) CLKIN_Hz;
	} else {
		uint32_t ulCsel = ( (cclk.ulCGU_DIV & BITM_CGU_DIV_CSEL) >> BITP_CGU_DIV_CSEL );
		if (ulCsel == 0uL) {
			ulCclk_hz = 0uL;
		} else {
			ulCclk_hz = get_pllclk_hz(cclk.ulCGU_CTL) / ulCsel;
		}
	}
	return ulCclk_hz;
} /* get_cclk_hz */


/****************************************************************************
 Name:          get_cclk_ns
 Description:   get current core clock frequency in ns
 Input:         none
 Return:        uint32_t CCLK [ns]
*****************************************************************************/
uint32_t get_cclk_ns(void)
{
	STRUCT_ROM_SYSCTRL cclk;
	uint32_t ulCclk_ns;

	rom_SysControl( BITM_ROM_SYSCTRL_CGU_READ | BITM_ROM_SYSCTRL_CGU_CTL |
			        BITM_ROM_SYSCTRL_CGU_DIV | BITM_ROM_SYSCTRL_CGU_STAT,
			        &cclk, NULL );

	if ((cclk.ulCGU_STAT & BITM_CGU_STAT_PLLBP) != 0uL) {
		ulCclk_ns = (uint32_t) CLKIN_Hz;
	} else {
		uint32_t ulCsel = ( (cclk.ulCGU_DIV & BITM_CGU_DIV_CSEL) >> BITP_CGU_DIV_CSEL );
		if (ulCsel == 0uL) {
			ulCclk_ns = 0uL;
		} else {
			ulCclk_ns = get_pllclk_ns(cclk.ulCGU_CTL) * ulCsel;
		}
	}
	return ulCclk_ns;
} /* get_cclk_ns */


/****************************************************************************
 Name:          get_sysclk_hz
 Description:   get current system clock frequency in Hz
 Input:         uint32_t ulRegCgu0Ctl, uint32_t ulRegCgu0Stat, uint32_t ulRegCgu0Div
 Return:        uint32_t SYSCLK [Hz]
*****************************************************************************/
uint32_t get_sysclk_hz(uint32_t ulRegCgu0Ctl, uint32_t ulRegCgu0Stat, uint32_t ulRegCgu0Div)
{
	STRUCT_ROM_SYSCTRL sysclk;
	uint32_t ulSysclk_hz;

	if (ulRegCgu0Ctl == 0uL) {
		rom_SysControl( BITM_ROM_SYSCTRL_CGU_READ | BITM_ROM_SYSCTRL_CGU_CTL |
				        BITM_ROM_SYSCTRL_CGU_DIV | BITM_ROM_SYSCTRL_CGU_STAT,
				        &sysclk, NULL );
	} else {
		sysclk.ulCGU_CTL  = ulRegCgu0Ctl;
		sysclk.ulCGU_DIV  = ulRegCgu0Div;
		sysclk.ulCGU_STAT = ulRegCgu0Stat;
	}

	if ((sysclk.ulCGU_STAT & BITM_CGU_STAT_PLLBP) != 0uL) {
		ulSysclk_hz = (uint32_t) CLKIN_Hz;
	} else {
		uint32_t ulSysSel = ( (sysclk.ulCGU_DIV & BITM_CGU_DIV_SYSSEL) >> BITP_CGU_DIV_SYSSEL );
		if (ulSysSel == 0uL) {
			ulSysclk_hz = 0uL;
		} else {
			ulSysclk_hz = get_pllclk_hz(sysclk.ulCGU_CTL) / ulSysSel;
		}
	}
	return ulSysclk_hz;
} /* get_sysclk_hz */


/****************************************************************************
 Name:          get_s0clk_hz
 Description:   get current s0 clock frequency in Hz
 Input:         none
 Return:        uint32_t S0CLK [Hz]
*****************************************************************************/
uint32_t get_s0clk_hz(void)
{
	STRUCT_ROM_SYSCTRL s0clk;
	uint32_t ulS0clk_hz;

	rom_SysControl( BITM_ROM_SYSCTRL_CGU_READ | BITM_ROM_SYSCTRL_CGU_CTL |
			        BITM_ROM_SYSCTRL_CGU_DIV | BITM_ROM_SYSCTRL_CGU_STAT,
			        &s0clk, NULL );

	if ((s0clk.ulCGU_STAT & BITM_CGU_STAT_PLLBP) != 0uL) {
		ulS0clk_hz = (uint32_t) CLKIN_Hz;
	} else {
		uint32_t ulS0sel = ( (s0clk.ulCGU_DIV & BITM_CGU_DIV_S0SEL) >> BITP_CGU_DIV_S0SEL );
		if (ulS0sel == 0uL) {
			ulS0clk_hz = 0uL;
		} else {
			ulS0clk_hz = get_sysclk_hz(s0clk.ulCGU_CTL, s0clk.ulCGU_STAT, s0clk.ulCGU_DIV) / ulS0sel;
		}
	}
	return ulS0clk_hz;
} /* get_s0clk_hz */


/****************************************************************************
 Name:          get_s1clk_hz
 Description:   get current s1 clock frequency in Hz
 Input:         none
 Return:        uint32_t S1CLK [Hz]
*****************************************************************************/
uint32_t get_s1clk_hz(void)
{
	STRUCT_ROM_SYSCTRL s1clk;
	uint32_t ulS1clk_hz;

	rom_SysControl( BITM_ROM_SYSCTRL_CGU_READ | BITM_ROM_SYSCTRL_CGU_CTL |
			        BITM_ROM_SYSCTRL_CGU_DIV | BITM_ROM_SYSCTRL_CGU_STAT,
			        &s1clk, NULL );

	if ((s1clk.ulCGU_STAT & BITM_CGU_STAT_PLLBP) != 0uL) {
		ulS1clk_hz = (uint32_t) CLKIN_Hz;
	} else {
		uint32_t ulS1sel = ( (s1clk.ulCGU_DIV & BITM_CGU_DIV_S1SEL) >> BITP_CGU_DIV_S1SEL );
		if (ulS1sel == 0uL) {
			ulS1clk_hz = 0uL;
		} else {
			ulS1clk_hz = get_sysclk_hz(s1clk.ulCGU_CTL, s1clk.ulCGU_STAT, s1clk.ulCGU_DIV) / ulS1sel;
		}
	}
	return ulS1clk_hz;
} /* get_s1clk_hz */


/****************************************************************************
 Name:          get_ddrclk_hz
 Description:   get current DDR clock frequency in Hz
 Input:         none
 Return:        uint32_t DDRCLK [Hz]
*****************************************************************************/
uint32_t get_ddrclk_hz(void)
{
	STRUCT_ROM_SYSCTRL ddrclk;
	uint32_t ulDdrclk_hz;

	rom_SysControl( BITM_ROM_SYSCTRL_CGU_READ | BITM_ROM_SYSCTRL_CGU_CTL |
			        BITM_ROM_SYSCTRL_CGU_DIV | BITM_ROM_SYSCTRL_CGU_STAT,
			        &ddrclk, NULL );

	if ((ddrclk.ulCGU_STAT & BITM_CGU_STAT_PLLBP) != 0uL) {
		ulDdrclk_hz = (uint32_t) CLKIN_Hz;
	} else {
		uint32_t ulDsel = ( (ddrclk.ulCGU_DIV & BITM_CGU_DIV_DSEL) >> BITP_CGU_DIV_DSEL );
		if (ulDsel == 0uL) {
			ulDdrclk_hz = 0uL;
		} else {
			ulDdrclk_hz = get_pllclk_hz(ddrclk.ulCGU_CTL) / ulDsel;
		}
	}
	return ulDdrclk_hz;
} /* get_ddrclk_hz */


/****************************************************************************
 Name:          get_ddrclk_ns
 Description:   get current DDR clock frequency in ns
 Input:         none
 Return:        uint32_t DDRCLK [ns]
*****************************************************************************/
uint32_t get_ddrclk_ns(void)
{
	STRUCT_ROM_SYSCTRL ddrclk;
	uint32_t ulDdrclk_ns;

	rom_SysControl( BITM_ROM_SYSCTRL_CGU_READ | BITM_ROM_SYSCTRL_CGU_CTL |
			BITM_ROM_SYSCTRL_CGU_DIV | BITM_ROM_SYSCTRL_CGU_STAT,
			&ddrclk, NULL );

	if ((ddrclk.ulCGU_STAT & BITM_CGU_STAT_PLLBP) != 0uL) {
		ulDdrclk_ns = (uint32_t) CLKIN_ns;
	} else {
		uint32_t ulDsel = ( (ddrclk.ulCGU_DIV & BITM_CGU_DIV_DSEL) >> BITP_CGU_DIV_DSEL );
		if (ulDsel == 0uL) {
			ulDdrclk_ns = 0uL;
		} else {
			ulDdrclk_ns = get_pllclk_ns(ddrclk.ulCGU_CTL) * ulDsel;
		}
	}
	return ulDdrclk_ns;
} /* get_ddrclk_ns */


/****************************************************************************
 Name:          ErrorWithNoReturn
 Description:   Kernel Panic!!!
 Input:         none
 Return:        none
*****************************************************************************/

#pragma noreturn
void ErrorWithNoReturn(void)
{
    while (true) {
    	asm volatile("EMUEXCPT;");
    	idle();
    }
} /* ErrorWithNoReturn */

/****************************************************************************
 EOF
*****************************************************************************/
