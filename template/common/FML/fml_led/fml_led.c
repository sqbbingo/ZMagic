/*********************************************************************************************
* �ļ���fml_led.c
* ���ߣ�
* ˵����
* ���ܣ�
* �޸ģ�
*********************************************************************************************/
/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include "fml_led/fml_led.h"

/*********************************************************************************************
* ���ƣ�
* ���ܣ���ʼ��
* ��������
* ���أ���
* �޸ģ���
*********************************************************************************************/
void Bsp_LedInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;                          //����һ��GPIO_InitTypeDef���͵Ľṹ��
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;               //��������ģʽΪ����ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;              //�������ŵ��������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                //��������Ϊ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;          //������������Ϊ100MHz
  
  RCC_AHB1PeriphClockCmd(LED1_RCC, ENABLE);                    
  GPIO_InitStructure.GPIO_Pin = LED1_PIN;                       
  GPIO_Init(LED1_CPIO, &GPIO_InitStructure);                    
  
  RCC_AHB1PeriphClockCmd(LED2_RCC, ENABLE);                    
  GPIO_InitStructure.GPIO_Pin = LED2_PIN;                       
  GPIO_Init(LED2_CPIO, &GPIO_InitStructure);                    
  
  RCC_AHB1PeriphClockCmd(LED3_RCC, ENABLE);                    
  GPIO_InitStructure.GPIO_Pin = LED3_PIN;                       
  GPIO_Init(LED3_CPIO, &GPIO_InitStructure);
  
  RCC_AHB1PeriphClockCmd(LED4_RCC, ENABLE);                    
  GPIO_InitStructure.GPIO_Pin = LED4_PIN;                       
  GPIO_Init(LED4_CPIO, &GPIO_InitStructure); 
  
  Bsp_LedOff(1);
  Bsp_LedOff(2);
  Bsp_LedOff(3);
  Bsp_LedOff(4);
}


/*********************************************************************************************
* ���ƣ�
* ���ܣ�
* ������
* ���أ�
* �޸ģ�
*********************************************************************************************/
void Bsp_LedOn(uint8_t index)
{
  switch(index)
  {
  case 1:
    GPIO_ResetBits(LED1_CPIO,LED1_PIN);
    break;
  case 2:
    GPIO_ResetBits(LED2_CPIO,LED2_PIN);
    break;
  case 3:
    GPIO_ResetBits(LED3_CPIO,LED3_PIN);
    break;
  case 4:
    GPIO_ResetBits(LED4_CPIO,LED4_PIN);
    break;
  }
}

/*********************************************************************************************
* ���ƣ�
* ���ܣ�
* ������
* ���أ�
* �޸ģ�
*********************************************************************************************/
void Bsp_LedOff(uint8_t index)
{
  switch(index)
  {
  case 1:
    GPIO_SetBits(LED1_CPIO,LED1_PIN);
    break;
  case 2:
    GPIO_SetBits(LED2_CPIO,LED2_PIN);
    break;
  case 3:
    GPIO_SetBits(LED3_CPIO,LED3_PIN);
    break;
  case 4:
    GPIO_SetBits(LED4_CPIO,LED4_PIN);
    break;
  }
}


/*********************************************************************************************
* ���ƣ�
* ���ܣ�
* ������
* ���أ�
* �޸ģ�
*********************************************************************************************/
void Bsp_LedTurn(uint8_t index)
{
  switch(index)
  {
  case 1:
    if(GPIO_ReadInputDataBit(LED1_CPIO,LED1_PIN))
      GPIO_ResetBits(LED1_CPIO,LED1_PIN);
    else
      GPIO_SetBits(LED1_CPIO,LED1_PIN);
    break;
  case 2:
    if(GPIO_ReadInputDataBit(LED2_CPIO,LED2_PIN))
      GPIO_ResetBits(LED2_CPIO,LED2_PIN);
    else
      GPIO_SetBits(LED2_CPIO,LED2_PIN);
    break;
  case 3:
    if(GPIO_ReadInputDataBit(LED3_CPIO,LED3_PIN))
      GPIO_ResetBits(LED3_CPIO,LED3_PIN);
    else
      GPIO_SetBits(LED3_CPIO,LED3_PIN);
    break;
  case 4:
    if(GPIO_ReadInputDataBit(LED4_CPIO,LED4_PIN))
      GPIO_ResetBits(LED4_CPIO,LED4_PIN);
    else
      GPIO_SetBits(LED4_CPIO,LED4_PIN);
    break;
  }
}


