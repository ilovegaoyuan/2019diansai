
#include"msp430g2553.h"
#include"spimaster.h"

#define SET_LE()		P2OUT|=BIT0;	//P2.0->/LE
#define CLR_LE()		P2OUT&=(~BIT0);

#define	SET_SCL()		P2OUT|=BIT1;	//P2.1->SCLK
#define	CLR_SCL()		P2OUT&=(~BIT1);

#define SET_SDIO()		P2OUT|=BIT2;	//P2.2->SDATA
#define CLR_SDIO()		P2OUT&=(~BIT2);



void WriteTope4302 ( unsigned char RegisterData)
{
	unsigned	char	ValueToWrite = 0;
	unsigned	char	i = 0;


	CLR_LE();
	delay(1);

	ValueToWrite = RegisterData;
	for (i=0; i<6; i++)
	{
		CLR_SCL();
		if(0x20 == (ValueToWrite & 0x20))
		{
			SET_SDIO();	  //Send one to SDIO pin
		}
		else
		{
			CLR_SDIO();	  //Send zero to SDIO pin
		}
		delay(1);
		SET_SCL();
		delay(1);
		ValueToWrite <<= 1;	//Rotate data
	}
	
	CLR_SCL();
	delay(1);
	SET_LE();	//bring CS high again
}
