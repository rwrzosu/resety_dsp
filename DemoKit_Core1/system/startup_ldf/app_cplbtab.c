/*
** ADSP-BF609 CPLB table definitions for 1 generated on Nov 24, 2025 at 13:27:08.
*/
/*
** Copyright (C) 2000-2021 Analog Devices Inc., All Rights Reserved.
**
** This file is generated automatically based upon the options selected
** in the System Configuration utility. Changes to the CPLB configuration
** should be made by modifying the appropriate options rather than editing
** this file. To access the System Configuration utility, double-click the
** system.svc file from a navigation view.
**
** Custom additions can be inserted within the user-modifiable sections. These
** sections are bounded by comments that start with "$VDSG". Only changes
** placed within these sections are preserved when this file is re-generated.
**
** Product      : CrossCore Embedded Studio
** Tool Version : 6.2.2.7
*/

#include <sys/platform.h>
#include <cplbtab.h>
#include <cplb.h>

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_2_2)
#pragma diag(suppress:misra_rule_8_10)
#pragma diag(suppress:misra_rule_10_1_a)
#endif /* _MISRA_RULES */

#define CACHE_MEM_MODE (CPLB_DDOCACHE_WB)

#define NOCACHE_READONLY (CPLB_READONLY_ACCESS)

#pragma section("cplb_data")
#pragma file_attr("DualCoreMem=CoreB")

cplb_entry dcplbs_table[] = {


   /*
   ** L1 data memory must be covered by a locked CPLB as it will contain
   ** the tables used in interrupt and exceptions handling support.
   ** For L1 Data A & B, set the write-through bit to avoid 1st write exceptions.
   */

   {0xFF400000, (ENUM_DCPLB_DATA_16KB | CPLB_DNOCACHE | BITM_DCPLB_DATA_LOCK | BITM_DCPLB_DATA_WT)}, 
   {0xFF404000, (ENUM_DCPLB_DATA_16KB | CPLB_DNOCACHE | BITM_DCPLB_DATA_LOCK | BITM_DCPLB_DATA_WT)}, 
   {0xFF500000, (ENUM_DCPLB_DATA_16KB | CPLB_DNOCACHE | BITM_DCPLB_DATA_LOCK | BITM_DCPLB_DATA_WT)}, 
   {0xFF504000, (ENUM_DCPLB_DATA_16KB | CPLB_DNOCACHE | BITM_DCPLB_DATA_LOCK | BITM_DCPLB_DATA_WT)}, 

   /*
   ** 256K L2 SRAM is split as follows:
   **
   **   ICC private area             - (0xC8080000-0xC808041F) 1056B
   **   Shareable, owned by 0        - (0xC8080420-0xC808820F) 32240B
   **   Shareable, owned by 1        - (0xC8088210-0xC808FFFF) 32240B
   **   Core 0                       - (0xC8090000-0xC80A7FFF) 96KB
   **   Core 1                       - (0xC80A8000-0xC80BFFFF) 96KB
   */

   {0xC8080000, (ENUM_DCPLB_DATA_64KB | CPLB_DNOCACHE)}, 
   {0xC8090000, (ENUM_DCPLB_DATA_64KB | CPLB_READONLY_ACCESS)}, 
   {0xC80A0000, (ENUM_DCPLB_DATA_16KB | CPLB_READONLY_ACCESS)}, 
   {0xC80A4000, (ENUM_DCPLB_DATA_16KB | CPLB_READONLY_ACCESS)}, 
   {0xC80A8000, (ENUM_DCPLB_DATA_16KB | CACHE_MEM_MODE)}, 
   {0xC80AC000, (ENUM_DCPLB_DATA_16KB | CACHE_MEM_MODE)}, 
   {0xC80B0000, (ENUM_DCPLB_DATA_64KB | CACHE_MEM_MODE)}, 

   /* 
   ** Configuration for 256 MB SDRAM (DDR2).
   **
   ** This SDRAM memory is split into two, one half for each core, as follows:
   **   Cached shareable       - 32MB (SDRAM_BANK0)
   **   Shareable              - 32MB (SDRAM_BANK1)
   **   Core specific data     - 32MB (SDRAM_BANK2)
   **   Core specific code     - 32MB (SDRAM_BANK3)
   */

   {0x08000000, (ENUM_DCPLB_DATA_16MB | CACHE_MEM_MODE)}, 
   {0x09000000, (ENUM_DCPLB_DATA_16MB | CACHE_MEM_MODE)}, 
   {0x0a000000, (ENUM_DCPLB_DATA_16MB | CPLB_DNOCACHE)}, 
   {0x0b000000, (ENUM_DCPLB_DATA_16MB | CPLB_DNOCACHE)}, 
   {0x0c000000, (ENUM_DCPLB_DATA_16MB | CACHE_MEM_MODE)}, 
   {0x0d000000, (ENUM_DCPLB_DATA_16MB | CACHE_MEM_MODE)}, 
   {0x0e000000, (ENUM_DCPLB_DATA_16MB | CACHE_MEM_MODE)}, 
   {0x0f000000, (ENUM_DCPLB_DATA_16MB | CACHE_MEM_MODE)}, 
   {0x00000000, (ENUM_DCPLB_DATA_16MB | CACHE_MEM_MODE)}, 
   {0x01000000, (ENUM_DCPLB_DATA_16MB | CACHE_MEM_MODE)}, 
   {0x02000000, (ENUM_DCPLB_DATA_16MB | CPLB_DNOCACHE)}, 
   {0x03000000, (ENUM_DCPLB_DATA_16MB | CPLB_DNOCACHE)}, 
   {0x04000000, (ENUM_DCPLB_DATA_16MB | CPLB_READONLY_ACCESS)}, 
   {0x05000000, (ENUM_DCPLB_DATA_16MB | CPLB_READONLY_ACCESS)}, 
   {0x06000000, (ENUM_DCPLB_DATA_16MB | CPLB_READONLY_ACCESS)}, 
   {0x07000000, (ENUM_DCPLB_DATA_16MB | CPLB_READONLY_ACCESS)}, 

     /* Async Memory */
   {0xBC000000, (ENUM_DCPLB_DATA_64MB | CACHE_MEM_MODE)}, 
   {0xB8000000, (ENUM_DCPLB_DATA_64MB | CACHE_MEM_MODE)}, 
   {0xB4000000, (ENUM_DCPLB_DATA_64MB | CACHE_MEM_MODE)}, 
   {0xB0000000, (ENUM_DCPLB_DATA_64MB | CACHE_MEM_MODE)}, 

     /* L2 ROM */
   {0xC8000000, (ENUM_DCPLB_DATA_16KB | CPLB_DNOCACHE)}, 
   {0xC8004000, (ENUM_DCPLB_DATA_16KB | CPLB_DNOCACHE)}, 

   /* end of section - termination */
   {0xffffffff, 0}, 
}; /* dcplbs_table */

#pragma section("cplb_data")
#pragma file_attr("DualCoreMem=CoreB")

cplb_entry icplbs_table[] = {


     /* L1 Code SRAM */
   {0xFF600000, (ENUM_ICPLB_DATA_64KB | CPLB_I_PAGE_MGMT)}, 

     /* L1 Code cache */
   {0xFF610000, (ENUM_ICPLB_DATA_16KB | CPLB_I_PAGE_MGMT)}, 

     /* L2 ROM */
   {0xC8000000, (ENUM_ICPLB_DATA_16KB | CPLB_I_PAGE_MGMT)}, 
   {0xC8004000, (ENUM_ICPLB_DATA_16KB | CPLB_I_PAGE_MGMT)}, 

     /* L2 SRAM */
   {0xC8080000, (ENUM_ICPLB_DATA_64KB | CPLB_INOCACHE)}, 
   {0xC8090000, (ENUM_ICPLB_DATA_64KB | CPLB_IDOCACHE)}, 
   {0xC80A0000, (ENUM_ICPLB_DATA_64KB | CPLB_IDOCACHE)}, 
   {0xC80B0000, (ENUM_ICPLB_DATA_64KB | CPLB_IDOCACHE)}, 

     /* 256 MB SDRAM memory space */
   {0x00000000, (ENUM_ICPLB_DATA_16MB | CPLB_IDOCACHE)}, 
   {0x01000000, (ENUM_ICPLB_DATA_16MB | CPLB_IDOCACHE)}, 
   {0x02000000, (ENUM_ICPLB_DATA_16MB | CPLB_INOCACHE)}, 
   {0x03000000, (ENUM_ICPLB_DATA_16MB | CPLB_INOCACHE)}, 
   {0x04000000, (ENUM_ICPLB_DATA_64MB | CPLB_IDOCACHE)}, 
   {0x08000000, (ENUM_ICPLB_DATA_16MB | CPLB_IDOCACHE)}, 
   {0x09000000, (ENUM_ICPLB_DATA_16MB | CPLB_IDOCACHE)}, 
   {0x0a000000, (ENUM_ICPLB_DATA_16MB | CPLB_INOCACHE)}, 
   {0x0b000000, (ENUM_ICPLB_DATA_16MB | CPLB_INOCACHE)}, 
   {0x0c000000, (ENUM_ICPLB_DATA_64MB | CPLB_IDOCACHE)}, 

     /* Async Memory */
   {0xBC000000, (ENUM_ICPLB_DATA_64MB | CPLB_IDOCACHE)}, 
   {0xB8000000, (ENUM_ICPLB_DATA_64MB | CPLB_IDOCACHE)}, 
   {0xB4000000, (ENUM_ICPLB_DATA_64MB | CPLB_IDOCACHE)}, 
   {0xB0000000, (ENUM_ICPLB_DATA_64MB | CPLB_IDOCACHE)}, 

   /* end of section - termination */
   {0xffffffff, 0}, 
}; /* icplbs_table */


#ifdef _MISRA_RULES
#pragma diag(pop)
#endif /* _MISRA_RULES */

