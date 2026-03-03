/*
 * system.h
 *
 * ADSP-BF60x system
 *
 * File Version 2.2.0.1
 *
 * Copyright (c) 2011-2015 Analog Devices, Inc. All Rights Reserved.
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#include "init_platform.h"
#include <sys/platform.h>
#include <bfrom.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*****************************************************************************
 Prototypes
******************************************************************************/

uint32_t get_pllclk_hz(uint32_t ulRegCgu0Ctl);
uint32_t get_pllclk_ns(uint32_t ulRegCgu0Ctl);
uint32_t get_cclk_hz(void);
uint32_t get_cclk_ns(void);
uint32_t get_sysclk_hz(uint32_t ulRegCgu0Ctl, uint32_t ulRegCgu0Stat, uint32_t ulRegCgu0Div);
uint32_t get_s0clk_hz(void);
uint32_t get_s1clk_hz(void);
uint32_t get_ddrclk_hz(void);
uint32_t get_ddrclk_ns(void);

#pragma noreturn
void ErrorWithNoReturn(void);

#endif /* SYSTEM_H */

