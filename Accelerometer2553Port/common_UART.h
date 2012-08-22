
unsigned char charBuf ='0'; //buffer for RX character
static int rxIndex =0; //the character we are presently looking at
static int rxLength = 0; // the total number of characters in rxArray
unsigned char rxArray[2];


void uartInitialisation(void);
void transmitString(const volatile char *string, int length);
/*uartINitialisation()		Inputs: none
 * 	`						Returns: none
 * 							Modifies: clock, pins hardware stuff
 *
 * The setup the msp430g2553 requires to use the hardware uart
 *
 *
 *
 *
 */

void uartInitialisation()
{
	  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
	  DCOCTL = CALDCO_1MHZ;
	  P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
	  P1SEL2 = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
	  P2DIR |= BIT0;
	  P2OUT &= ~BIT0;
	  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	  UCA0BR0 = 26;                            // 1MHz 38400
	  UCA0BR1 = 0;                              // 1MHz 38400
	  UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
	  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	  IE2 = UCA0RXIE;                          // Enable USCI_A0 RX interrupt
		BCSCTL2 &= ~(DIVS_3);
		P1SEL |= BIT3 + BIT4 +BIT5; //allows these pins to do be used for non-digital io

		__delay_cycles(10000);

}

/*transmitString()		Inputs: String, length
 * 						Returns: nothing
 *
 * 						sends off a string as needed
 *
 *
 *
 *
 */

void transmitString(const volatile char *string, int length)
{

	 int i =0;
	 while (i<length)
	 {
		 WDTCTL = WDT_MDLY_32;
		 while (!(IFG2&UCA0TXIFG));
		 UCA0TXBUF = string[i];
		 i++;
	 }



}



//  Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{

	charBuf = UCA0RXBUF;
	rxArray[rxIndex]= charBuf;
	rxIndex++;
}
