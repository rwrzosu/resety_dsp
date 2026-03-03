/*
 *  init_platform.h
 *
 *  Configures the ADSP-BF70x initcode DXE via macro defines. The various
 *  configuration macros are defined to default values below. Alternative
 *  configurations can be achieved by defining the macros as required in the
 *  project options or by modifying the definitions in this file.
 *
 *  File Version 2.2.0.0
 *
 *  Copyright (c) 2011-2015 Analog Devices, Inc. All Rights Reserved.
 */

#ifndef INIT_PLATFORM_H
#define INIT_PLATFORM_H

#if !defined(__ADSPBF60x__)
  #error "Expected target processor to be a ADSP-BF60x family part""
#endif

#define TEST_BUILD 0
#if TEST_BUILD
#define CONFIG_CGU 1
#define CONFIG_MEMORY 1
#define CHANGE_SPI_BAUD 0
#define CONFIG_CPLBS 0
#define ENABLE_DCACHE 0
#define ENABLE_DCACHE2 0
#endif

#define CLKIN_Hz              25000000  /* CLKIN [Hz]   */
#define CLKIN_ns (1000000000/CLKIN_Hz)  /* CLKIN [ns]   */

/*
 *  CGU initialization is controlled by the definition of macro CONFIG_CGU.
 */

#if !defined(CONFIG_CGU)
  #define CONFIG_CGU 1 /* CGU initialization is done by default */
#endif
#if CONFIG_CGU
void init_cgu(void);
#endif

/*
 *  DDR2 memory initialization is controlled by the definition of macro CONFIG_MEMORY.
 */

#if !defined(CONFIG_MEMORY)
  #define CONFIG_MEMORY 1 /* DDR2 memory initialization is done by default */
#endif
#if CONFIG_MEMORY
void init_memory(void);
#endif

/*
 * For Boot from SPI memory, this definition will modify the SPI_BAUD
 * register value and can increase or decrease the boot speed.
 *
 * To activate, define __CHANGE_SPI_BAUD__ to 1.
 * Boot ROM default: 0x1F
 */

#if !defined(CHANGE_SPI_BAUD)
  #define CHANGE_SPI_BAUD 1 /* done by default */
#endif

#if CHANGE_SPI_BAUD && !defined(SPI_BAUD_VAL)
  #define SPI_BAUD_VAL 0x1F
#endif

/*
 * CPLB initialization (instruction cache only) can be activated by default
 * See "instruction cache only" line in chart below.
 *
 * To change the cache configuration, selectively modify the #defines below,
 * matching a legal cache configuration combination supported for core 0 for the
 * ADSP-BF60x. Each line in the chart below shows a legal combination:
 *
 *   Icache          Dcache A        Dcache B         Write Through
 *   ENABLE_ICACHE   ENABLE_DCACHE   ENABLE_DCACHE2   ENABLE_DCACHE_WT
 *     0               0               0                0
 *     1               0               0                0      instruction cache only
 *     1               1               0                0
 *     1               1               1                0
 *     1               1               0                1
 *     1               1               1                1
 *     0               1               0                0
 *     0               1               1                0
 *     0               1               0                1
 *     0               1               1                1
 *
 * To activate CPLB initialization define CONFIG_CPLBS to 1.
 */

#if !defined(CONFIG_CPLBS)
  #define CONFIG_CPLBS 0 /* CPLBS initialization disabled by default due to issues found in testing (reference CCES-1766). */
#endif

#if defined(CONFIG_CPLBS)
  void init_cplbs(void);
  #if !defined(ENABLE_ICACHE)
    #define ENABLE_ICACHE 1
  #endif
  #if !defined(ENABLE_DCACHE)
    #define ENABLE_DCACHE 0
  #endif
  #if !defined(ENABLE_DCACHE2)
    #define ENABLE_DCACHE2 0
  #endif
  #if !defined(ENABLE_DCACHE_WT)
    #define ENABLE_DCACHE_WT 0
  #endif
#endif

/*
 * Silicon anomaly workaround configuration.
 */

#if !defined(WA_ROM_CLKOUTSEL_ANOM_16000023)
/* Anomaly workaround configuration for CGU initialization
 *  16-00-0023 "Rom_SysControl() Does not Update CGU0_CLKOUTSEL"
 */
  #define WA_ROM_CLKOUTSEL_ANOM_16000023  \
          (defined(__SILICON_REVISION__) && \
           (__SILICON_REVISION__ < 0x0 ||  __SILICON_REVISION__ == 0xffff))
#endif

#if !defined(WA_DLLLOCK_ANOM_16000046)
/* Silicon anomaly workaround configuration for DMC PHY DLL locking
 * issue 16-00-0046 "DLL in the DMC PHY May Not Lock to New DCLK Frequency".
 */
  #define WA_DLLLOCK_ANOM_16000046 1 /* workaround enabled by default */
#endif

#endif /* INIT_PLATFORM_H */
