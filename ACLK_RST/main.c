/*
 * main.c
 */
#include "MSP430G2553.h"


void GPIO_Init();

static unsigned int click_time;
void main(void)
{
	WDTCTL = WDTPW + WDTHOLD +WDTNMI; //NMI模式（非Reset模式）
	IE1|=NMIIE; //开NMI中断，无需开总中断
	DCOCTL = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
	BCSCTL3 |= LFXT1S1;
	BCSCTL1 |= DIVA_0;
	GPIO_Init();
	click_time=0;
	while(1)
	{

	}

}

void GPIO_Init()
{
	P1DIR |= BIT0;
	P1SEL |=BIT0;
}

#pragma vector=NMI_VECTOR
__interrupt void NMI_ISR(void) //不可屏蔽中断
{
	IFG1 &= ~NMIIFG;
	if(click_time>=3)
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
	IE1|=NMIIE;
}

