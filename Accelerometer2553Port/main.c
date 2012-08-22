// 1st attempt at reading data off an accelerometer:
// the ADXL335
#include  "msp430g2553.h"
#include  "accelerometer_header.h" //header with all the required calls for using the accelerometer
#include "common.h"
#include "common_UART.h"
#include "bluetooth.h"


#include <stdlib.h>	// for itoa() call



void main(void)
{

	adcInitialisation();
	uartInitialisation();
	bluetoothSetup();


	while(1)
	{



		int i;

		for( i = 0; i<sizeOfdataArray;i++)
		{
			__delay_cycles(50000);
			adcRead();
		}
		for( i = 0; i<sizeOfdataArray; i++)
		{
			transmitString("{\'accl\':{",9);
			itoa(i, dat, 10);
			transmitString(dat,4);
			transmitString(":[",2);
			itoa(dataArray[i].x, dat, 10);
			transmitString(dat,4);
			transmitString(",",1);
			itoa(dataArray[i].y, dat, 10);
			transmitString(dat,4);
			transmitString(",",1);
			itoa(dataArray[i].z, dat, 10);
			transmitString(dat,4);
			transmitString("]}}",3);
		}
		transmitString("\r\n",1);
		charBuf = '0';
		__enable_interrupt();
		while(charBuf !='C');
		__disable_interrupt();


	}



}






