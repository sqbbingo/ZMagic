#include <stdio.h>
#include "pcf8563.h"
#include "soft_iic.h"

/*********************************************************************************************
* ����:pcf8563_readReg()
* ����:��ȡPCF8563�Ĵ���
* ����:regAddr-�Ĵ�����ַ
* ����:data-��ȡ����
* �޸�:
* ע��:
*********************************************************************************************/
int pcf8563_readReg(unsigned char regAddr)
{
  unsigned char data;
  I2C_Start();
  I2C_WriteByte(PCF8563_ADDR & 0xFE);
  if(I2C_WaitAck()) return -1;
  I2C_WriteByte(regAddr);
  if(I2C_WaitAck()) return -1;
  I2C_Start();
  I2C_WriteByte(PCF8563_ADDR | 0x01);
  if(I2C_WaitAck()) return -1;
  data = I2C_ReadByte();
  if(I2C_WaitAck()) return -1;
  I2C_Stop();
  return data;
}

/*********************************************************************************************
* ����:pcf8563_writeReg()
* ����:д��PCF8563�Ĵ���
* ����:regAddr-�Ĵ�����ַ data-д�������
* ����: 0/-1 �ɹ�/ʧ��
* �޸�:
* ע��:
*********************************************************************************************/
int pcf8563_writeReg(unsigned char regAddr, unsigned char data)
{
  I2C_Start();
  I2C_WriteByte(0XA2 & 0xFE);
  if(I2C_WaitAck()) return -1;
  I2C_WriteByte(regAddr);
  if(I2C_WaitAck()) return -1;
  I2C_WriteByte(data);
  if(I2C_WaitAck()) return -1;
  I2C_Stop();
  return 0;
}

/*********************************************************************************************
* ����:pcf8563_readRegs()
* ����:��ȡPCF8563�Ĵ����������
* ����:regAddr-�Ĵ�����ַ readLen-��ȡ���� buf-����
* ����:buf-����
* �޸�:
* ע��:
*********************************************************************************************/
unsigned char *pcf8563_readRegs(unsigned char regAddr ,unsigned char readLen, unsigned char *buf)
{
  *buf = NULL;
  I2C_Start();                
  I2C_WriteByte(PCF8563_ADDR & 0xFE);       
  if (I2C_WaitAck()) return NULL;
  I2C_WriteByte(regAddr);        
  if ( I2C_WaitAck()) return NULL;
  I2C_Start();                
  I2C_WriteByte(PCF8563_ADDR | 0x01); 
  if (I2C_WaitAck()) return NULL;
  for(unsigned char i=0; i<readLen; i++)
  {
    buf[i] = I2C_ReadByte();
    if(i < readLen-1)             
      I2C_Ack();
  }
  I2C_NoAck();          
  I2C_Stop();
  return buf;
}

/*********************************************************************************************
* ����:pcf8563_init()
* ����:PCF8563��ʼ��
* ����:��
* ����:��
* �޸�:
* ע��:
*********************************************************************************************/
void pcf8563_init(void)
{
  I2C_GPIOInit();
  pcf8563_writeReg(0x00, 0x00);                                 // 0x00,0x01Ϊ����/״̬�Ĵ�����ַ
  pcf8563_writeReg(0x01, 0x12);                                 // ����,�����ж���Ч
  pcf8563_writeReg(0x09, 0x80);                                 // �رշ��ӱ���  
  pcf8563_writeReg(0x0A, 0x80);                                 // �ر�Сʱ����
  pcf8563_writeReg(0x0B, 0x80);                                 // �ر��ձ���
  pcf8563_writeReg(0x0C, 0x80);                                 // �ر����ڱ���
  pcf8563_writeReg(0x0D, 0x00);                                 // �ر�CLKOUT���
  pcf8563_writeReg(0x0E, 0x03);                                 // �رն�ʱ��
}

/*********************************************************************************************
* ����:get_pcf8563Time()
* ����:��ȡPCF8563��ǰʱ��
* ����:��
* ����:date-��ȡ����ʱ��,��ת��Ϊʵ��ֵ
* �޸�:
* ע��:
*********************************************************************************************/
unsigned char get_pcf8563Time(unsigned char* date)
{
  unsigned char buf[8] = {0};
  if(pcf8563_readRegs(0x02, 7, buf))
  {
    date[0] = ((buf[0] >> 4)&0x07)*10 + (buf[0]&0x0F);       // ��
    date[1] = ((buf[1] >> 4)&0x07)*10 + (buf[1]&0x0F);       // ��
    date[2] = ((buf[2] >> 4)&0x03)*10 + (buf[2]&0x0F);       // ʱ
    date[3] = ((buf[3] >> 4)&0x03)*10 + (buf[3]&0x0F);       // ��
    date[4] = buf[4] & 0x07;                                 // ��,0-6,0-������
    date[5] = ((buf[5]>>4)&0x01)*10 + (buf[5]&0x0F);         // ��
    date[6] = ((buf[6]>>4)&0x0F)*10 + (buf[6]&0x0F);         // ��,��ֵ��2000
    return 0;
  }
  return 1;
}

/*********************************************************************************************
* ����:set_pcf8563Time()
* ����:����PCF8563ʱ��
* ����:buf-��Ҫ�޸ĵĲ���
* ����:data-0/-1 �ɹ�/ʧ��
* �޸�:
* ע��:����ΪBCD��
*       ���磺timeBuf[7] = {0x55, 0x56, 0x20, 0x07, 0x04, 0x03, 0x19} // �� �� ʱ �� �� �� ��
*********************************************************************************************/
int set_pcf8563Time(unsigned char *buf)
{
  unsigned char i=0;
  for(i=0; i<7; i++)
  {
    buf[i]+=(buf[i]/10)*6;
  }
  for(i=0; i<7; i++)
  {
    int status = pcf8563_writeReg(0x02 + i, buf[i]);
    if(status < 0)
      return -1;
  }
  return 0;
}











