/*
 * init_cplbs.c
 *
 * Initialize CPLBs programmatically.
 *
 * Copyright (c) 2011-2015 Analog Devices, Inc. All Rights Reserved.
 */

#include "init_cplbs.h"

#if CONFIG_CPLBS

#define AddCPLB(_addr,_flags) do { \
  addrptr[idx] = (void *)(_addr); \
  dataptr[idx] = (uint32_t)(_flags); \
  idx++; \
} while (0)

static void InitDataCPLBs(void);
static void InitInstrCPLBs(void);

extern void _disable_cplbs(void);

#if defined(_MISRA_2004_RULES)
  #pragma diag(push)
  #pragma diag(suppress:misra_rule_17_4:"anomaly macros break this rule, it expands to use the defined token. This type of macro definition works fine with ADI supplied compilers and assemblers.")
#endif

void init_cplbs(void) {
  _disable_cplbs();  /* ensure CPLBs are disabled while configuring them */
  InitInstrCPLBs();
  InitDataCPLBs();
#if defined(ENABLE_DCACHE)
  *pDMEM_CONTROL |= (BITM_DMEM_CONTROL_ENCPLB | BITM_DMEM_CONTROL_CFG);
#else
  *pDMEM_CONTROL |= BITM_DMEM_CONTROL_ENCPLB;
#endif
  ssync();
#if defined(ENABLE_ICACHE)
  *pIMEM_CONTROL |= (BITM_IMEM_CONTROL_ENCPLB | BITM_IMEM_CONTROL_CFG);
#else
  *pIMEM_CONTROL |= BITM_IMEM_CONTROL_ENCPLB;
#endif
  ssync();
} /* init_cplbs */

static void InitDataCPLBs(void) {
  volatile uint32_t * dataptr = pDCPLB_DATA0;
  void * volatile *addrptr    = pDCPLB_ADDR0;
  int32_t idx = 0;

#if defined(ENABLE_DCACHE)
#if defined(ENABLE_DCACHE_WT)
  uint32_t cache_bits  = ENUM_DCPLB_DATA_WT | BITM_DCPLB_DATA_L1_CHBL | BITM_DCPLB_DATA_DIRTY;
#else 
  uint32_t cache_bits  = ENUM_DCPLB_DATA_WB | BITM_DCPLB_DATA_L1_CHBL | BITM_DCPLB_DATA_DIRTY;
#endif
#if defined(ENABLE_DCACHE2)
#if defined(ENABLE_DCACHE_WT)
  uint32_t cache2_bits = ENUM_DCPLB_DATA_WT | BITM_DCPLB_DATA_L1_CHBL | BITM_DCPLB_DATA_DIRTY;
#else 
  uint32_t cache2_bits = ENUM_DCPLB_DATA_WB | BITM_DCPLB_DATA_L1_CHBL | BITM_DCPLB_DATA_DIRTY;
#endif
#endif
#else
  uint32_t cache_bits  =                                                BITM_DCPLB_DATA_DIRTY;
  uint32_t cache2_bits =                                                BITM_DCPLB_DATA_DIRTY;
#endif
  uint32_t perms = BITM_DCPLB_DATA_UREAD | BITM_DCPLB_DATA_UWRITE | BITM_DCPLB_DATA_SWRITE;
  uint32_t valid = BITM_DCPLB_DATA_VALID;
  uint32_t cache_flags   = cache_bits  | perms | valid;
  uint32_t cache2_flags  = cache2_bits | perms | valid;
  uint32_t nocache_flags =               perms | valid;

  /* L1 Data Spaces */
#if !defined(CORE1)
  
  /* Core 0 L1 Data A */
  AddCPLB(0xFF800000, ENUM_DCPLB_DATA_16KB | nocache_flags);
#if defined(ENABLE_DCACHE)
  /* If cache is enabled, we'll leave the Cache/SRAM space free for cache */
  /* so we won't need a CPLB here. */
#else
  AddCPLB(0xFF804000, ENUM_DCPLB_DATA_16KB | nocache_flags);
#endif

  /* Core 0 L1 Data B */
  AddCPLB(0xFF900000, ENUM_DCPLB_DATA_16KB | nocache_flags);
#if defined(ENABLE_DCACHE2)
  /* If cache is enabled, we'll leave the Cache/SRAM space free for cache */
  /* so we won't need a CPLB here. */
#else
  AddCPLB(0xFF904000, ENUM_DCPLB_DATA_16KB | nocache_flags);
#endif

  /* Core 0 Scratchpad  has a built-in CPLB, so we don't need to provide one. */
#else /* !CORE1 */
  
  /* Core 1 L1 Data A */
  AddCPLB(0xFF400000, ENUM_DCPLB_DATA_16KB | nocache_flags);
#if defined(ENABLE_DCACHE)
  /* If cache is enabled, we'll leave the Cache/SRAM space free for cache */
  /* so we won't need a CPLB here. */
#else
  AddCPLB(0xFF404000, ENUM_DCPLB_DATA_16KB | nocache_flags);
#endif

  /* Core 1 L1 Data B */
  AddCPLB(0xFF500000, ENUM_DCPLB_DATA_16KB | nocache_flags);
#if defined(ENABLE_DCACHE)
  /* If cache is enabled, we'll leave the Cache/SRAM space free for cache */
  /* so we won't need a CPLB here. */
#else
  AddCPLB(0xFF504000, ENUM_DCPLB_DATA_16KB | nocache_flags);
#endif

  /* Core 1 Scratchpad has a built-in CPLB, so we don't need to provide one. */
#endif /* CORE1 */

  /* L2 RAM */

  /* First 64KB is ICC private areas and shared so uncached */
  AddCPLB(0xC8080000, ENUM_DCPLB_DATA_64KB | nocache_flags);

#if !defined(CORE1)
  /* 96 KB core 0 memory - cached */
  AddCPLB(0xC8090000, ENUM_DCPLB_DATA_64KB | cache_flags);
  AddCPLB(0xC80A0000, ENUM_DCPLB_DATA_16KB | cache_flags);
  AddCPLB(0xC80A4000, ENUM_DCPLB_DATA_16KB | cache_flags);
#if 0
  /* 96 KB core 1 memory - readonly */
  AddCPLB(0xC80A8000, ENUM_DCPLB_DATA_16KB | CPLB_READONLY_ACCESS);
  AddCPLB(0xC80AC000, ENUM_DCPLB_DATA_16KB | CPLB_READONLY_ACCESS);
  AddCPLB(0xC80B0000, ENUM_DCPLB_DATA_64KB | CPLB_READONLY_ACCESS);
#endif
#else
#if 0
  /* 96 KB core 0 memory - readonly */
  AddCPLB(0xC8090000, ENUM_DCPLB_DATA_64KB | CPLB_READONLY_ACCESS);
  AddCPLB(0xC80A0000, ENUM_DCPLB_DATA_16KB | CPLB_READONLY_ACCESS);
  AddCPLB(0xC80A4000, ENUM_DCPLB_DATA_16KB | CPLB_READONLY_ACCESS);
#endif
  /* 96 KB core 1 memory - cached */
  AddCPLB(0xC80A8000, ENUM_DCPLB_DATA_16KB | cache_flags);
  AddCPLB(0xC80AC000, ENUM_DCPLB_DATA_16KB | cache_flags);
  AddCPLB(0xC80B0000, ENUM_DCPLB_DATA_64KB | cache_flags);
#endif

  /* L2 ROM */
  AddCPLB(0xC8000000, ENUM_DCPLB_DATA_16KB | cache_flags);
  AddCPLB(0xC8004000, ENUM_DCPLB_DATA_16KB | cache_flags);

  /* Async Bank 0 */
  AddCPLB(0xB0000000, ENUM_DCPLB_DATA_64MB | cache_flags);

  /* External memory */
#if !defined(CORE1)
  AddCPLB(0x00000000, ENUM_DCPLB_DATA_16MB | cache_flags);
  AddCPLB(0x01000000, ENUM_DCPLB_DATA_16MB | nocache_flags);
  AddCPLB(0x02000000, ENUM_DCPLB_DATA_16MB | cache_flags);
#if 0
  AddCPLB(0x03000000, ENUM_DCPLB_DATA_16MB | cache_flags); /* for data access to code */
#endif
  AddCPLB(0x04000000, ENUM_DCPLB_DATA_16MB | cache_flags);
  AddCPLB(0x05000000, ENUM_DCPLB_DATA_16MB | nocache_flags);
#if 0
  AddCPLB(0x06000000, ENUM_DCPLB_DATA_16MB | CPLB_READONLY_ACCESS); /* core 1 owned */
  AddCPLB(0x07000000, ENUM_DCPLB_DATA_16MB | CPLB_READONLY_ACCESS); /* core 1 owned */
#endif
#else
  AddCPLB(0x00000000, ENUM_DCPLB_DATA_16MB | cache_flags);
  AddCPLB(0x01000000, ENUM_DCPLB_DATA_16MB | nocache_flags);
#if 0
  AddCPLB(0x02000000, ENUM_DCPLB_DATA_16MB | CPLB_READONLY_ACCESS); /* core 0 owned */
  AddCPLB(0x03000000, ENUM_DCPLB_DATA_16MB | CPLB_READONLY_ACCESS); /* core 0 owned */
#endif
  AddCPLB(0x04000000, ENUM_DCPLB_DATA_16MB | cache_flags);
  AddCPLB(0x05000000, ENUM_DCPLB_DATA_16MB | nocache_flags);
  AddCPLB(0x06000000, ENUM_DCPLB_DATA_16MB | cache_flags);
#if 0
  AddCPLB(0x07000000, ENUM_DCPLB_DATA_16MB | cache_flags); /* for data access to code */
#endif
#endif

}

static void InitInstrCPLBs(void) {
  volatile uint32_t *dataptr = pICPLB_DATA0;
  void * volatile *addrptr = pICPLB_ADDR0;
  int32_t idx = 0;

#if defined(ENABLE_ICACHE)
  uint32_t cache_bits = BITM_ICPLB_DATA_L1_CHBL;
#else
  uint32_t cache_bits = 0uL;
#endif
  uint32_t perms = BITM_ICPLB_DATA_UREAD;
  uint32_t valid = BITM_ICPLB_DATA_VALID;
  uint32_t cache_flags  =  cache_bits  | perms | valid;
  uint32_t nocache_flags =               perms | valid;

  /* L1 Instr Spaces */
#if !defined(CORE1)
  
  /* Core 0 L1 Instr */
  AddCPLB(0xFFA00000, ENUM_ICPLB_DATA_64KB | nocache_flags);
#if defined(ENABLE_ICACHE)
  /* If cache is enabled, we'll leave the Cache/SRAM space free for cache */
  /* so we won't need a CPLB here. */
#else
  AddCPLB(0xFFA10000, ENUM_ICPLB_DATA_16KB | nocache_flags);
#endif

#else  /* CORE1 */

  /* Core 1 L1 Instr */
  AddCPLB(0xFF600000, ENUM_ICPLB_DATA_64KB | nocache_flags);
#if defined(ENABLE_DCACHE)
  /* If cache is enabled, we'll leave the Cache/SRAM space free for cache */
  /* so we won't need a CPLB here */
#else
  AddCPLB(0xFF610000, ENUM_ICPLB_DATA_16KB | nocache_flags);
#endif

#endif /* CORE1 */

  /* L2 RAM */
  AddCPLB(0xC8080000, ENUM_ICPLB_DATA_64KB | nocache_flags);
  AddCPLB(0xC8090000, ENUM_ICPLB_DATA_64KB | cache_flags);
  AddCPLB(0xC80A0000, ENUM_ICPLB_DATA_64KB | cache_flags);
  AddCPLB(0xC80B0000, ENUM_ICPLB_DATA_64KB | cache_flags);

  /* L2 ROM */
  AddCPLB(0xC8000000, ENUM_ICPLB_DATA_16KB | cache_flags);
  AddCPLB(0xC8004000, ENUM_ICPLB_DATA_16KB | cache_flags);

  /* Async Bank 0 */
  AddCPLB(0xB0000000, ENUM_ICPLB_DATA_64MB | cache_flags);

  /* L3 - SDRAM */
  AddCPLB(0x00000000, ENUM_ICPLB_DATA_16MB | cache_flags);
  AddCPLB(0x01000000, ENUM_ICPLB_DATA_16MB | nocache_flags);
  /* 0x02000000 - core specific data */
  AddCPLB(0x03000000, ENUM_ICPLB_DATA_16MB | cache_flags);
  AddCPLB(0x04000000, ENUM_ICPLB_DATA_16MB | cache_flags);
  AddCPLB(0x05000000, ENUM_ICPLB_DATA_16MB | nocache_flags);
  /* 0x06000000 - core specific data */
  AddCPLB(0x06000000, ENUM_ICPLB_DATA_16MB | cache_flags);
  AddCPLB(0x07000000, ENUM_ICPLB_DATA_16MB | cache_flags);

}

#if defined(_MISRA_2004_RULES)
  #pragma diag(pop)
#endif

#else
/* suppress translation unit must contain at least one declaration warning */
#pragma diag(suppress : 96)
#endif /* CONFIG_CPLBS */
