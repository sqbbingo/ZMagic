/*********************************************************************************************
* �ļ���
* ���ߣ�
* ˵����
* ���ܣ�
* �޸ģ�
*********************************************************************************************/
#ifndef __FML_LED_H__
#define __FML_LED_H__

#include "stm32f4xx.h"

/*********************************************************************************************
* �궨��
*********************************************************************************************/
#define LED1_RCC                RCC_AHB1Periph_GPIOE              //�궨��LED1ʱ��
#define LED1_CPIO               GPIOE                             //�궨��LED1 GPIO
#define LED1_PIN                GPIO_Pin_0                        //�궨��LED1�ܽ�

#define LED2_RCC                RCC_AHB1Periph_GPIOE              //�궨��LED1ʱ��
#define LED2_CPIO               GPIOE                             //�궨��LED1 GPIO
#define LED2_PIN                GPIO_Pin_1                        //�궨��LED1�ܽ�

#define LED3_RCC                RCC_AHB1Periph_GPIOE              //�궨��LED1ʱ��
#define LED3_CPIO               GPIOE                             //�궨��LED1 GPIO
#define LED3_PIN                GPIO_Pin_2                        //�궨��LED1�ܽ�

#define LED4_RCC                RCC_AHB1Periph_GPIOE              //�궨��LED1ʱ��
#define LED4_CPIO               GPIOD                             //�궨��LED1 GPIO
#define LED4_PIN                GPIO_Pin_3                        //�궨��LED1�ܽ�

/********************************************************************************************/

void Bsp_LedInit(void);
void Bsp_LedOn(uint8_t index);
void Bsp_LedOff(uint8_t index);
void Bsp_LedTurn(uint8_t index);

#endif
