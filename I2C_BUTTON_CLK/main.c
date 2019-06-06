/*���棺����G2 LaunchPad�ϵ�LED2ʹ����P1.6��I2C���ţ������������漰��I2C��ʵ�鶼�����P1.6���߰γ�,���������̣�
 * main.c
 */
#include"MSP430G2553.h"
#include"TCA6416A.h"

static unsigned SEL_temp=0x00;
static unsigned DCO_temp=0x00;
static unsigned MOD_temp=0x00;
int p_s_flag=1;
void GPIO_Init()
{
	P1DIR |= BIT4;
	P1SEL |=BIT4;
}

void WDT_init();
void I2C_IODect()	;		                 //����¼�ȷʵ������
void delay (signed int length)
{
	while (length >0)
    	length--;
}

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;	//�ع�
    BCSCTL1 = CALBC1_8MHZ;      /* Set DCO to 8MHz */
    DCOCTL = CALDCO_8MHZ;
    BCSCTL3 |= LFXT1S1;
    GPIO_Init();
    TCA6416A_Init();			// ��ʼ��IO��չ��
//----��ʾ��ʼ���ɹ�----
	PinOUT(0,1);           // ָ��0�Źܽ����Ϊ0
	PinOUT(1,0);           // ָ��1�Źܽ����Ϊ0
	PinOUT(2,1);           // ָ��0�Źܽ����Ϊ0
	PinOUT(3,0);           // ָ��1�Źܽ����Ϊ0
	PinOUT(4,1);           // ָ��0�Źܽ����Ϊ0
	PinOUT(5,0);           // ָ��1�Źܽ����Ϊ0
	PinOUT(6,1);           // ָ��0�Źܽ����Ϊ0
	PinOUT(7,0);           // ָ��1�Źܽ����Ϊ0
	WDT_init();
	while(1)
	{
		PinIN();
		 I2C_IODect();
		 //delay(1000);
		 _bis_SR_register(LPM0_bits);
	}


}
/********WDT��ʼ������**********/
void WDT_init()
{
	//-----�趨WDTΪ-----------
	WDTCTL=WDT_ADLY_16;
	//-----WDT�ж�ʹ��----------------------
    IE1 |= WDTIE;
}

/***********WDT��ʱ�жϺ���*************/
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
	//PinIN();
	 //I2C_IODect();
__bic_SR_register_on_exit(LPM0_bits );
}
/**********ʱ���޸ĺ���***********/
void SEL_PLUS()
{
	if(SEL_temp==0x0F)
		SEL_temp=0x00;
	else
		SEL_temp+=0x01;
}
void SEL_SUB()
{
	if(SEL_temp==0x00)
		SEL_temp=0x0F;
	else
		SEL_temp-=0x01;
}
void DCO_PLUS()
{
	if(DCO_temp==0xE0)
	{
		DCO_temp=0x00;
		SEL_PLUS();
	}

	else
		DCO_temp+=0x20;
}

void DCO_SUB()
{
	if(DCO_temp==0x00)
	{
		DCO_temp=0xE0;
		SEL_SUB();
	}

	else
		DCO_temp-=0x20;
}
void MOD_PLUS()
{
	if(MOD_temp==0x1F)
	{
		MOD_temp=0x00;
		DCO_PLUS();
	}

	else
		MOD_temp+=0x01;
}

void MOD_SUB()
{
	if(MOD_temp==0x00)
	{
		MOD_temp=0x1F;
		DCO_SUB();
	}

	else
		MOD_temp-=0x01;
}
/**********I2C��չIO�ж��¼�������***************/
void I2C_IO10_Onclick()
{
	static unsigned char turn = 1;
	turn ^= BIT0;
	PinOUT(0,turn);         //ָ��0�Źܽ����Ϊ0&1
	PinOUT(1,turn);         //ָ��1�Źܽ����Ϊ0&1
	if(p_s_flag==1)
		SEL_PLUS();
	else
		SEL_SUB();

	BCSCTL1&=0xF0;
	BCSCTL1|=SEL_temp;
	DCOCTL =DCO_temp+MOD_temp;
}
void I2C_IO11_Onclick()
{
	static unsigned char turn = 1;
	turn ^= BIT0;
	PinOUT(2,turn);         //ָ��2�Źܽ����Ϊ0&1
	PinOUT(3,turn);         //ָ��3�Źܽ����Ϊ0&1
	if(p_s_flag==1)
			DCO_PLUS();
	else
			DCO_SUB();

	BCSCTL1&=0xF0;
	BCSCTL1|=SEL_temp;
	DCOCTL =DCO_temp+MOD_temp;
}
void I2C_IO12_Onclick()
{
	static unsigned char turn = 1;
	turn ^= BIT0;
	PinOUT(5,turn);         //ָ��5�Źܽ����Ϊ0&1
	PinOUT(4,turn);         //ָ��4�Źܽ����Ϊ0&1
	if(p_s_flag==1)
			MOD_PLUS();
	else
			MOD_SUB();
	BCSCTL1&=0xF0;
	BCSCTL1|=SEL_temp;
	DCOCTL =DCO_temp+MOD_temp;
}
void I2C_IO13_Onclick()
{
	static unsigned char turn = 1;
	//turn ^= BIT0;
	//PinOUT(6,turn);         //ָ��6�Źܽ����Ϊ0&1
	//PinOUT(7,turn);         //ָ��7�Źܽ����Ϊ0&1
	if(p_s_flag==1)
	{
		p_s_flag=0;
		PinOUT(6,1);
		PinOUT(7,1);
	}
	else
	{
		p_s_flag=1;
		PinOUT(6,0);
		PinOUT(7,0);
	}

}
void I2C_IODect()			                 //����¼�ȷʵ������
{
	static unsigned char KEY_Past=0,KEY_Now=0;
	KEY_Past=KEY_Now;
	//----�ж�I2C_IO10������KEY1�����Ƿ񱻰���------
	if((TCA6416A_InputBuffer&BIT8) == BIT8)
		KEY_Now |=BIT0;
	else
		KEY_Now &=~BIT0;
	if(((KEY_Past&BIT0)==BIT0)&&(KEY_Now&BIT0) !=BIT0)
		I2C_IO10_Onclick();
	//----�ж�I2C_IO11������KEY2�����Ƿ񱻰���------
	if((TCA6416A_InputBuffer&BIT9)== BIT9)
		KEY_Now |=BIT1;
	else
		KEY_Now &=~BIT1;
	if(((KEY_Past&BIT1)==BIT1)&&(KEY_Now&BIT1)!=BIT1)
		I2C_IO11_Onclick();
	//----�ж�I2C_IO12������KEY3�����Ƿ񱻰���------
	if((TCA6416A_InputBuffer&BITA) == BITA)
		KEY_Now |=BIT2;
	else
		KEY_Now &=~BIT2;
	if(((KEY_Past&BIT2)==BIT2)&&(KEY_Now&BIT2) ==0)
	{
		I2C_IO12_Onclick();
	}
	//----�ж�I2C_IO13������KEY4�����Ƿ񱻰���------
	if((TCA6416A_InputBuffer&BITB) ==  BITB)
		KEY_Now |=BIT3;
	else
		KEY_Now &=~BIT3;
	if(((KEY_Past&BIT3) == BIT3)&& (KEY_Now&BIT3) == 0)    //
	{
		I2C_IO13_Onclick();
	}
}
