#include "main.h"
#include "led.h"
#include "stdio.h"
#include "tim.h"

extern int src_flag;	//��Ļ��־λ
extern int change_num;
	

int time1=0;//��ʱ��־λ
int time16=0;	//tim16������
int pinlv=0;	//PWMĬ�ϵ�Ƶ���		0:��Ƶ��1����Ƶ
int pinlv_lock=0;	//Ƶ�ʽ�����0������1����
int R=1;	//
int K=1;
int R_K=0;	//0�ǵ���R��1�ǵ���K
int change_num=0;	//Ƶ���л�����
int led_flag=0;
int my=0;
int pwm_lock_flag=0;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==htim17.Instance){	//����1ms=0.001s
		time1++;
//�����ʱ������0.1s
		if(time1%100==0){//100ms=0.1s
			if(led_flag==0){
				led_Disp(0x03);
				led_flag=1;
			}else if(led_flag==1){
				led_Disp(0x01);
				led_flag=0;
			}

		}
		
//�����ʱ������5s
		if(time1%5000==0){	//5s���Ƶ�ʲ��ܸı�
			led_Disp(0x01);
			pinlv_lock=0;	//����
			HAL_TIM_Base_Stop_IT(&htim17);
			time1=0;
		}
	}
	if(htim->Instance==htim16.Instance){	////����1ms=0.001s
		time16++;
		if(time16==2000){
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==0){
				pwm_lock_flag=1;	//���Ϊ����״̬
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
		case 1://����1
			if(src_flag==0){
				src_flag=1;
			}else if(src_flag==1){
				src_flag=2;
			}else if(src_flag==2){
				src_flag=0;
			}
		break;
		
		case 2:	//����2
			if(src_flag==0){	//�����ݽ����£������л�ѡ���Ƶ���Ƶģʽ��
								//�������º�5 ���ڲ����ٴδ����л����ܡ�
				HAL_TIM_Base_Start_IT(&htim17);
				my=1;
				
				if(pinlv==0&&pinlv_lock==0){//��Ƶ
					pinlv=1;//�����Ƶ
					change_num++;
					
					pinlv_lock=1;	//Ƶ������
					
					
				}else if(pinlv==1&&pinlv_lock==0){//��Ƶ
					pinlv=0;//�����Ƶ
					change_num++;
					pinlv_lock=1;	//Ƶ������
				}

				
				
			}else if(src_flag==1){		/*�ڲ��������£����� B2 �������л�ѡ�� R �� K ������ÿ�δ����ݽ������������棬
								Ĭ�ϵ�ǰ�ɵ����Ĳ���Ϊ R ������
								�Ӳ��������˳�ʱ���µ� R ������ K ������Ч��!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
				if(R_K==0){
					R_K=1;
				}else if(R_K==1){
					R_K=0;
				}
			}
		
		
		break;
			
		//����3
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
		
		//����4
		case 4:
			
		/*�����ݽ����£����� B4 �������� 2 ����ɿ����������������ԡ�������
		ռ�ձȵ������ܣ���ʱ����ź�ռ�ձȱ��ֲ��䣬���� R37 ��λ�������
		ѹ���ƣ����ڡ�������״̬���ٴΰ��� B4 �������̰�������ʵ�֡������� 
		���ܣ��ָ� R37 ��λ��������ź�ռ�ձȵĿ��ơ�*/
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
			}else if(src_flag==0){	//���ݽ���
				HAL_TIM_Base_Start_IT(&htim16);
				if(pwm_lock_flag==1){
					pwm_lock_flag=0;
					led_Disp(0x01);
					//δ��ɣ��ָ�R37 ��λ��������ź�ռ�ձȵĿ���
				}
			}
			
		break;
	}
		
	
}


