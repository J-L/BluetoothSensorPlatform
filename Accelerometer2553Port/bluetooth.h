
//sets up MCU
void bluetoothSetup(void);
//sets up HC-05





const char bluetoothAT[] = "AT\r\n";
const char bluetoothInitName[] = "AT+NAME=BT-SENSOR\r\n";
const char bluetoothNameCheck[]="AT+NAME?\r\n";
const char bluetoothReset[] = "AT+RESET\r\n";



void bluetoothSetup(void)
{
	P2OUT |= BIT0;
	__delay_cycles(10000000);


	 transmitString(bluetoothAT,4);

	 __delay_cycles(1000000);
	  transmitString(bluetoothInitName,19);
	  __delay_cycles(10000000);

	  P2OUT &= ~BIT0;
	  __delay_cycles(1000000);
	  transmitString(bluetoothReset,10);

}
