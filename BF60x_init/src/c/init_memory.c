/*
 * init_memory.c
 *
 * Initialize memory programmatically
 * DMC is the Dynamic Memory Controller, the initialization is for DDR2 SDRAM
 *
 * File Version 2.2.0.0
 *
 * Copyright (c) 2011-2015 Analog Devices, Inc. All Rights Reserved.
 */

#include "init_memory.h"

#if CONFIG_MEMORY

void init_memory(void)
{
   uint32_t i, Current_CSEL_Val, Current_DSEL_Val, DLL_Lock_Cycles;

   /* Read the current CSEL and DSEL values to calculate the DLL lock cycles */

   Current_CSEL_Val = (*pREG_CGU0_DIV & BITM_CGU_DIV_CSEL) >> BITP_CGU_DIV_CSEL ;
   Current_DSEL_Val = (*pREG_CGU0_DIV & BITM_CGU_DIV_DSEL) >> BITP_CGU_DIV_DSEL ;

   DLL_Lock_Cycles = (uint32_t) DLL_LOCK_PERIOD * (Current_DSEL_Val/Current_CSEL_Val);

   /* Wait for DLL to lock */
   for (i=0uL; i<DLL_Lock_Cycles; i++)
   {
      NOP();
   }

   /* Keep the DLLCALRDCNT reset value and only modify DATACYC */
   /*
   *pREG_DMC0_DLLCTL 	= 0x0000054B;
   *pREG_DMC0_CFG  		= 0x00000522;
   *pREG_DMC0_TR0  		= 0x20E0A424;
   *pREG_DMC0_TR1  		= 0x3020079E;
   *pREG_DMC0_TR2  		= 0x0032050D;
   *pREG_DMC0_MR   		= 0x00000842;
   *pREG_DMC0_PHY_CTL3 	= 0x050000C4;
   *pREG_DMC0_PHY_CTL1 	= 0x00000000;
   *pREG_DMC0_CTL  		= 0x00000804;
   */

   *pREG_DMC0_CFG       = DMC_CFG;
   *pREG_DMC0_TR0       = DMC_TR0;
   *pREG_DMC0_TR1       = DMC_TR1;
   *pREG_DMC0_TR2       = DMC_TR2;
   *pREG_DMC0_MR        = DMC_MR;
   *pREG_DMC0_PHY_CTL3  = DMC_PHY_CTL3;
   *pREG_DMC0_PHY_CTL1  = DMC_PHY_CTL1;
   *pREG_DMC0_CTL       = DMC_CTL;


   /* Wait for DDR2 initialization to complete */
   while( (*pREG_DMC0_STAT & BITM_DMC_STAT_MEMINITDONE) == 0uL) { }

   /* Wait for Calibration to complete */
   while( (*pREG_DMC0_STAT & BITM_DMC_STAT_DLLCALDONE) == 0uL) { }

   /* keep the DLLCALRDCNT reset value and only modify DATACYC */
   *pREG_DMC0_DLLCTL= 0x54B;

} /* init_memory */

#endif /* CONFIG_MEMORY */
