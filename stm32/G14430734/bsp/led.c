#include "main.h"
#include "led.h"
#include "stdio.h"
#include "tim.h"

extern int src_flag;	//屏幕标志位
extern int change_num;
	

int time1=0;//计时标志位
int time16=0;	//tim16计数器
int pinlv=0;	//PWM默认低频输出		0:低频，1：高频
int pinlv_lock=0;	//频率解锁，0解锁，1上锁
int R=1;	//
int K=1;
int R_K=0;	//0是调整R，1是调整K
int change_num=0;	//频率切换次数
int led_flag=0;
int my=0;
int pwm_lock_flag=0;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==htim17.Instance){	//周期1ms=0.001s
		time1++;
//软件定时器周期0.1s
		if(time1%100==0){//100ms=0.1s
			if(led_flag==0){
				led_Disp(0x03);
				led_flag=1;
			}else if(led_flag==1){
				led_Disp(0x01);
				led_flag=0;
			}

		}
		
//软件定时器周期5s
		if(time1%5000==0){	//5s输出频率不能改变
			led_Disp(0x01);
			pinlv_lock=0;	//解锁
			HAL_TIM_Base_Stop_IT(&htim17);
			time1=0;
		}
	}
	if(htim->Instance==htim16.Instance){	////周期1ms=0.001s
		time16++;
		if(time16==2000){
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==0){
				pwm_lock_flag=1;	//变更为锁定状态
				led_Disp(0x05);
			}
			
			time16=0;
			HAL_TIM_Base_Stop_IT(&htim16);
		}
	}
}




void led_Disp(unsigned char ds_LED){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,ds_LED<<8,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

int scanKey(void){
	int c=0;
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==0){
		c=1;
	}else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==0){
		c=2;
	}else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==0){
		c=3;
	}else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==0){
		c=4;
	}
	return c;
}

void key_proc(){
	static __IO uint32_t KeyTick;
	static unsigned char key_old;
	unsigned char key_value;
	unsigned char key_down;
	
	if(uwTick -KeyTick<50){
		return;
	}
	KeyTick = uwTick;
	
	key_value = scanKey();
	key_down=key_value&(key_value^key_old);
	key_old=key_value;
	
	switch(key_down){
		case 1://按键1
			if(src_flag==0){
				src_flag=1;
			}else if(src_flag==1){
				src_flag=2;
			}else if(src_flag==2){
				src_flag=0;
			}
		break;
		
		case 2:	//按键2
			if(src_flag==0){	//在数据界面下，用于切换选择低频或高频模式。
								//按键按下后，5 秒内不可再次触发切换功能。
				HAL_TIM_Base_Start_IT(&htim17);
				my=1;
				
				if(pinlv==0&&pinlv_lock==0){//低频
					pinlv=1;//输出高频
					change_num++;
					
					pinlv_lock=1;	//频率上锁
					
					
				}else if(pinlv==1&&pinlv_lock==0){//高频
					pinlv=0;//输出低频
					change_num++;
					pinlv_lock=1;	//频率上锁
				}

				
				
			}else if(src_flag==1){		/*在参数界面下，按下 B2 按键，切换选择 R 或 K 参数。每次从数据界面进入参数界面，
								默认当前可调整的参数为 R 参数；
								从参数界面退出时，新的 R 参数和 K 参数生效。!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
				if(R_K==0){
					R_K=1;
				}else if(R_K==1){
					R_K=0;
				}
			}
		
		
		break;
			
		//按键3
		case 3:
//			led_Disp(0x02);
			if(src_flag==1){
				if(R_K==0){
				R++;
				if(R>10){
					R=1;
				}
			}else if(R_K==1){
				K++;
				if(K>10){
					K=1;
				}
			}
			}
			
		break;
		
		//按键4
		case 4:
			
		/*在数据界面下，长按 B4 按键超过 2 秒后松开（长按键），可以“锁定”
		占空比调整功能，此时输出信号占空比保持不变，不受 R37 电位器输出电
		压控制；处于“锁定”状态后，再次按下 B4 按键（短按键），实现“解锁” 
		功能，恢复 R37 电位器对输出信号占空比的控制。*/
			if(src_flag==1){
				if(R_K==0){
				R--;
				if(R<1){
					R=10;
				}
			}else if(R_K==1){
				K--;
				if(K<1){
					K=10;
				}
			}
			}else if(src_flag==0){	//数据界面
				HAL_TIM_Base_Start_IT(&htim16);
				if(pwm_lock_flag==1){
					pwm_lock_flag=0;
					led_Disp(0x01);
					//未完成：恢复R37 电位器对输出信号占空比的控制
				}
			}
			
		break;
	}
		
	
}


