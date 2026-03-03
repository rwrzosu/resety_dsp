/*
 * init_cgu.c
 *
 * Initialize CGU programmatically.
 *
 * File Version 2.2.0.0
 *
 * Copyright (c) 2011-2015 Analog Devices, Inc. All Rights Reserved.
 */

#include "init_cgu.h"

#if CONFIG_CGU

#if defined(_MISRA_2004_RULES) || defined(_MISRA_2012_RULES)
  #pragma diag(push)
  #ifdef _MISRA_2012_RULES
    #pragma diag(suppress:misra2012_rule_1_3:"anomaly macros break this rule, it expands to use the defined token. This type of macro definition works fine with ADI supplied compilers and assemblers.")
  #else
    #pragma diag(suppress:misra_rule_19_14:"anomaly macros break this rule, it expands to use the defined token. This type of macro definition works fine with ADI supplied compilers and assemblers.")
  #endif
#endif

void init_cgu(void)
{
   STRUCT_ROM_SYSCTRL ConfigValues;

   ConfigValues.ulCGU_CTL =        MSEL;

   ConfigValues.ulCGU_DIV =        OSEL | DSEL | S1SEL | S0SEL | SYSSEL | CSEL;

                                 /** CLKOUTSEL OPTIONS:
                                  **   ENUM_CGU_CLKOUTSEL_CLKIN        CLKIN
                                  **   ENUM_CGU_CLKOUTSEL_CCLKDIV4     CCLKn/4
                                  **   ENUM_CGU_CLKOUTSEL_DISABLE      Disable OUTCLK
                                  **   ENUM_CGU_CLKOUTSEL_SYSCLKDIV2   SYSCLK/2
                                  **   ENUM_CGU_CLKOUTSEL_SCLK0        SCLK0
                                  **   ENUM_CGU_CLKOUTSEL_SCLK1        SCLK1
                                  **   ENUM_CGU_CLKOUTSEL_DCLKDIV2     DCLK/2
                                  **   ENUM_CGU_CLKOUTSEL_USBPLL       USB PLL
                                  **   ENUM_CGU_CLKOUTSEL_OUTCLK       OUTCLK
                                  **   ENUM_CGU_CLKOUTSEL_USBCLKIN     USB CLKIN
                                  **/

   ConfigValues.ulCGU_CLKOUTSEL =  ENUM_CGU_CLKOUTSEL_CCLKDIV4;

   rom_SysControl((BITM_ROM_SYSCTRL_CGU_WRITE  |
                   BITM_ROM_SYSCTRL_CGU_CTL    |
                   BITM_ROM_SYSCTRL_CGU_DIV    |
                   BITM_ROM_SYSCTRL_CGU_CLKOUTSEL),
                   &ConfigValues, NULL);

#if WA_ROM_CLKOUTSEL_ANOM_16000023
   *pREG_CGU0_CLKOUTSEL = ENUM_CGU_CLKOUTSEL_CCLKDIV4;
#endif /* WA_ROM_CLKOUTSEL_ANOM_16000023 */

} /* init_cgu */

#if defined(_MISRA_2004_RULES) || defined(_MISRA_2012_RULES)
  #pragma diag(pop)
#endif

#else
/* suppress translation unit must contain at least one declaration warning */
#pragma diag(suppress : 96)
#endif /* CONFIG_CGU */
