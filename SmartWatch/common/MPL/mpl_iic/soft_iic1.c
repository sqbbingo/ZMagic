#include "soft_iic.h"


/*********************************************************************************************
* ����:I2C1_GPIOInit()
* ����:��ʼ��I2C1
* ����:��
* ����:��
* �޸�:
* ע��:
*********************************************************************************************/
void I2C1_GPIOInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(I2C1_SCL_RCC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = I2C1_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(I2C1_SCL_GPIO, &GPIO_InitStructure);
  
    RCC_AHB1PeriphClockCmd(I2C1_SDA_RCC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = I2C1_SDA_PIN;
    GPIO_Init(I2C1_SDA_GPIO, &GPIO_InitStructure);

    SDA1_H;
    SCL1_H;
}
/*********************************************************************************************
* ����:I2C1_Start()
* ����:I2C1��ʼ�ź�
* ����:��
* ����:0
* �޸�:
* ע��:
*********************************************************************************************/
void I2C1_Start(void)
{
  SDA1_OUT;
  SDA1_H;
  SCL1_H;
  IIC_DelayUs(2);
  SDA1_L;
  IIC_DelayUs(2);
  SCL1_L;
}

/*********************************************************************************************
* ����:I2C1_Stop()
* ����:I2C1ֹͣ�ź�
* ����:��
* ����:��
* �޸�:
* ע��:
*********************************************************************************************/
void I2C1_Stop(void)
{
  SDA1_OUT;
  SCL1_L;
  SDA1_L;
  IIC_DelayUs(2);
  SCL1_H;
  SDA1_H;
  IIC_DelayUs(2);
}

/*********************************************************************************************
* ����:I2C1_Ack()
* ����:I2C1Ӧ���ź�
* ����:��
* ����:��
* �޸�:
* ע��:
*********************************************************************************************/
void I2C1_Ack(void)
{
  SCL1_L;
  SDA1_OUT;
  SDA1_L;
  IIC_DelayUs(2);
  SCL1_H;
  IIC_DelayUs(2);
  SCL1_L;
}

/*********************************************************************************************
* ����:I2C1_NoAck()
* ����:I2C1��Ӧ���ź�
* ����:��
* ����:��
* �޸�:
* ע��:
*********************************************************************************************/
void I2C1_NoAck(void)
{
  SCL1_L;
  SDA1_OUT;
  SDA1_H;
  IIC_DelayUs(2);
  SCL1_H;
  IIC_DelayUs(2);
  SCL1_L;
}

/*********************************************************************************************
* ����:I2C1_WaitAck()
* ����:I2C1�ȴ�Ӧ���ź�
* ����:��
* ����:����Ϊ:=1��ACK,=0��ACK
* �޸�:
* ע��:
*********************************************************************************************/
int I2C1_WaitAck(void)
{
  uint8_t ucErrTime=0;
  SDA1_H;
  SDA1_IN;
  IIC_DelayUs(2);
  SCL1_H;
  IIC_DelayUs(2);
  while (SDA1_R)
  {
    ucErrTime++;
    if(ucErrTime>250)
    {
      I2C1_Stop();
      return 1;
    }
  }
  SCL1_L;
  return 0;
}

/*********************************************************************************************
* ����:I2C1_WriteByte()
* ����:I2C1д�ֽ�����
* ����:char SendByte -- ��������
* ����:��
* �޸�:
* ע��:
*********************************************************************************************/
void I2C1_WriteByte(uint8_t SendByte) //���ݴӸ�λ����λ//
{
  uint8_t i=8;
  SDA1_OUT;
  while(i--)
  {
    if(SendByte&0x80) SDA1_H;
    else SDA1_L;
    SendByte<<=1;
    IIC_DelayUs(1);
    SCL1_H;
    IIC_DelayUs(2);
    SCL1_L;
    IIC_DelayUs(1);
  }
  SDA1_H;
}

/*********************************************************************************************
* ����:I2C1_ReadByte()
* ����:I2C1���ֽ����ݣ����ݴӸ�λ����λ
* ����:��
* ����:��
* �޸�:
* ע��:
*********************************************************************************************/
uint8_t I2C1_ReadByte(void)
{
  uint8_t i=8;
  uint8_t ReceiveByte=0;
  
  SDA1_H;
  SDA1_IN;
  while(i--)
  {
    ReceiveByte<<=1;
    SCL1_L;
    IIC_DelayUs(2);
    SCL1_H;
    IIC_DelayUs(2);
    if(SDA1_R)
    {
      ReceiveByte|=0x01;
    }
  }
  SCL1_L;
  return ReceiveByte;
}

