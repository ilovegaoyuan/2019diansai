#include<MSP430G2553.h>
void GPIO_Init();
static unsigned int click_time;
void main( void )
{
	WDTCTL = WDTPW + WDTHOLD +WDTNMI; //NMI模式（非Reset模式）
	IE1|=NMIIE; //开NMI中断，无需开总中断
	DCOCTL = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;
	BCSCTL2 |= DIVS_3;
	GPIO_Init();
	click_time=0;
	while(1)
	{}
}
//==========NMI中断服务子函数==============
#pragma vector=NMI_VECTOR
__interrupt void NMI_ISR(void) //不可屏蔽中断
{
	IFG1 &= ~NMIIFG;
	if(click_time>=6)
			click_time=0;
		else
			click_time++;
		switch(click_time)
		{
		case 0:DCOCTL = CALDCO_1MHZ;BCSCTL1 = CALBC1_1MHZ;BCSCTL2 &= !DIVS_3;BCSCTL2 |= DIVS_3;break;
		case 1:DCOCTL = CALDCO_1MHZ;BCSCTL1 = CALBC1_1MHZ;BCSCTL2 &= !DIVS_3;BCSCTL2 |= DIVS_2;break;
		case 2:DCOCTL = CALDCO_1MHZ;BCSCTL1 = CALBC1_1MHZ;BCSCTL2 &= !DIVS_3;BCSCTL2 |= DIVS_1;break;
		case 3:DCOCTL = CALDCO_1MHZ;BCSCTL1 = CALBC1_1MHZ;BCSCTL2 &= !DIVS_3;BCSCTL2 |= DIVS_0;break;
		case 4:DCOCTL = CALDCO_8MHZ;BCSCTL1 = CALBC1_8MHZ;BCSCTL2 &= !DIVS_3;BCSCTL2 |= DIVS_0;break;
		case 5:DCOCTL = CALDCO_12MHZ;BCSCTL1 = CALBC1_12MHZ;BCSCTL2 &= !DIVS_3;BCSCTL2 |= DIVS_0;break;
		case 6:DCOCTL = CALDCO_16MHZ;BCSCTL1 = CALBC1_16MHZ;BCSCTL2 &= !DIVS_3;BCSCTL2 |= DIVS_0;break;
		default:break;
		}
	IE1|=NMIIE;
}






void GPIO_Init()
{
	P1DIR |= BIT4
	P1SEL |=BIT4;
}


