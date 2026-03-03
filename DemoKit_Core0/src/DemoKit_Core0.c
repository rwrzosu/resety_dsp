/*****************************************************************************
 * DemoKit_Core0.c
 *****************************************************************************/

#include <sys/platform.h>
#include <sys/adi_core.h>
#include <ccblkfn.h>
#include <services/wd/adi_wd.h>
#include <services\pwr\adi_pwr.h>
#include <services/tmr/adi_ctmr.h>
#include <drivers\uart\adi_uart.h>
#include <stdarg.h>
#include "adi_initialize.h"
#include "DemoKit_Core0.h"

#define APP_NAME "C0_MAIN"

char CompilationDate[12] = __DATE__;
char CompilationTime[9] = __TIME__;

char __argv_string[] = "";

char tmp_tx_buff [256];

void UART_printf (const char *format, ...) {
	int ret;
	va_list args;


	va_start(args, format);
	int size = vsprintf( tmp_tx_buff , format, args);
	va_end(args);

	UART_Write_BL(tmp_tx_buff, size);

	return ;
}

int UART_Write_BL(char* buff, uint16_t size){
    while (size-- != 0){
        while ((*pREG_UART0_STAT & ENUM_UART_STAT_THR_EMPTY) == 0);
        *pREG_UART0_THR = *buff++;
    }
    return 0;
}


char WdgUartBuff[100];
void WDTimer_Callback(void *pCBParam,uint32_t Event,void *pArg)
{
    uint32_t wd_ctl = *pREG_WDOG0_CTL;
	*pREG_RCU0_CTL = 1;
	NOP();
}

ADI_WD_HANDLE  hWDTimer;
int WDTimer_Init(void)
{
    ADI_WD_RESULT eResult;

    if((eResult = adi_wd_Open(WD_CORE0, &hWDTimer, WDTimer_Callback, NULL)) != ADI_WD_SUCCESS) {
    	return (eResult);
    }

    if((eResult = adi_wd_SetCounter(hWDTimer, WD_TIMER_TIME)) != ADI_WD_SUCCESS) {
    	return (eResult);
    }

    if((eResult = adi_wd_Enable(hWDTimer, true)) != ADI_WD_SUCCESS) {
    	return (eResult);
    }

    adi_sec_EnableSystemReset(false);

    adi_sec_EnableFault (INTR_WDOG0_EXP, true);
    adi_sec_EnableInterrupt (INTR_WDOG0_EXP, false);        //przerwanie od watchdoga wylaczone dla C0, dla C1 wlaczone przy inicjalizacji handlera do tego przerwania
                                                            //zrobione tak w celach debugowych
    adi_sec_SetSysResetDelay(250000000);
    adi_sec_EnableTriggerOutput(true);

    adi_sec_EnableSFI(true);

	return 0;
}

int int_fun_count;
void (*pfCallbackFunction[CTMR_CALLBACKS_MAX])(void);

void CmtrCallback(void *pCBParam, uint32_t Event, void *pArg)
{
	uint16_t i;
	static uint16_t max_time;

	for (i=0; i < int_fun_count; i++) {
		if(pfCallbackFunction[i] != NULL)
			pfCallbackFunction[i]();
	}
}

#pragma align(4)
ADI_CTMR_HANDLE            hCTimer;

int CTMR_mgmt_IntRegister(void (*fun)(void))
{
	if (int_fun_count >= CTMR_CALLBACKS_MAX)
		return -1;

	uint32_t reg = cli();
	pfCallbackFunction[int_fun_count++] = fun;
	sti(reg);

	return 0;
}

int CTMR_mgmt_Init(void)
{
	 ADI_CTMR_RESULT result;

	 if ((result = adi_ctmr_Open (ADI_CTMR_DEV0, CmtrCallback, NULL, &hCTimer)) != ADI_CTMR_SUCCESS)
	 	 return result;

	 if ((result = adi_ctmr_SetScale (hCTimer, CTMR_SCALE)) != ADI_CTMR_SUCCESS)
 	 	 return result;

	 if ((result = adi_ctmr_SetPeriod (hCTimer, CTMR_PERIOD)) != ADI_CTMR_SUCCESS)
 	 	 return result;

	 if ((result = adi_ctmr_EnableAutoReload (hCTimer, true)) != ADI_CTMR_SUCCESS)
 	 	 return result;

	 if ((result = adi_ctmr_Activate (hCTimer, true)) != ADI_CTMR_SUCCESS)
 	 	 return result;

	 result = adi_ctmr_Enable (hCTimer, true);

 	 return (int)result;
}

uint8_t  gUARTMemory[ADI_UART_BIDIR_DMA_MEMORY_SIZE];
ADI_UART_HANDLE  ghUART;
volatile bool TxDone = 1;
static uint32_t Clk_Reg;
static uint32_t Ctl_Reg;
int UART_Hold_Regs(void)
{
    Clk_Reg = *pREG_UART0_CLK;
    Ctl_Reg = *pREG_UART0_CTL;
    return 0;
}

static void UARTCallback(void* pHandle, uint32_t u32Arg, void* pArg)
{
    ADI_UART_HANDLE pDevice = (ADI_UART_HANDLE)pHandle;
    ADI_UART_EVENT event    = (ADI_UART_EVENT)u32Arg;
    uint16_t *data = (uint16_t*)pArg;

    switch (event) {
        case ADI_UART_EVENT_TX_BUFFER_PROCESSED:
        case ADI_UART_EVENT_RX_BUFFER_PROCESSED:
        case ADI_UART_EVENT_OVERRUN_ERROR:
        case ADI_UART_EVENT_PARITY_ERROR:
        case ADI_UART_EVENT_FRAMING_ERROR:
        case ADI_UART_EVENT_BREAK_INTERRUPT:
        case ADI_UART_EVENT_RX_FIFO_WM:
        case ADI_UART_EVENT_RECIEVED_ADDR:
        case ADI_UART_EVENT_AUTOBAUD_COMPLETE:
            break;
        case ADI_UART_EVENT_TX_COMPLETE:
            TxDone = 1;
            break;
        case ADI_UART_EVENT_RX_DMA_ERROR:
        case ADI_UART_EVENT_TX_DMA_ERROR:
            // Add code to handle these events
            asm("nop;");
            break;
    default:
        break;
    }
}

int UART_Init(void)
{
	ADI_UART_RESULT    eResult;

    _Bool bStopFlag = false;
    if((eResult = adi_uart_Open(UART_DEVICE_NUM,
                                 ADI_UART_DIR_BIDIRECTION,
                                 gUARTMemory,
                                 ADI_UART_BIDIR_DMA_MEMORY_SIZE,
                                 &ghUART)) != ADI_UART_SUCCESS)
    {
    	REPORT_ERROR("Could not open UART Device 0x%08X \n", eResult);
    	return FAILED;
    }
    /* Set the UART Mode */
    if((eResult = adi_uart_SetMode(ghUART,
                                    ADI_UART_MODE_UART
                                   )) != ADI_UART_SUCCESS)
    {
    	REPORT_ERROR("Could not set the Mode 0x%08X \n", eResult);
    	return FAILED;
    }


    /* Set UART Baud Rate */
    if((eResult = adi_uart_SetBaudRate(ghUART,
    									BAUD_RATE
                                        )) != ADI_UART_SUCCESS)
    {
    	REPORT_ERROR("Could not set the Baud Rate 0x%08X \n", eResult);
    	return FAILED;
    }

    /* Set UART parity */

    if((eResult = adi_uart_SetParity(ghUART,
                                        ADI_UART_NO_PARITY
                                       )) != ADI_UART_SUCCESS)
     {
         REPORT_ERROR("Could not set the Baud Rate 0x%08X \n", eResult);
         return FAILED;
     }

    /* Set number of stop bits */
    if((eResult = adi_uart_SetNumStopBits(ghUART,
                                            ADI_UART_ONE_STOPBIT
                                         )) != ADI_UART_SUCCESS)
    {
    	REPORT_ERROR("Could not set the stop bits 0x%08X \n", eResult);
    	return FAILED;
    }

    /* Set number of stop bits */
    if((eResult = adi_uart_SetWordLen(ghUART,
                                          ADI_UART_WORDLEN_8BITS
                                         )) != ADI_UART_SUCCESS)
    {
    	REPORT_ERROR("Could not set word length 0x%08X \n", eResult);
    	return FAILED;
    }
/*
    if((eResult = adi_uart_SubmitTxBuffer  ( ghUART, buff, nTxSize) ) != ADI_UART_SUCCESS) {
            REPORT_ERROR("Could not do a SubmitTxBuffer 0x%08X \n", eResult);
            return FAILED;
    }
*/


    /* register the callback function */
    if((eResult = adi_uart_RegisterCallback(ghUART, UARTCallback, (void*)0)) != ADI_UART_SUCCESS)
    {
        REPORT_ERROR("Could not register callback 0x%08X \n", eResult);
        return FAILED;
    }


    if((eResult = adi_uart_EnableDMAMode  ( ghUART,	true )) != ADI_UART_SUCCESS) {
//    if((eResult = adi_uart_EnableDMAMode  ( ghUART, false )) != ADI_UART_SUCCESS) {
		REPORT_ERROR("Could not set DMA mode 0x%08X \n", eResult);
		return FAILED;
	}

    uint8_t buffer[] = {0};

    if(adi_uart_SubmitTxBuffer (ghUART, buffer, sizeof(buffer))  != ADI_UART_SUCCESS)
        return FAILED;

    eResult = adi_uart_EnableTx(ghUART, true);

    UART_Hold_Regs();

    return  PASSED;
}

void TimeoutInterrupt (void){
	uint32_t* ppp = 0xc8091a50;

	if (*ppp > 1)
		(*ppp)--;

}

extern void*  hWDTimer;

int main(int argc, char *argv[])
{
	adi_initComponents();

	if(adi_pwr_Init(CLKIN, CORE_MAX, SYSCLK_MAX, VCO_MIN) != ADI_PWR_SUCCESS){
		UART_printf("adi_pwr_Init fail \n");
	}

	WDTimer_Init();

	if (CTMR_mgmt_Init() != 0)
		UART_printf("CTMR_mgmt_Init fail \n");

	if (UART_Init() != 0)
		UART_printf("UART_Init fail \n");

	int ret = CTMR_mgmt_IntRegister(TimeoutInterrupt);

	adi_core_1_enable();

    UART_printf("DSP SOFT VERSION: %s LAST COMPILED: %s %s\r\n", SOFT_VERSION, CompilationDate, CompilationTime);
	UART_printf("# while (1)\r\n");
	int licznik=100000;
	while (1) {
		static int16_t bus = 0;
		bus = bus==0 ? 1 : 0;
		int16_t *res_data_ptr = bus==0 ? 0x4ea11f4 : 0x50dd95c;

		memcpy(0xa005908 ,  res_data_ptr, 512 * sizeof(int16_t));

		adi_wd_Reset(hWDTimer, WD_TIMER_TIME);
		if(--licznik==0){
//			UART_printf("C0\r\n");
			licznik = 10000;
		}
	}
}

