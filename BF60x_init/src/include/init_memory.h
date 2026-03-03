/*
 * init_memory.h
 *
 * Initialize DDR2 SDRAM memory programmatically.
 *
 * Note that DMC is the acronym for the Dynamic Memory Controller.
 *
 * File Version 2.2.0.0
 *
 * Copyright (c) 2011-2015 Analog Devices, Inc. All Rights Reserved.
 */

#ifndef INIT_MEMORY_H
#define INIT_MEMORY_H

#include "init_platform.h"

#if defined(CONFIG_MEMORY)

#include <sys/platform.h>
#include <stdint.h>
#include <builtins.h>



/*  For the case,  tDD2CLK = 1/250 MHz = 4ns */

#define CAS_LATENCY     ENUM_DMC_MR_CL4 	/* CAS Latency = 4 */
#define T_RCD           4uL             	/* tRCD = 12.5/4=  4 (3.125) */
#define T_RP            4uL                 /* tRP = 12.5/4 = 4 (3.125) */
#define T_RC            15uL	            /* tRC = 57.5/4 = 15 (14.375) */
#define T_RRD           3uL	                /* tRRD = 10/4 = 3 (2.5) */
#define T_RAS           12uL                /* tRAS = 45/4 = 12 (11.25) */
#define T_RFC           49uL                /* tRFC = 195/4 = 49 (48.75) = 0x31 */
#define T_FAW           12uL                /* tFAW = 45/4 =  12 (11.25) */
#define T_RTP           5uL                 /* tRTP = 7.5/4 = 2(1.85) // Keep it 5 ONLY */
#define T_WR            4uL                 /* tWR = 15/4 = 4 (3.75) */
#define T_WTR           2uL                 /* tWTR = 7.5/4 = 2 (1.85) */
#define T_CKE           3uL                 /* tCKE = 3 */
#define T_MRD           2uL                 /* tMRD = 2 */
#define T_XP            2uL                 /* tXP = 2 */
#define T_REF           959uL               /* dla tREFI = 3.9 us (industrial) */
                        /*0x78e*/           /* dla tREFI = 7.8 us (commercial)
                                               RDIV = ((f DDCLK X t REFI)) - (t RAS + t RP)
                                               RDIV = (250 X 10^6 x 7.8 x 10^-6) - (12 + 4)
                                               RDIV = 1950-16 = 1934 = 0x78e */

/*  For the case,  tDD2CLK = 1/166 MHz = 6ns */
//#define CAS_LATENCY     ENUM_DMC_MR_CL4   /* CAS Latency = 4 */
//#define T_RCD           3uL               /* tRCD */
//#define T_RP            3uL               /* tRP  */
//#define T_RC            10uL              /* tRC  */
//#define T_RRD           2uL               /* tRRD  */
//#define T_RAS           8uL               /* tRAS  */
//#define T_RFC           33uL              /* tRFC  */
//#define T_FAW           8uL               /* tFAW  */
//#define T_RTP           2uL               /* tRTP  // Keep it 5 ONLY */
//#define T_WR            3uL               /* tWR  */
//#define T_WTR           2uL               /* tWTR  */
//#define T_CKE           3uL               /* tCKE  */
//#define T_MRD           2uL               /* tMRD  */
//#define T_XP            2uL               /* tXP  */
//#define T_REF           638uL             /* dla tREFI = 3.9 us (industrial) */


#define DLL_LOCK_PERIOD 4500

/* CFG bits */
#define DMC_CFG_IFWID             (BITM_DMC_CFG_IFWID   & (ENUM_DMC_CFG_IFWID16))      /* Interface Width is 16-bits */
#define DMC_CFG_SDRSIZE           (BITM_DMC_CFG_SDRSIZE & (ENUM_DMC_CFG_SDRSIZE2G))    /* Individual SDRAM is 1Gbit */
#define DMC_CFG_SDRWID            (BITM_DMC_CFG_SDRWID  & (ENUM_DMC_CFG_SDRWID16))     /* Individual SDRAM is 16-bits wide */
#define DMC_CFG_EXTBANK           (BITM_DMC_CFG_EXTBANK & (ENUM_DMC_CFG_EXTBANK1))     /* 1 external bank */

#define DMC_CFG                   (DMC_CFG_IFWID | DMC_CFG_SDRSIZE | DMC_CFG_SDRWID | DMC_CFG_EXTBANK)

/* TR0 bits */
#define DMC_TR0_TRCD              (BITM_DMC_TR0_TRCD & (T_RCD << BITP_DMC_TR0_TRCD))
#define DMC_TR0_TWTR              (BITM_DMC_TR0_TWTR & (T_WTR << BITP_DMC_TR0_TWTR))
#define DMC_TR0_TRP               (BITM_DMC_TR0_TRP  & (T_RP << BITP_DMC_TR0_TRP))
#define DMC_TR0_TRAS              (BITM_DMC_TR0_TRAS & (T_RAS << BITP_DMC_TR0_TRAS))
#define DMC_TR0_TRC               (BITM_DMC_TR0_TRC  & (T_RC << BITP_DMC_TR0_TRC))
#define DMC_TR0_TMRD              (BITM_DMC_TR0_TMRD & (T_MRD << BITP_DMC_TR0_TMRD))

#define DMC_TR0                   (DMC_TR0_TRCD | DMC_TR0_TWTR | DMC_TR0_TRP | DMC_TR0_TRAS | DMC_TR0_TRC | DMC_TR0_TMRD)

/* TR1 bits */
#define DMC_TR1_TRRD              (BITM_DMC_TR1_TRRD & (T_RRD << BITP_DMC_TR1_TRRD))
#define DMC_TR1_TRFC              (BITM_DMC_TR1_TRFC & (T_RFC << BITP_DMC_TR1_TRFC))
#define DMC_TR1_TREFVAL           (BITM_DMC_TR1_TREF & (T_REF << BITP_DMC_TR1_TREF))

#define DMC_TR1                   (DMC_TR1_TRRD | DMC_TR1_TRFC | DMC_TR1_TREFVAL)

/* TR2 bits */
#define DMC_TR2_TCKE              (BITM_DMC_TR2_TCKE & (T_CKE << BITP_DMC_TR2_TCKE))
#define DMC_TR2_TXP               (BITM_DMC_TR2_TXP  & (T_XP << BITP_DMC_TR2_TXP))
#define DMC_TR2_TRTP              (BITM_DMC_TR2_TRTP & (T_RTP << BITP_DMC_TR2_TRTP))
#define DMC_TR2_TFAW              (BITM_DMC_TR2_TFAW & (T_FAW << BITP_DMC_TR2_TFAW))

#define DMC_TR2                   (DMC_TR2_TCKE | DMC_TR2_TXP | DMC_TR2_TRTP | DMC_TR2_TFAW)

/* MR bits */
#define DMC_MR_WRRECOV            (BITM_DMC_MR_WRRECOV & (T_WR << BITP_DMC_MR_WRRECOV))
#define DMC_MR_CL                 (BITM_DMC_MR_CL      & (CAS_LATENCY))            /*  clock cycle latency (DMC2) */
#define DMC_MR_BLEN               (BITM_DMC_MR_BLEN    & (ENUM_DMC_MR_BLEN4))          /* Burst Length 4 */

#define DMC_MR                    (DMC_MR_WRRECOV | DMC_MR_CL | DMC_MR_BLEN)

/* EMR1 bits */
#define DMC_EMR1_AL               (BITM_DMC_EMR1_AL & (0uL << BITP_DMC_EMR1_AL))

#define DMC_EMR1                  (ADI_DMC_EMR1_AL0)

/* PHY_CTL1 bits */
#define DMC_PHY_CTL1_ODT          (BITM_DMC_PHY_CTL1_CONTODTVAL & (ENUM_DMC_PHY_CTL1_ODT_75))

#define DMC_PHY_CTL1              (DMC_PHY_CTL1_ODT)

/* PHY_CTL3 bits */
#define DMC_PHY_CTL3              (BITM_DMC_PHY_CTL3_ENODTDQ | BITM_DMC_PHY_CTL3_TMG0 | BITM_DMC_PHY_CTL3_TMG1 | BITM_DMC_PHY_CTL3_OFST0 | BITM_DMC_PHY_CTL3_OFST1)

/* CTL bits */
#define DMC_CTL_RDTOWR            (BITM_DMC_CTL_RDTOWR & (ENUM_DMC_CTL_RDTOWR4))       /* Four extra cycles added */

#define DMC_CTL                   (BITM_DMC_CTL_INIT | DMC_CTL_RDTOWR)

#endif /* CONFIG_MEMORY */

#endif /* INIT_MEMORY_H */
