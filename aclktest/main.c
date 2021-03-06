/*
 * main.c
 */
#include "MSP430G2553.h"

void P1_IODect();
void P13_Onclick();
void GPIO_Init();

static unsigned int click_time;
void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; //close DOG
	DCOCTL = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
	BCSCTL3 |= LFXT1S1;
	BCSCTL1 |= DIVA_0;
	GPIO_Init();
	click_time=0;
	_enable_interrupts();
	while(1)
	{

	}

}

void GPIO_Init()
{
	P1DIR |= BIT0;
	P1SEL |=BIT0;
	P1REN |= BIT3;
	P1OUT |= BIT3;
	P1DIR &= ~BIT3;
	P1IES |= BIT3;
	P1IE |= BIT3 ;
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	P1_IODect();
	P1IFG=0;

}

void P1_IODect()
{
	unsigned int Push_Key=0;
	Push_Key=P1IFG&(~P1DIR);
	__delay_cycles(10000);
	if((P1IN&Push_Key)==0)
	{
		switch(Push_Key)
		{
		case BIT3: P13_Onclick(); break;
		default: break;
		}
	}
}

void P13_Onclick()
{
	if(click_time>=4)
		click_time=0;
	else
		click_time++;
	switch(click_time)
	{
	case 0:BCSCTL1 &= !DIVA_3;BCSCTL1 |= DIVA_0;break;
	case 1:BCSCTL1 &= !DIVA_3;BCSCTL1 |= DIVA_1;break;
	case 2:BCSCTL1 &= !DIVA_3;BCSCTL1 |= DIVA_2;break;
	case 3:BCSCTL1 &= !DIVA_3;BCSCTL1 |= DIVA_3;break;
	default:break;
	}
}



