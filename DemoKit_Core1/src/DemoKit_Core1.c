/*****************************************************************************
 * DemoKit_Core1.c
 *****************************************************************************/

#include <sys/platform.h>
#include <sys/adi_core.h>
#include <ccblkfn.h>
#include <services/int/adi_sec.h>
#include <sysreg.h>
#include "adi_initialize.h"
#include "DemoKit_Core1.h"
#include "UART_c1.h"

section ("end_of_uncached_L2_sram")
uint32_t degub_c1[20];


static void WdExpHandler(uint32_t iid, void* handlerArg)
{
	UART_printf("C1 Watchdog expired\n\r");
	UART_printf("C1 DMC0_CTL = 0x%X\n\r", *pREG_DMC0_CTL);
	UART_printf("C1 DMC0_STAT = 0x%X\n\r", *pREG_DMC0_STAT);
	UART_printf("C1 reg_RETI %x\n\r", sysreg_read (reg_RETI));

	/* Check for DMA IDLE */
	UART_printf("BITM_DMC_STAT_IDLE checking ...\n\r");
	int16_t cnt = 10000;
	while(!(*pREG_DMC0_STAT & BITM_DMC_STAT_IDLE) && --cnt > 0)
		NOP();
	UART_printf("BITM_DMC_STAT_IDLE done\n\r");
	if(cnt > 0)
		UART_printf("BITM_DMC_STAT_IDLE NOT BUSY\n\r");
	else
		UART_printf("BITM_DMC_STAT_IDLE BUSY\n\r");

	*pREG_TRU0_MTR = (uint32_t)TRGM_SOFT0;
	while(1);
}

static void NmiC1Handler(uint32_t iid, void *pCBParam)
{
    UART_printf ("\n\rCORE1 NMI handler\n\r");
}


complex_fract32 w32[PQ_FFT_NUMPOINTS];

fract32 g_chx[PQ_FFT_NUMPOINTS];

complex_fract32 g_chx_o[PQ_FFT_NUMPOINTS];

typedef PQ_DATA_BUFFERS_t data_out_layout_t[PQ_MAX_MODULES_NR][TEN_PERIODS_DATA_BUFFERS_NO];
static data_out_layout_t *data_out = (data_out_layout_t*)0xc000000;

uint8_t curr_buffer[PQ_MAX_MODULES_NR] = {0,0};

int main(int argc, char *argv[])
{
	adi_initComponents();

	UART_printf("start C1\r\n");

    adi_sec_EnableNMI  ( ADI_SEC_CORE_1, false);
    adi_sec_ClearNMIStatus (ADI_SEC_CORE_1);


	//test watchdoga
	adi_sec_SetCoreID(INTR_WDOG0_EXP, ADI_SEC_CORE_1);
	adi_int_InstallHandler(INTR_WDOG0_EXP, WdExpHandler, NULL, true);
	adi_int_InstallHandler( ADI_CID_NMI, NmiC1Handler, NULL, true);

	int be;
	int wst32 = 1;
	PQ_DATA_MCAPI_MESSAGE_t *data_in = 0xff4037f0;
	UART_printf("# while (1) C1\r\n");
	int licznik=100000;
	while (1) {
		for (int current_period = 0; current_period < 2; current_period++) {
			for (int bus = 0; bus < 2; bus++) {

				PQ_DATA_BUFFERS_t *d_out_curr = &(*data_out)[bus][curr_buffer[bus]];
				for (int channel = 0; channel < 8; channel++) {
					degub_c1[0] = 12;

					memcpy (&d_out_curr->samples[channel][256 * d_out_curr->periods_cnt], data_in->BusData[bus].Channels[channel] + (256 * current_period), sizeof(int16_t) * 256);

					degub_c1[0] = 13;
				}
				if (++d_out_curr->periods_cnt == 10) {
					(*data_out)[bus][curr_buffer[bus]].periods_cnt = 0;
				}
			}
		}
		if(--licznik==0){
			UART_printf("C1\r\n");
			licznik = 100000;
		}
	}
}

