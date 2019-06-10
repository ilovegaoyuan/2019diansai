/*
 * PWM_LED.c
 * 硬件描述：Launchpad G2553开发板上P1.3接了一个按键，P1.6各接了1个绿光LED（用跳线帽连接）。
 * 功能描述：按键按下的瞬间，LED亮度变化。需调用TA_PWM库函数。
 *  Created on: 2013-4-8
 *  Author: Administrator
 */

#include "MSP430G2553.h"
#include "TA_PWM.h"

void P1_IODect();
void P13_Onclick();
void GPIO_init();
void Change_Bright();
void timer0_init();

int Bright;
int timer_cnt=0;
int p13_flag;
int step;

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;	//关狗
	//-----初始化TA0为ACLK输入，不分频，通道1超前PWM，通道2关闭-----
	DCOCTL = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
	TA1_PWM_Init('S',1,'F',0);
	TA1_PWM_SetPeriod(400);
	TA1_PWM_SetPermill(1,10);
	timer0_init();
	Bright=50;
	p13_flag=1;
	step=20;
	//设定PWM周期
	GPIO_init();											//初始化GPIO
	_enable_interrupts();							//使能总中断
    //_bis_SR_register(LPM3_bits);				//LPM3方式休眠
}
void Change_Bright()
{
	if(p13_flag==0)
	{
		if(timer_cnt==48)
		{
			timer_cnt=0;
			if(Bright==10)
				Bright=990;
			else
				Bright=10;
		}
		else
			timer_cnt++;
	}
	else if (p13_flag==1)
	{
		if(Bright==10)
			step=20;
		else if(Bright==990)
			step=-20;
		Bright=Bright+step;
	}
	TA1_PWM_SetPermill(1,Bright);
}
/******************************************************************************************************
 * 名       称：GPIO_Init()
 * 功       能：设定按键和LED控制IO的方向，启用按键IO的上拉电阻
 * 入口参数：无
 * 出口参数：无
 * 说       明：无
 * 范       例：无
 ******************************************************************************************************/
void GPIO_init()
{
	//----- TA0_PWM_Init()自动设置TA口（P1.6）-----

	//-----配合机械按键，启用内部上拉电阻-----
	P1REN |= BIT3;				        //启用P1.3内部上下拉电阻
	P1OUT |= BIT3;			            //将电阻设置为上拉
	//-----配置P1.3中断参数-----
	P1DIR &= ~BIT3;                    // P1.3设为输入(可省略)
	P1IES |= BIT3;                          // P1.3设为下降沿中断
	P1IE  |= BIT3 ;                          // 允许P1.3中断
}

/******************************************************************************************************
 * 名       称：PORT1_ISR()
 * 功       能：响应P1口的外部中断服务
 * 入口参数：无
 * 出口参数：无
 * 说       明：P1.0~P1.8共用了PORT1中断，所以在PORT1_ISR()中必须查询标志位P1IFG才能知道
 * 					 具体是哪个IO引发了外部中断。P1IFG必须手动清除，否则将持续引发PORT1中断。
 * 范       例：无
 ******************************************************************************************************/
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	//-----启用Port1事件检测函数-----
	P1_IODect();								//检测通过，则会调用事件处理函数
   	P1IFG=0;                            			//退出中断前必须手动清除IO口中断标志
}

/******************************************************************************************************
 * 名       称：P1_IODect()
 * 功       能：判断具体引发中断的IO，并调用相应IO的中断事件处理函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：该函数兼容所有8个IO的检测，请根据实际输入IO激活“检测代码”。
 * 					 本例中，仅有P1.3被用作输入IO，所以其他7个IO的“检测代码”没有被“激活”。
 * 范       例：无
 ******************************************************************************************************/
void P1_IODect()
{
	unsigned int Push_Key=0;
	//-----排除输出IO的干扰后，锁定唯一被触发的中断标志位-----
	Push_Key=P1IFG&(~P1DIR);
	//-----延时一段时间，避开机械抖动区域-----
	__delay_cycles(10000);							//消抖延时
	//----判断按键状态是否与延时前一致-----
	if((P1IN&Push_Key)==0) 	                	//如果该次按键确实有效
   {
	 //----判断具体哪个IO被按下，调用该IO的事件处理函数-----
	  switch(Push_Key){
//	 case BIT0:	P10_Onclick();		break;
//	 case BIT1: 	P11_Onclick();		break;
//	 case BIT2: 	P12_Onclick();		break;
	 case BIT3: 	P13_Onclick();		break;
//	 case BIT4: 	P14_Onclick();		break;
//	 case BIT5: 	P15_Onclick();		break;
//	 case BIT6: 	P16_Onclick();		break;
//	 case BIT7: 	P17_Onclick();		break;
	 default:									break;		//任何情况下均加上default
	 }
   }
}

/******************************************************************************************************
 * 名       称：P13_Onclick()
 * 功       能：P1.3的中断事件处理函数，即当P1.3键被按下后，下一步干什么
 * 入口参数：无
 * 出口参数：无
 * 说       明：使用事件处理函数的形式，可以增强代码的移植性和可读性
 * 范       例：无
 ******************************************************************************************************/
void P13_Onclick()									//P1.3的事件处理函数
{
	if(p13_flag==0)
		p13_flag=1;
	else
		p13_flag=0;

	_nop();
}

void timer0_init()
{
  /*
   *设置TIMER_A的时钟
   *TASSEL_0: TACLK,使用外部引脚信号作为输入
   *TASSEL_1: ACLK,辅助时钟
   *TASSEL_2: SMCLK,子系统主时钟
   *TASSEL_3: INCLK,外部输入时钟
  */
  TACTL |= TASSEL_2;

  /*
   *时钟源分频
   *ID_0: 不分频
   *ID_1: 2分频
   *ID_2: 4分频
   *ID_3: 8分频
   */
  TACTL |= ID_0;

  /*
   *模式选择
   *MC_0: 停止模式,用于定时器暂停
   *MC_1: 增计数模式,计数器计数到CCR0,再清零计数器
   *MC_2: 连续计数模式,计数器增计数到0XFFFF(65535),再清零计数器
   *MC_3: 增减计数模式,增计数到CCR0,再减计数到0
   */
  TACTL |= MC_1;  //增计数模式

  //----计数器清零-----
  TACTL |= TACLR;

  //----设置TACCRx的值-----
  //TACCR0=32768-1;     //时钟是32.768K,那么值设为32678-1
  TACCR0=1306-1;
  //TACCR1=10000;         //TACCR1和TACCR2要小于TACCR0,否则不会产生中断
  //TACCR2=20000;

  //----中断允许----
  TACCTL0 |= CCIE;      //TACCR0中断
  //TACCTL1 |= CCIE;      //TACCR1中断
  //TACCTL2 |= CCIE;      //TACCR2中断
  //TACTL |= TAIE;        //TA0溢出中断

}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0(void)    //TACCR0中断
{
  Change_Bright();
}

