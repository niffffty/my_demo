#include "my_pwm.h"
#include "main.h"
#include "led.h"
#include "stdio.h"
#include "tim.h"

extern int pinlv;


void pwm_proc(void){
	if(pinlv==0){//��Ƶ
//		led_Disp(0x01);
		__HAL_TIM_SetAutoreload(&htim2,250-1);
	}else if(pinlv==1){//��Ƶ
//		led_Disp(0x02);
		__HAL_TIM_SetAutoreload(&htim2,125-1);
	}
}


