/*********************************************************************************************
* �ļ���fml_key.h
* ���ߣ�fuyou
* ˵���������Ĺ���ģ������ͷ�ļ�
* ���ܣ�
* �޸ģ�
*********************************************************************************************/
#ifndef __FML_KEY_H__
#define __FML_KEY_H__

#include "stm32f4xx.h"

/*********************************************************************************************
* �궨��
*********************************************************************************************/
#define K1_PIN                GPIO_Pin_3                        //�궨��K1�ܽ�ΪKEY1_PIN
#define K1_PORT               GPIOC                             //�궨��K1ͨ��ΪKEY1_PORT
#define K1_CLK                RCC_AHB1Periph_GPIOC              //�궨��KY1ʱ��ΪKEY1_CLK

#define K2_PIN                GPIO_Pin_3                        //�궨��K2�ܽ�ΪKEY2_PIN
#define K2_PORT               GPIOE                             //�궨��K2ͨ��ΪKEY2_PORT
#define K2_CLK                RCC_AHB1Periph_GPIOE              //�궨��K2ʱ��ΪKEY2_CLK

#define K3_PIN                GPIO_Pin_1                        //�궨��K3�ܽ�ΪKEY3_PIN
#define K3_PORT               GPIOA                             //�궨��K3ͨ��ΪKEY3_PORT
#define K3_CLK                RCC_AHB1Periph_GPIOA              //�궨��K3ʱ��ΪKEY3_CLK

#define K1_PRESSED            0x01                              //�궨��K1���ֱ��
#define K2_PRESSED            0x02                              //�궨��K2���ֱ��
#define K3_PRESSED            0x04                              //�궨��K3���ֱ��
/********************************************************************************************/

void key_GpioInit(void);
uint8_t key_GetStatus(void);

#endif
