/*
 * main.c
 */
#include"msp430g2553.h"
#include "stdio.h"
#include"SPImaster.h"
unsigned char RegisterData[4] = {0,0,0,0};
void GPIO_INTA();
void main(void)
{
	unsigned long int R;
	unsigned long int Rx;
	int i;
	WDTCTL=WDTPW|WDTHOLD;
	GPIO_INTA();
	P1OUT&=(~BIT4);
	delay(10);
	P1OUT|=(BIT4);
	delay(10);
	P1OUT&=(~BIT4);
	RegisterData[0] = 0x10;		//Channel 0 enable
	WriteToAD9959ViaSpi(0x00,1,RegisterData);

	RegisterData[2] = 0xD3;		//REF CLK = 50MHz, Multiplication factor = 10
	RegisterData[1] = 0x00;
	RegisterData[0] = 0x00;
	WriteToAD9959ViaSpi(0x01,3,RegisterData);//VCO gain control = 1, system clock = 500MHz
	R=0x00000008;
//	RegisterData[3] = 0x25;
//	RegisterData[2] = 0x1E;
//	RegisterData[1] = 0xB8;
	//RegisterData[0] = 0x51;
//	RegisterData[0] = R&0xFF;
//	RegisterData[1] = (R&0xFF00)>>8;
//	RegisterData[2] = (R&0xFF0000)>>16;
//	RegisterData[3] = (R&0xFF000000)>>24;
//	WriteToAD9959ViaSpi(0x04,4,RegisterData);//Output frequency = 10MHz

	//IO_Update();
//	ReadFromAD9959ViaSpi(0x80,1,RegisterData);
	while(1)
	{
		if(R>=0x66666666)
		{
			R=0x00000008;
//			for(i=0;i<10000;i++)
//					delay(10000);
		}
		R=R+(R>>3);
//		printf("%d",R);

		//Rx = R * 10;
		Rx=R;
		RegisterData[0] = Rx & 0x000000FF;
		RegisterData[1] = (Rx & 0x0000FF00)>>8;
		RegisterData[2] = (Rx & 0x00FF0000)>>16;
		RegisterData[3] = (Rx & 0xFF000000)>>24;
		WriteToAD9959ViaSpi(0x04,4,RegisterData);//Output frequency = 10MHz

		IO_Update();
		for(i=0;i<500;i++)
		delay(100);
	}

}

void GPIO_INTA()
{
	P1DIR|=BIT0;
	P1DIR|=BIT1;
	P1DIR|=BIT2;
	P1DIR|=BIT3;
	P1DIR|=BIT4;
}
