#include "soft_iic.h"


/*********************************************************************************************
* ����:I2C2_GPIOInit()
* ����:��ʼ��I2C2
* ����:��
* ����:��
* �޸�:
* ע��:
*********************************************************************************************/
void I2C2_GPIOInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd(I2C2_SCL_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = I2C2_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(I2C2_SCL_GPIO, &GPIO_InitStructure);
  
  RCC_AHB1PeriphClockCmd(I2C2_SDA_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = I2C2_SDA_PIN;
  GPIO_Init(I2C2_SDA_GPIO, &GPIO_InitStructure);
  
  SDA2_H;
  SCL2_H;
}
/*********************************************************************************************
* ����:I2C2_Start()
* ����:I2C2��ʼ�ź�
* ����:��
* ����:0
* �޸�:
* ע��:
*********************************************************************************************/
void I2C2_Start(void)
{
  SDA2_OUT;
  SDA2_H;
  SCL2_H;
  IIC_DelayUs(2);
  SDA2_L;
  IIC_DelayUs(2);
  SCL2_L;
}

/*********************************************************************************************
* ����:I2C2_Stop()
* ����:I2C2ֹͣ�ź�
* ����:��
* ����:��
* �޸�:
* ע��:
*********************************************************************************************/
void I2C2_Stop(void)
{
  SDA2_OUT;
  SCL2_L;
  SDA2_L;
  IIC_DelayUs(2);
  SCL2_H;
  SDA2_H;
  IIC_DelayUs(2);
}

/*********************************************************************************************
* ����:I2C2_Ack()
* ����:I2C2Ӧ���ź�
* ����:��
* ����:��
* �޸�:
* ע��:
*********************************************************************************************/
void I2C2_Ack(void)
{
  SCL2_L;
  SDA2_OUT;
  SDA2_L;
  IIC_DelayUs(2);
  SCL2_H;
  IIC_DelayUs(2);
  SCL2_L;
}

/*********************************************************************************************
* ����:I2C2_NoAck()
* ����:I2C2��Ӧ���ź�
* ����:��
* ����:��
* �޸�:
* ע��:
*********************************************************************************************/
void I2C2_NoAck(void)
{
  SCL2_L;
  SDA2_OUT;
  SDA2_H;
  IIC_DelayUs(2);
  SCL2_H;
  IIC_DelayUs(2);
  SCL2_L;
}

/*********************************************************************************************
* ����:I2C2_WaitAck()
* ����:I2C2�ȴ�Ӧ���ź�
* ����:��
* ����:����Ϊ:=1��ACK,=0��ACK
* �޸�:
* ע��:
*********************************************************************************************/
int I2C2_WaitAck(void)
{
  uint8_t ucErrTime=0;
  SDA2_H;
  SDA2_IN;
  IIC_DelayUs(2);
  SCL2_H;
  IIC_DelayUs(2);
  while (SDA2_R)
  {
    ucErrTime++;
    if(ucErrTime>250)
    {
      I2C2_Stop();
      return 1;
    }
  }
  SCL2_L;
  return 0;
}

/*********************************************************************************************
* ����:I2C2_WriteByte()
* ����:I2C2д�ֽ�����
* ����:char SendByte -- ��������
* ����:��
* �޸�:
* ע��:
*********************************************************************************************/
void I2C2_WriteByte(uint8_t SendByte) //���ݴӸ�λ����λ//
{
  uint8_t i=8;
  SDA2_OUT;
  while(i--)
  {
    if(SendByte&0x80) SDA2_H;
    else SDA2_L;
    SendByte<<=1;
    IIC_DelayUs(1);
    SCL2_H;
    IIC_DelayUs(2);
    SCL2_L;
    IIC_DelayUs(1);
  }
  SDA2_H;
}

/*********************************************************************************************
* ����:I2C2_ReadByte()
* ����:I2C2���ֽ����ݣ����ݴӸ�λ����λ
* ����:��
* ����:��
* �޸�:
* ע��:
*********************************************************************************************/
uint8_t I2C2_ReadByte(void)
{
  uint8_t i=8;
  uint8_t ReceiveByte=0;
  
  SDA2_H;
  SDA2_IN;
  while(i--)
  {
    ReceiveByte<<=1;
    SCL2_L;
    IIC_DelayUs(2);
    SCL2_H;
    IIC_DelayUs(2);
    if(SDA2_R)
    {
      ReceiveByte|=0x01;
    }
  }
  SCL2_L;
  return ReceiveByte;
}

