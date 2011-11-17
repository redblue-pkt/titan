#ifndef CARDREADER_H
#define CARDREADER_H

//MAJOR NUM OF DEVICE DRVIER
#define MAJOR_NUM			169

//constants
#define DEVICE_NAME		"sci_dev"
#define SCI_IOW_MAGIC	's'
#define ATR_TIMEOUT   800

typedef struct s_ATR
{
  unsigned length;
  unsigned char TS;
  unsigned char T0;
  struct
  {
    unsigned char value;
    int present;
  } ib[ATR_MAX_PROTOCOLS][ATR_MAX_IB], TCK;
  unsigned pn;
  unsigned char hb[ATR_MAX_HISTORICAL];
  unsigned hbn;
} ATR;

//ATR Paramenters
#define ATR_MAX_SIZE 		33	//Maximum size of ATR byte array/
#define ATR_MAX_HISTORICAL	15	//Maximum number of historical bytes
#define ATR_MAX_PROTOCOLS	7	//Maximun number of protocols
#define ATR_MAX_IB		4	//MaxNr of interface bytes per protocol
#define ATR_CONVENTION_DIRECT	0	//Direct convention
#define ATR_CONVENTION_INVERSE	1	//Inverse convention
#define ATR_PROTOCOL_TYPE_T0	0	//Protocol type T=0
#define ATR_PROTOCOL_TYPE_T1	1	//Protocol type T=1
#define ATR_PROTOCOL_TYPE_T2	2	//Protocol type T=2
#define ATR_PROTOCOL_TYPE_T3	3	//Protocol type T=3
#define ATR_PROTOCOL_TYPE_T14	14	//Protocol type T=14
#define ATR_INTERFACE_BYTE_TA	0	//Interface byte TAi
#define ATR_INTERFACE_BYTE_TB	1	//Interface byte TBi
#define ATR_INTERFACE_BYTE_TC	2	//Interface byte TCi
#define ATR_INTERFACE_BYTE_TD	3	//Interface byte TDi
#define ATR_PARAMETER_F		0	//Parameter F
#define ATR_PARAMETER_D		1	//Parameter D
#define ATR_PARAMETER_I		2	//Parameter I
#define ATR_PARAMETER_P		3	//Parameter P
#define ATR_PARAMETER_N		4	//Parameter N
#define ATR_INTEGER_VALUE_FI	0	//Integer value FI
#define ATR_INTEGER_VALUE_DI	1	//Integer value DI
#define ATR_INTEGER_VALUE_II	2	//Integer value II
#define ATR_INTEGER_VALUE_PI1	3	//Integer value PI1
#define ATR_INTEGER_VALUE_N	4	//Integer value N
#define ATR_INTEGER_VALUE_PI2	5	//Integer value PI2

//Default values for paramenters
#define ATR_DEFAULT_FI	1
#define ATR_DEFAULT_D	1
#define ATR_DEFAULT_I 	50
#define ATR_DEFAULT_N	0
#define ATR_DEFAULT_P	5

#define INVERT_BYTE(a)	((((a) << 7) & 0x80) | \
			(((a) << 5) & 0x40) | \
			(((a) << 3) & 0x20) | \
			(((a) << 1) & 0x10) | \
			(((a) >> 1) & 0x08) | \
			(((a) >> 3) & 0x04) | \
			(((a) >> 5) & 0x02) | \
			(((a) >> 7) & 0x01))


const uint32_t atr_fs_table[16] = {4000000L, 5000000L, 6000000L, 8000000L, 12000000L, 16000000L, 20000000L, 0, 0, 5000000L, 7500000L, 10000000L, 15000000L, 20000000L, 0, 0};
static const uint32_t atr_num_ib_table[16] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
const uint32_t atr_f_table[16] = {372, 372, 558, 744, 1116, 1488, 1860, 0, 0, 512, 768, 1024, 1536, 2048, 0, 0};
const double atr_d_table[16] = {0, 1, 2, 4, 8, 16, 32, 64, 12, 20, 0.5, 0.25, 0.125, 0.0625, 0.03125, 0.015625};
const uint32_t atr_i_table[4] = {25, 50, 100, 0};

void cs_sleepms(uint32_t msec)
{
	//does not interfere with signals like sleep and usleep do
	struct timespec req_ts;
	req_ts.tv_sec = msec / 1000;
	req_ts.tv_nsec = (msec % 1000) * 1000000L;
	int32_t olderrno = errno; //Some OS (especially MacOSX) seem to set errno to ETIMEDOUT when sleeping
	nanosleep(&req_ts, NULL);
	errno = olderrno;
}

int serialread(struct dvbdev* node, uint32_t timeout, uint32_t size, unsigned char* data)
{
	char c;
	uint32_t count = 0;
	int readed;
	struct timeval tv, tv_spent;

	if(node == NULL) return 1;
	
//TODO:
/*	if((node->typ != R_INTERNAL) && (node->written > 0))
	{
		char buf[256];
		int32_t n = node->written;
		node->written = 0;
	
		if(serialread(node, timeout, n, buf))
			return 1;
	}
*/
	
	for(count = 0; count < size ; count++)
	{
		gettimeofday(&tv, 0);
		memcpy(&tv_spent,&tv, sizeof(struct timeval));
		readed = FALSE;
		while((((tv_spent.tv_sec - tv.tv_sec) * 1000) + ((tv_spent.tv_usec - tv.tv_usec) / 1000L)) < timeout)
 		{
 			if(read(node->fd, &c, 1) == 1)
 			{
 				readed = TRUE;
				break;
 			}
 			gettimeofday(&tv_spent, 0);
		}
		if(!readed)
		{
			debug(630, "Receiving: count=%d", count);
			return 1;
		}
		data[count] = c;
	}
	debug(630, "Receiving: count=%d", count);
	return 0;
}

int atrinitfromarray(ATR* atr, char atr_buffer[ATR_MAX_SIZE], uint32_t length)
{
	unsigned char TDi;
	unsigned char buffer[ATR_MAX_SIZE];
	uint32_t pointer = 0, pn = 0;
	
	/* Check size of buffer */
	if(length < 2) return(2);
	
	/* Check if ATR is from a inverse convention card */
	if(atr_buffer[0] == 0x03)
	{
		for(pointer = 0; pointer < length; pointer++)
			buffer[pointer] = ~(INVERT_BYTE(atr_buffer[pointer]));
	}
	else
		memcpy(buffer, atr_buffer, length);
	
	/* Store T0 and TS */
	atr->TS = buffer[0];
	atr->T0 = TDi = buffer[1];
	pointer = 1;
	
	/* Store number of historical bytes */
	atr->hbn = TDi & 0x0F;
	
	/* TCK is not present by default */
	(atr->TCK).present = 0;
	
	/* Extract interface bytes */
	while(pointer < length)
	{
		/* Check buffer is long enought */
		if(pointer + atr_num_ib_table[(0xF0 & TDi) >> 4] >= length)
			return(2);
		
		/* Check TAi is present */
		if((TDi | 0xEF) == 0xFF)
		{
			pointer++;
			atr->ib[pn][ATR_INTERFACE_BYTE_TA].value = buffer[pointer];
			atr->ib[pn][ATR_INTERFACE_BYTE_TA].present = 1;
		}
		else
			atr->ib[pn][ATR_INTERFACE_BYTE_TA].present = 0;
		
		/* Check TBi is present */
		if((TDi | 0xDF) == 0xFF)
		{
			pointer++;
			atr->ib[pn][ATR_INTERFACE_BYTE_TB].value = buffer[pointer];
			atr->ib[pn][ATR_INTERFACE_BYTE_TB].present = 1;
		}
		else
			atr->ib[pn][ATR_INTERFACE_BYTE_TB].present = 0;
		
		/* Check TCi is present */
		if((TDi | 0xBF) == 0xFF)
		{
			pointer++;
			atr->ib[pn][ATR_INTERFACE_BYTE_TC].value = buffer[pointer];
			atr->ib[pn][ATR_INTERFACE_BYTE_TC].present = 1;
		}
		else
			atr->ib[pn][ATR_INTERFACE_BYTE_TC].present = 0;
		
		/* Read TDi if present */
		if((TDi | 0x7F) == 0xFF)
		{
			pointer++;
			TDi = atr->ib[pn][ATR_INTERFACE_BYTE_TD].value = buffer[pointer];
			atr->ib[pn][ATR_INTERFACE_BYTE_TD].present = 1;
			(atr->TCK).present = ((TDi & 0x0F) != ATR_PROTOCOL_TYPE_T0);
			if(pn >= ATR_MAX_PROTOCOLS) return(2);
			pn++;
		}
		else
		{
			atr->ib[pn][ATR_INTERFACE_BYTE_TD].present = 0;
			break;
		}
	}
	
	/* Store number of protocols */
	atr->pn = pn + 1;
	
	/* Store historical bytes */
	if (pointer + atr->hbn >= length)
	{
		err("ATR is malformed, it reports %i historical bytes but there are only %i", atr->hbn, length-pointer - 2);
		if(length-pointer >= 2)
			atr->hbn = length-pointer - 2;
		else
		{
			atr->hbn = 0;
			atr->length = pointer + 1;
			return(2);
		}

	}
	
	memcpy(atr->hb, buffer + pointer + 1, atr->hbn);
	pointer += (atr->hbn);
	
	/* Store TCK  */
	if((atr->TCK).present)
	{	
		if(pointer + 1 >= length) return(2);
		pointer++;
		(atr->TCK).value = buffer[pointer];
	}
	
	atr->length = pointer + 1;
	
	//check that TA1, if pn==1 , has a valid value for FI
	if((atr->pn==1) && (atr->ib[pn][ATR_INTERFACE_BYTE_TA].present != 0))
	{
		unsigned char FI;
		debug(630, "TA1 = %02x", atr->ib[pn][ATR_INTERFACE_BYTE_TA].value);
		FI = (atr->ib[pn][ATR_INTERFACE_BYTE_TA].value & 0xF0) >> 4;
		debug(630, "FI = %02x", FI);
		if(atr_fs_table[FI] == 0)
		{
			err("Invalid ATR as FI is not returning a valid frequency value");
			return(2);
		}
	}
    
	//check that TB1 < 0x80
	if((atr->pn==1) && (atr->ib[pn][ATR_INTERFACE_BYTE_TB].present != 0))
	{
		if(atr->ib[pn][ATR_INTERFACE_BYTE_TB].value > 0x80)
		{
			err("Invalid ATR as TB1 has an invalid value");
			return(2);
		}
	}
	return(0);
}

#endif
