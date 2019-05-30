/*
 * Breath_Lamp_Event.c
 *
 *  Created on: 2019-5-30
 *      Author: moc
 */

#include "MSP430G2553.h"
#include "Breath_Lamp_Global.h"
#include "TA_PWM.h"

void WDT_Ontime()
{
	static int Bright_Delta=0;
	TA0_PWM_SetPermill(1,Bright);
	if(Bright>=400) //占空比最大40%，更亮的区间视觉变化不明显
		Bright_Delta=-5;
	if(Bright<=10) //变亮
		Bright_Delta=5;
	Bright=Bright+Bright_Delta;
	_nop();
}
