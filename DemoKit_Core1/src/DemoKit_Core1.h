/*****************************************************************************
 * DemoKit_Core1.h
 *****************************************************************************/

#ifndef __DEMOKIT_CORE1_H__
#define __DEMOKIT_CORE1_H__


#define PQ_FFT_NUMPOINTS            2048 /*1024*/
#define TEN_PERIODS_DATA_BUFFERS_NO  4
#define PQ_MAX_MODULES_NR			2
#define PQ_V_PHASE_NUMBER           4
#define PQ_I_PHASE_NUMBER           4
#define PQ_VI_PHASE_NUMBER          ( PQ_V_PHASE_NUMBER + PQ_I_PHASE_NUMBER )

#define PQ_INPUT_CHANNELS_PER_UNIT	PQ_VI_PHASE_NUMBER
#define PQ_TRIG_SIG_PER_UNIT		PQ_VI_PHASE_NUMBER

#define PQ_SAMPLES_NUMBER			256
#define PQ_CALC_PERIODS_NUMBER		10
#define PQ_SAMPLES_NUMBERx10		(PQ_SAMPLES_NUMBER * PQ_CALC_PERIODS_NUMBER)
#define PQ_NUMPOINTSx10				PQ_SAMPLES_NUMBERx10

#pragma pack (2)
typedef struct {
	uint32_t precision;		// sekundy po przecinku
	struct {
		uint16_t sec0;
		uint16_t sec1;
		uint16_t sec2;
	} seconds;
} PPI_TIME_t;

typedef struct {
	uint8_t 		Bus;
	PPI_TIME_t		Time;
	uint64_t		Step;
	int16_t 		*Channels[8];
} PQ_BUS_DATA_MCAPI_MESSAGE_t;

typedef struct {
	uint8_t 		BusNo;
	uint16_t		DataSize;
	PQ_BUS_DATA_MCAPI_MESSAGE_t BusData[PQ_MAX_MODULES_NR];

} PQ_DATA_MCAPI_MESSAGE_t;

typedef struct
{
    fract16     	samples[PQ_INPUT_CHANNELS_PER_UNIT][PQ_NUMPOINTSx10];
    uint64_t     	timeStamp[PQ_CALC_PERIODS_NUMBER];
    uint64_t     	timeStep[PQ_CALC_PERIODS_NUMBER];
    uint8_t			periods_cnt;										// licznik okresow dla danego bufora
    uint8_t			overlapPeriod;										// liczba kresow nakladki (dla nastepnej 200milisekundowki po overlapHappen) (
    bool			overlapHappen;										// 1 - oznacza ze dla danej 200ms wystapilo przejscie pelnej 10-cio minutowki
    bool            one_min_expired;                                    // 1 - oznacza ze zostala przekroczona pelna minuta
} PQ_DATA_BUFFERS_t;

#endif /* __DEMOKIT_CORE1_H__ */
