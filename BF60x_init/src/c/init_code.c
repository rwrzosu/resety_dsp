/*
 * init_code.c
 *
 * Run initialization code to configure for ADSP-BF609 EZ-Board.
 *
 * File Version 2.2.0.0
 *
 * Copyright (c) 2011-2015 Analog Devices, Inc. All Rights Reserved.
 */

#include "init_platform.h"
#if !CONFIG_CGU && !CONFIG_MEMORY && !CHANGE_SPI_BAUD && !CONFIG_CPLBS
  #error no configuration macros are set
#endif

#include <sys/platform.h>
#include <bfrom.h>
#include <builtins.h>
#include <stdbool.h>
#include "uart.h"

#pragma retain_name /* retain resolved initcode entry */
void initcode(STRUCT_ROM_BOOT_CONFIG* pBS);

#pragma retain_name /* retain resolved initcode entry */
void initcode(STRUCT_ROM_BOOT_CONFIG* pBS)
{
#if CONFIG_CGU

	{
		uint32_t udUART_BIT_RATE = 0uL;
		uint32_t udUART_CTL = 0uL;
		bool isUartHostBmode = (*pREG_RCU0_STAT & BITM_RCU_STAT_BMODE) == (7uL << BITP_RCU_STAT_BMODE);

		/*
		 * Mode 'Boot from UART host (slave mode)'
		 * Save current BIT RATE value and Force transfer stop before changing CGU
		 */

		if(isUartHostBmode)
		{
			udUART_BIT_RATE = UartGetBitrate(0uL);
			udUART_CTL = UartRxFifoClear(0uL, udUART_BIT_RATE);
		}

#if WA_DLLLOCK_ANOM_16000046
		*pREG_DMC0_PHY_CTL0 |= 0x0800uL;
#endif

		/* Configure the CGU */

		init_cgu();

#if WA_DLLLOCK_ANOM_16000046
		*pREG_DMC0_PHY_CTL0 &= (~0x0800uL);
#endif

		/*
		 * Mode 'Boot from UART host (slave mode)'
		 * Update UART Clock register according to new clock frequencies
		 * and restore UART Control Register
		 */

		if(isUartHostBmode)
		{
			UartSetBitrate(0uL, udUART_BIT_RATE);
			*pREG_UART0_CTL = udUART_CTL;
		}

	}
#endif /* CONFIG_CGU */

#if CONFIG_MEMORY
   /* initializes DDR2 SDRAM memory */
   init_memory();

#endif /* CONFIG_MEMORY */

/*****************************************************************************
    BMODE 3: 'Boot from SPI memory (Master mode)'
    modify SPIx_BAUD register to speed up booting from SPI memory
******************************************************************************/
#if CHANGE_SPI_BAUD
   if ( (*pREG_RCU0_STAT & BITM_RCU_STAT_BMODE) == (3uL << BITP_RCU_STAT_BMODE)  )
   {
       *pREG_SPI0_CLK = (uint32_t)SPI_BAUD_VAL;
   }
#endif /* CHANGE_SPI_BAUD */

#if CONFIG_CPLBS
   /* Configure CPLBs */
   init_cplbs();
#endif /* CONFIG_CPLBS */

} /* initcode */
