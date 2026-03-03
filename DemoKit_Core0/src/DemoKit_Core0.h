/*****************************************************************************
 * DemoKit_Core0.h
 *****************************************************************************/

#ifndef __DEMOKIT_CORE0_H__
#define __DEMOKIT_CORE0_H__

#define SOFT_VERSION		"1.16.3"

#define MHZTOHZ       (1000000u)

#define CLKIN         (25u * MHZTOHZ)
#define CORE_MAX      (500u * MHZTOHZ)
#define SYSCLK_MAX    (250u * MHZTOHZ)
#define SCLK_MAX      (125u * MHZTOHZ)
#define DCLK_MAX      (250u * MHZTOHZ)
#define VCO_MIN       (72u * MHZTOHZ)

#define WD_CORE0								0
#define WD_TIMER_TIME                         	0x9502F900        	/* 10sek watchdog */
#define CTMR_CALLBACKS_MAX						10
#define CTMR_SCALE 								249u
#define CTMR_TIME 								12u
#define CTMR_PERIOD 							CTMR_TIME

#define BAUD_RATE           19200u
#define FAILED              (-1)
#define PASSED              0
#define REPORT_ERROR        printf
#define UART_DEVICE_NUM     0u

#endif /* __DEMOKIT_CORE0_H__ */
