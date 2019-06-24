#include "exti.h"


/********************************************************************************************/
//EXTI0
void Exti0_Init(uint8_t EXTI_PortSourceGPIOx)
{
  //config
  EXTI_InitTypeDef EXTI_InitStructure;  
  NVIC_InitTypeDef NVIC_InitStructure; 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);        //ʹ��SYSCFGʱ��
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOx, EXTI_PinSource0); //���ӵ��ж���  
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //�½��ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //ʹ��
  EXTI_Init(&EXTI_InitStructure);                               //���� 
  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;         //�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //ʹ���ⲿ�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;              //�ⲿ�ж�
  NVIC_Init(&NVIC_InitStructure);                               //����
}

void (*Exti0IrqCall)(void) = NULL;
uint8_t Exti0IrqCall_Set(void (*func)(void))
{
  Exti0IrqCall = func;
  return 0;
}

void EXTI0_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line0))
  {
    EXTI_ClearITPendingBit(EXTI_Line0);
    
    if(Exti0IrqCall!=NULL)
      Exti0IrqCall();
  }
}

/********************************************************************************************/
//EXTI1
void Exti1_Init(uint8_t EXTI_PortSourceGPIOx)
{
  //config
  EXTI_InitTypeDef EXTI_InitStructure;  
  NVIC_InitTypeDef NVIC_InitStructure; 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);        //ʹ��SYSCFGʱ��
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOx, EXTI_PinSource1); //���ӵ��ж���1  
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //�½��ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //ʹ��
  EXTI_Init(&EXTI_InitStructure);                               //���� 
  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;         //�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //ʹ���ⲿ�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;              //�ⲿ�ж�1
  NVIC_Init(&NVIC_InitStructure);                               //����
}

void (*Exti1IrqCall)(void) = NULL;
uint8_t Exti1IrqCall_Set(void (*func)(void))
{
  Exti1IrqCall = func;
  return 0;
}

void EXTI1_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line1))
  {
    EXTI_ClearITPendingBit(EXTI_Line1);
    
    if(Exti1IrqCall!=NULL)
      Exti1IrqCall();
  }
}


/********************************************************************************************/
//EXTI2
void Exti2_Init(uint8_t EXTI_PortSourceGPIOx)
{
  //config
  EXTI_InitTypeDef EXTI_InitStructure;  
  NVIC_InitTypeDef NVIC_InitStructure; 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);        //ʹ��SYSCFGʱ��
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOx, EXTI_PinSource2); //���ӵ��ж���
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //������ 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //ʹ��
  EXTI_Init(&EXTI_InitStructure);                               //���� 
  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            //�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //ʹ���ⲿ�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;              //�ⲿ�ж�
  NVIC_Init(&NVIC_InitStructure);                               //����
}

void (*Exti2IrqCall)(void) = NULL;
uint8_t Exti2IrqCall_Set(void (*func)(void))
{
  Exti2IrqCall = func;
  return 0;
}

void EXTI2_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line2))
  {
    EXTI_ClearITPendingBit(EXTI_Line2);
    
    if(Exti2IrqCall!=NULL)
      Exti2IrqCall();
  }
}

/********************************************************************************************/
//EXTI9-5
void Exti9_5_Init(uint8_t EXTI_PortSourceGPIOx,uint8_t EXTI_PinSourceX)
{
  uint32_t EXTI_LineX = (1<<EXTI_PinSourceX);
  //config
  EXTI_InitTypeDef EXTI_InitStructure;  
  NVIC_InitTypeDef NVIC_InitStructure; 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);        //ʹ��SYSCFGʱ��
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOx, EXTI_PinSourceX); //���ӵ��ж���
  EXTI_InitStructure.EXTI_Line = EXTI_LineX;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //������ 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //ʹ��
  EXTI_Init(&EXTI_InitStructure);                               //���� 
  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;            //�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //ʹ���ⲿ�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;              //�ⲿ�ж�
  NVIC_Init(&NVIC_InitStructure);                               //����
}


void (*Exti9_5_IrqCall[5])(void)={NULL};

uint8_t Exti9_5_IrqCall_Set(uint8_t index,void (*func)(void))
{
  if((index<5)||(index>9)) return 1;
  Exti9_5_IrqCall[index-5] = func;
  return 0;
}


//�ⲿ�жϷ������
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line10)!=RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line10);//����жϱ�־λ  
    
    if(Exti9_5_IrqCall[0] != NULL)
      Exti9_5_IrqCall[0]();
  }
  if(EXTI_GetITStatus(EXTI_Line11)!=RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line11);//����жϱ�־λ  
    
    if(Exti9_5_IrqCall[1] != NULL)
      Exti9_5_IrqCall[1]();
  }
  if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line12);//����жϱ�־λ 
    
    if(Exti9_5_IrqCall[2] != NULL)
      Exti9_5_IrqCall[2]();
  }
  if(EXTI_GetITStatus(EXTI_Line13)!=RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line13);//����жϱ�־λ  
    
    if(Exti9_5_IrqCall[3] != NULL)
      Exti9_5_IrqCall[3]();
  }
  if(EXTI_GetITStatus(EXTI_Line14)!=RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line14);//����жϱ�־λ  
    
    if(Exti9_5_IrqCall[4] != NULL)
      Exti9_5_IrqCall[4]();
  }
}



/********************************************************************************************/
//EXTI10-15
void Exti15_10_Init(uint8_t EXTI_PortSourceGPIOx,uint8_t EXTI_PinSourceX)
{
  uint32_t EXTI_LineX = (1<<EXTI_PinSourceX);
  //config
  EXTI_InitTypeDef EXTI_InitStructure;  
  NVIC_InitTypeDef NVIC_InitStructure; 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);        //ʹ��SYSCFGʱ��
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOx, EXTI_PinSourceX); //���ӵ��ж���
  EXTI_InitStructure.EXTI_Line = EXTI_LineX;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //������ 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //ʹ��
  EXTI_Init(&EXTI_InitStructure);                               //���� 
  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;            //�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //ʹ���ⲿ�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;              //�ⲿ�ж�
  NVIC_Init(&NVIC_InitStructure);                               //����
}


void (*Exti15_10_IrqCall[6])(void)={NULL};

uint8_t Exti15_10_IrqCall_Set(uint8_t index,void (*func)(void))
{
  if((index<10)||(index>15)) return 1;
  Exti15_10_IrqCall[index-10] = func;
  return 0;
}


//�ⲿ�жϷ������
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line10)!=RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line10);//����жϱ�־λ  
    
    if(Exti15_10_IrqCall[0] != NULL)
      Exti15_10_IrqCall[0]();
  }
  if(EXTI_GetITStatus(EXTI_Line11)!=RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line11);//����жϱ�־λ  
    
    if(Exti15_10_IrqCall[1] != NULL)
      Exti15_10_IrqCall[1]();
  }
  if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line12);//����жϱ�־λ  
    if(Exti15_10_IrqCall[2] != NULL)
      Exti15_10_IrqCall[2]();
  }
  if(EXTI_GetITStatus(EXTI_Line13)!=RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line13);//����жϱ�־λ  
    
    if(Exti15_10_IrqCall[3] != NULL)
      Exti15_10_IrqCall[3]();
  }
  if(EXTI_GetITStatus(EXTI_Line14)!=RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line14);//����жϱ�־λ  
    
    if(Exti15_10_IrqCall[4] != NULL)
      Exti15_10_IrqCall[4]();
  }
  if(EXTI_GetITStatus(EXTI_Line15)!=RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line15);//����жϱ�־λ  
    
    if(Exti15_10_IrqCall[5] != NULL)
      Exti15_10_IrqCall[5]();
  }
}



