/*
 * PWM_LED.c
 * Ӳ��������Launchpad G2553��������P1.3����һ��������P1.6������1���̹�LED��������ñ���ӣ���
 * �����������������µ�˲�䣬LED���ȱ仯�������TA_PWM�⺯����
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
	WDTCTL = WDTPW + WDTHOLD;	//�ع�
	//-----��ʼ��TA0ΪACLK���룬����Ƶ��ͨ��1��ǰPWM��ͨ��2�ر�-----
	DCOCTL = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
	TA1_PWM_Init('S',1,'F',0);
	TA1_PWM_SetPeriod(400);
	TA1_PWM_SetPermill(1,30);
	timer0_init();
	Bright=50;
	p13_flag=1;
	step=20;
	//�趨PWM����
	GPIO_init();											//��ʼ��GPIO
	_enable_interrupts();							//ʹ�����ж�
    //_bis_SR_register(LPM3_bits);				//LPM3��ʽ����
}
void Change_Bright()
{
	if(p13_flag==0)
	{
		if(timer_cnt==46)
		{
			timer_cnt=0;
			step=10;
			if(Bright==30)
				Bright=970;
			else
				Bright=30;
		}
		else
			timer_cnt++;
	}
	else if (p13_flag==1)
	{
		if(Bright==30)
			step=20;
		else if(Bright==970)
			step=-20;
		Bright=Bright+step;
	}
	//

	TA1_PWM_SetPermill(1,Bright,step);
}
/******************************************************************************************************
 * ��       �ƣ�GPIO_Init()
 * ��       �ܣ��趨������LED����IO�ķ������ð���IO����������
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ������
 * ��       ������
 ******************************************************************************************************/
void GPIO_init()
{
	//----- TA0_PWM_Init()�Զ�����TA�ڣ�P1.6��-----

	//-----��ϻ�е�����������ڲ���������-----
	P1REN |= BIT3;				        //����P1.3�ڲ�����������
	P1OUT |= BIT3;			            //����������Ϊ����
	//-----����P1.3�жϲ���-----
	P1DIR &= ~BIT3;                    // P1.3��Ϊ����(��ʡ��)
	P1IES |= BIT3;                          // P1.3��Ϊ�½����ж�
	P1IE  |= BIT3 ;                          // ����P1.3�ж�
}

/******************************************************************************************************
 * ��       �ƣ�PORT1_ISR()
 * ��       �ܣ���ӦP1�ڵ��ⲿ�жϷ���
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����P1.0~P1.8������PORT1�жϣ�������PORT1_ISR()�б����ѯ��־λP1IFG����֪��
 * 					 �������ĸ�IO�������ⲿ�жϡ�P1IFG�����ֶ���������򽫳�������PORT1�жϡ�
 * ��       ������
 ******************************************************************************************************/
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	//-----����Port1�¼���⺯��-----
	P1_IODect();								//���ͨ�����������¼���������
   	P1IFG=0;                            			//�˳��ж�ǰ�����ֶ����IO���жϱ�־
}

/******************************************************************************************************
 * ��       �ƣ�P1_IODect()
 * ��       �ܣ��жϾ��������жϵ�IO����������ӦIO���ж��¼���������
 * ��ڲ�������
 * ���ڲ�������
 * ˵       �����ú�����������8��IO�ļ�⣬�����ʵ������IO��������롱��
 * 					 �����У�����P1.3����������IO����������7��IO�ġ������롱û�б��������
 * ��       ������
 ******************************************************************************************************/
void P1_IODect()
{
	unsigned int Push_Key=0;
	//-----�ų����IO�ĸ��ź�����Ψһ���������жϱ�־λ-----
	Push_Key=P1IFG&(~P1DIR);
	//-----��ʱһ��ʱ�䣬�ܿ���е��������-----
	__delay_cycles(1000);
	P13_Onclick();//������ʱ
	//----�жϰ���״̬�Ƿ�����ʱǰһ��-----
	//if((P1IN&Push_Key)==0) 	                	//����ôΰ���ȷʵ��Ч
  // {
	 //----�жϾ����ĸ�IO�����£����ø�IO���¼���������-----
	  //switch(Push_Key){
//	 case BIT0:	P10_Onclick();		break;
//	 case BIT1: 	P11_Onclick();		break;
//	 case BIT2: 	P12_Onclick();		break;
	 //case BIT3:
//		P13_Onclick();		//break;
//	 case BIT4: 	P14_Onclick();		break;
//	 case BIT5: 	P15_Onclick();		break;
//	 case BIT6: 	P16_Onclick();		break;
//	 case BIT7: 	P17_Onclick();		break;
	 //default:									break;		//�κ�����¾�����default
	 //}
   //}
}

/******************************************************************************************************
 * ��       �ƣ�P13_Onclick()
 * ��       �ܣ�P1.3���ж��¼���������������P1.3�������º���һ����ʲô
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����ʹ���¼�������������ʽ��������ǿ�������ֲ�ԺͿɶ���
 * ��       ������
 ******************************************************************************************************/
void P13_Onclick()									//P1.3���¼���������
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
   *����TIMER_A��ʱ��
   *TASSEL_0: TACLK,ʹ���ⲿ�����ź���Ϊ����
   *TASSEL_1: ACLK,����ʱ��
   *TASSEL_2: SMCLK,��ϵͳ��ʱ��
   *TASSEL_3: INCLK,�ⲿ����ʱ��
  */
  TA0CTL |= TASSEL_2;

  /*
   *ʱ��Դ��Ƶ
   *ID_0: ����Ƶ
   *ID_1: 2��Ƶ
   *ID_2: 4��Ƶ
   *ID_3: 8��Ƶ
   */
  TA0CTL |= ID_0;

  /*
   *ģʽѡ��
   *MC_0: ֹͣģʽ,���ڶ�ʱ����ͣ
   *MC_1: ������ģʽ,������������CCR0,�����������
   *MC_2: ��������ģʽ,��������������0XFFFF(65535),�����������
   *MC_3: ��������ģʽ,��������CCR0,�ټ�������0
   */
  TA0CTL |= MC_1;  //������ģʽ

  //----����������-----
  TA0CTL |= TACLR;

  //----����TACCRx��ֵ-----
  //TACCR0=32768-1;     //ʱ����32.768K,��ôֵ��Ϊ32678-1
  TA0CCR0=1361-1;
  //TACCR1=10000;         //TACCR1��TACCR2ҪС��TACCR0,���򲻻�����ж�
  //TACCR2=20000;

  //----�ж�����----
  TA0CCTL0 |= CCIE;      //TACCR0�ж�
  //TACCTL1 |= CCIE;      //TACCR1�ж�
  //TACCTL2 |= CCIE;      //TACCR2�ж�
  //TACTL |= TAIE;        //TA0����ж�

}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0(void)    //TACCR0�ж�
{
	//_disable_interrupts();
  Change_Bright();
 // _enable_interrupts();
}
