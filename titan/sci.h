#ifndef SCI_H
#define SCI_H

#define SCI_CLASS_A			1   //only 5V Vcc to Smart Card
#define SCI_CLASS_B			2   //only 3V Vcc to Smart Card
#define SCI_CLASS_AB			3   //5V or 3V Vcc to Smart Card
#define SCI_NUMBER_OF_CONTROLLERS	2   //number of SCI controllers

#define SCI_BUFFER_SIZE			512

#define SCI_CLOCK_STOP_DISABLED		0
#define SCI_CLOCK_STOP_LOW		1
#define SCI_CLOCK_STOP_HIGH		2

#define SCI_MAX_ATR_SIZE		33

#define SCI_MAX_F			80000000
#define SCI_MAX_ETU			0xFFF
#define SCI_MAX_WWT			0xFFFFFFFF
#define SCI_MAX_CWT			0xFFFF
#define SCI_MAX_BWT			0xFFFFFFFF
#define SCI_MAX_EGT			0xFF

#define SCI_MIN_F			1000000
#define SCI_MIN_ETU			8
#define SCI_MIN_WWT			12
#define SCI_MIN_CWT			12
#define SCI_MIN_BWT			971
#define SCI_MIN_EGT			0

#define SCI_SYNC			0x00000001
#define SCI_DATA_ANY			0x00000002

//Reserved for Future Use defined as 0
#define RFU				0

typedef enum
{
	SCI_ERROR_OK = 0,
	SCI_ERROR_DRIVER_NOT_INITIALIZED = -1691,
	SCI_ERROR_FAIL,
	SCI_ERROR_KERNEL_FAIL,
	SCI_ERROR_NO_ATR,
	SCI_ERROR_TS_CHARACTER_INVALID,
	SCI_ERROR_LRC_FAIL,
	SCI_ERROR_CRC_FAIL,
	SCI_ERROR_LENGTH_FAIL,
	SCI_ERROR_PARITY_FAIL,
	SCI_ERROR_RX_OVERFLOW_FAIL,
	SCI_ERROR_TX_OVERFLOW_FAIL,
	SCI_ERROR_TX_UNDERRUN_FAIL,
	SCI_ERROR_CARD_NOT_PRESENT,
	SCI_ERROR_CARD_NOT_ACTIVATED,
	SCI_ERROR_AWT_TIMEOUT,
	SCI_ERROR_WWT_TIMEOUT,
	SCI_ERROR_CWT_TIMEOUT,
	SCI_ERROR_BWT_TIMEOUT,
	SCI_ERROR_PARAMETER_OUT_OF_RANGE,
	SCI_ERROR_TRANSACTION_ABORTED,
	SCI_ERROR_CLOCK_STOP_DISABLED,
	SCI_ERROR_TX_PENDING,
	SCI_ERROR_ATR_PENDING
} SCI_ERROR;

//SCI driver modes
typedef struct sci_modes
{
	int32_t emv2000;
	int32_t dma;
	int32_t man_act;
	int32_t rw_mode;
} SCI_MODES;

//SCI communication parameters
typedef struct sci_parameters
{
	unsigned char T;
	unsigned char fs;
	unsigned char ETU;
	unsigned char WWT;
	unsigned char CWT;
	unsigned char BWT;
	unsigned char EGT;
	unsigned char clock_stop_polarity;
	unsigned char check;
	unsigned char P;
	unsigned char I;
	unsigned char U;
} SCI_PARAMETERS;

//SCI ATR status
typedef enum
{
	SCI_WITHOUT_ATR = 0,
	SCI_ATR_READY
} SCI_ATR_STATUS;

//--------------------------------------------------------------------------
// reset sci_parameters are optional, i.e. may be NULL
// (but not: ioctl (fd, STB04SCI_RESET),
// rather ioctl (fd, STB04SCI_RESET, NULL))
//--------------------------------------------------------------------------

#define IOCTL_SET_RESET		_IOW(SCI_IOW_MAGIC, 1, uint32_t)
#define IOCTL_SET_MODES		_IOW(SCI_IOW_MAGIC, 2, SCI_MODES)
#define IOCTL_GET_MODES		_IOW(SCI_IOW_MAGIC, 3, SCI_MODES)
#define IOCTL_SET_PARAMETERS	_IOW(SCI_IOW_MAGIC, 4, SCI_PARAMETERS)
#define IOCTL_GET_PARAMETERS	_IOW(SCI_IOW_MAGIC, 5, SCI_PARAMETERS)
#define IOCTL_SET_CLOCK_START	_IOW(SCI_IOW_MAGIC, 6, uint32_t)
#define IOCTL_SET_CLOCK_STOP	_IOW(SCI_IOW_MAGIC, 7, uint32_t)
#define IOCTL_GET_IS_CARD_PRESENT	_IOW(SCI_IOW_MAGIC, 8, uint32_t)
#define IOCTL_GET_IS_CARD_ACTIVATED	_IOW(SCI_IOW_MAGIC, 9, uint32_t)
#define IOCTL_SET_DEACTIVATE		_IOW(SCI_IOW_MAGIC, 10, uint32_t)
#define IOCTL_SET_ATR_READY		_IOW(SCI_IOW_MAGIC, 11, uint32_t)
#define IOCTL_GET_ATR_STATUS		_IOW(SCI_IOW_MAGIC, 12, uint32_t)
#define IOCTL_DUMP_REGS			_IOW(SCI_IOW_MAGIC, 20, uint32_t)

int scigetstatus(struct dvbdev* node, int32_t* status)
{
	if(node == NULL) return 1;

	if(ioctl(node->fd, IOCTL_GET_IS_CARD_PRESENT, status) < 0)
	{
		perr("IOCTL_GET_IS_CARD_PRESENT");
		return 1;
	}

	return 0;
}

int scireset(struct dvbdev* node, ATR* atr)
{
	unsigned char buf[SCI_MAX_ATR_SIZE];
	int32_t n = 0;
	SCI_PARAMETERS params;

	if(node == NULL) return 1;
	
	memset(&params, 0 , sizeof(SCI_PARAMETERS));
	
	params.ETU = 372; 
	params.EGT = 3; //not sure why this value is chosen
	params.fs = 5;
	params.T = 0;
	
	if(ioctl(node->fd, IOCTL_SET_PARAMETERS, &params) != 0)
	{
		perr("IOCTL_GET_PARAMETERS");
	}
	if(ioctl(node->fd, IOCTL_SET_RESET) < 0)
	{
		perr("IOCTL_GET_PARAMETERS");
	}

	while(n < SCI_MAX_ATR_SIZE && !serialread(node, ATR_TIMEOUT, 1, buf + n))
		n++;

	if((buf[0] !=0x3B) && (buf[0] != 0x3F) && (n > 9 && !memcmp(buf + 4, "IRDETO", 6))) //irdeto S02 reports FD as first byte on dreambox SCI, not sure about SH4 or phoenix
		buf[0] = 0x3B;
	
	if(n == 0)
	{
		err("no characters found in ATR");
		return 1;
	}

	if(!atrinitfromarray(atr, buf, n) == 0)
	{
		cs_sleepms(50);
		if(ioctl(node->fd, IOCTL_SET_ATR_READY) < 0)
		{
			perr("IOCTL_SET_ATR_READY");
			return 1;
		}
	}

	return 0;
}

int sciwrite(struct dvbdev* node, char T, uint32_t fs, uint32_t ETU, uint32_t WWT, uint32_t BWT, uint32_t CWT, uint32_t EGT, unsigned char P, unsigned char I)
{
	SCI_PARAMETERS params;

	if(node == NULL) return 1;

	//memset(&params,0,sizeof(SCI_PARAMETERS));
	if(ioctl(node->fd, IOCTL_GET_PARAMETERS, &params) < 0)
	{
		perr("IOCTL_GET_PARAMETERS");
	}

	params.T = T;
	params.fs = fs;

	//for Irdeto T14 cards, do not set ETU
	if(ETU) params.ETU = ETU;
	params.EGT = EGT;
	params.WWT = WWT;
	params.BWT = BWT;
	params.CWT = CWT;
	if(P) params.P = P;
	if(I) params.I = I;

	debug(630, "Setting T=%d fs=%d mhz ETU=%d WWT=%d CWT=%d BWT=%d EGT=%d clock=%d check=%d P=%d I=%d U=%d", (int)params.T, params.fs, (int)params.ETU, (int)params.WWT, (int)params.CWT, (int)params.BWT, (int)params.EGT, (int)params.clock_stop_polarity, (int)params.check, (int)params.P, (int)params.I, (int)params.U);

	if(ioctl(node->fd, IOCTL_SET_PARAMETERS, &params) != 0)
	{
		perr("IOCTL_SET_PARAMETERS");
		return 1;
	}

	return 0;
}

int sciactivate(struct dvbdev* node)
{
	int32_t in = 0;

	if(node == NULL) return 1;

	if(ioctl(node->fd, IOCTL_GET_IS_CARD_PRESENT, &in) < 0);
	{
		perr("IOCTL_GET_IS_CARD_PRESENT");
	}
			
	if(in)
		cs_sleepms(50);
	else
		return 1;

	return 0;
}

int scideactivate(struct dvbdev* node)
{
	int32_t in = 0;

	if(node == NULL) return 1;
		
	if(ioctl(node->fd, IOCTL_GET_IS_CARD_PRESENT, &in) < 0)
	{
		perr("IOCTL_GET_IS_CARD_PRESENT");
	}
			
	if(in)
	{
		if(ioctl(node->fd, IOCTL_SET_DEACTIVATE) < 0)
		{
			perr("IOCTL_SET_DEACTIVATE");
			return 1;
		}
	}

	return 0;
}

int scisetreset(struct dvbdev* node)
{
	unsigned char buf[SCI_MAX_ATR_SIZE];
	int32_t n = 0;

	if(node == NULL) return 1;

	if(ioctl(node->fd, IOCTL_SET_RESET) < 0)
	{
		perr("IOCTL_SET_RESET");
	}

	cs_sleepms(50);

	//flush atr from buffer
	while(n < SCI_MAX_ATR_SIZE && !serialread(node, ATR_TIMEOUT, 1, buf + n))
		n++;

	if(ioctl(node->fd, IOCTL_SET_ATR_READY) < 0)
	{
		perr("IOCTL_SET_ATR_READY");
		return 1;
	}

	cs_sleepms(50);
	return 0;
}

#endif
