/*
common.h 
commonly used functions which will be needed
itoa(a interger to string converter
 */

#define xPin 3
#define yPin 4
#define zPin 5
#define sizeOfdataArray 60

struct accelerometerData
{
	int x;
	int y;
	int z;
};


struct accelerometerData dataArray[sizeOfdataArray];




int gforces[3];
int adcSel = 6;
int voltageCalibration;
int temp2;
int centerpointCalibration[3] = {200, 0 ,0};
int readingNumber =0;

void adcRead(void);
void adcInitialisation( void);
unsigned int  pinRead(void);
void adcConfiguration(void);
void mspSetup(void);
unsigned int adcSupplyVoltage(void);

/*adcConfiguration is used by adcRead to configure the ports for each read
 *
 *
 *
 *
 */

void adcInitialisation()
{
	WDTCTL = WDTPW +WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	BCSCTL2 &= ~(DIVS_3);
	P1SEL |= BIT3 + BIT4 +BIT5; //allows these pins to do be used for non-digital io

}
/*adcConfiguration() 	Inputs: Nothing
 * 						Returns: Nothing
 * 						Uses: adcSel to know which pin to set up for reading
 * 						Modifies: nothing
 *
 *  					The function is used by pinRead() to set up the adc correctly
 *
 */


void adcConfiguration()
{
	if(adcSel == xPin)
	{
		ADC10AE0 = BIT3;
		ADC10CTL1 = INCH_3; //channel 3 (corresponds to pin 1.3)
	}
	else if (adcSel == yPin)
	{
		ADC10AE0 = BIT4;
		ADC10CTL1 = INCH_4 ; //channel 4 (corresponds to pin 1.4)
	}
	else
	{
		ADC10AE0 = BIT4;
		ADC10CTL1 = INCH_5  ; //channel 5 (corresponds to pin 1.5)
	}

}
/*adcRead() Inputs: nothing
 * 			Outputs: nothing
 * 			Modifies: writes to global variable gforces the z, y ,x inputs from accelerometer on pins 1.3-5
 *
 * 			Completes one full ADC read on each of the pins connected to the accelerometer
 *
 */

void adcRead()
{

	while (adcSel <= zPin)
	{
		gforces[adcSel-3] =(pinRead()-512);
		adcSel++;

	}
	dataArray[readingNumber].x = gforces[0];
	dataArray[readingNumber].y = gforces[1];
	dataArray[readingNumber].z = gforces[2];
	readingNumber++;
	if(readingNumber==sizeOfdataArray)
	{
		readingNumber =0;
	}

	voltageCalibration = (int)adcSupplyVoltage();
	adcSel = xPin;


}
/*pinRead 	Inputs: nothing
 * 			Returns: the 10 bit adc value associate to the accelerometer output for a pin,
 * 			Uses: global variable adcSel to know which pin to read
 * 			modifies: nothing
 *
 */

unsigned int pinRead()
{
	ADC10CTL0 &= ~ENC;
	ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;// interrupting adc goodness
	adcConfiguration();
	__delay_cycles(100); //replace ith timer

	ADC10CTL0 |= ENC +ADC10SC; //start sampling and conversion
	while (!(ADC10CTL0 & ADC10IFG)); //__bis_SR_register(CPUOFF +GIE);
	return ADC10MEM;



}
/*adcSupplyVoltage() 	Inputs: None
 * 						Returns: the 10bit ADC
 *
 * 						the supply voltage must be recorded when using the accelerometer to calibrate the g forces, as the offset is dependent on the supply voltage
 * 						the dependence is roughly 100 mV/Vss aka the offset is 300mV with 3V supply / g and 200mV with 2V /g supply
 */


unsigned int adcSupplyVoltage(void)// calculations must be calibrated to agree with supply voltage
{
	ADC10CTL0 = 0;
	__delay_cycles(100);
	ADC10CTL0 = SREF_1 + REFON + REF2_5V + ADC10ON + ADC10SHT_3; // sets up voltage reference
	ADC10CTL1 = INCH_11;
	__delay_cycles(100);                                     // Allow internal reference to stabilize
	ADC10CTL0 |= ENC + ADC10SC;                     // Enable conversions
	while (!(ADC10CTL0 & ADC10IFG));   //__bis_SR_register(CPUOFF +GIE);             // Conversion done?
	return  ADC10MEM;



}





