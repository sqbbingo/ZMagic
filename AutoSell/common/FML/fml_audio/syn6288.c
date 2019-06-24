/*********************************************************************************************
* �ļ���syn6288.c
* ���ߣ�
* ˵����syn6288��������
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include "syn6288.h"
/*********************************************************************************************
* ���ƣ�uart_send_char()
* ���ܣ����ڷ����ֽں���
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void uart_send_char(unsigned char ch)
{
  uart6_putc(ch);
}

void uart_sendString(unsigned char *data, unsigned char strLen)
{
  for(unsigned char i=0; i<strLen; i++)
  {
    uart_send_char(data[i]);
  }
}

/*********************************************************************************************
* ���ƣ�syn6288_init()
* ���ܣ�syn6288��ʼ��
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void syn6288_init()
{
//  GPIO_InitTypeDef  GPIO_InitStructure;
 	
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  uart6_init(9600); 
}

int syn6288_busy(void)
{ 
//  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))                                                      //æ�������
    return 0;                                                   //û�м�⵽�źŷ��� 0
//  else
//    return 1;                                                   //��⵽�źŷ��� 1
}

/*********************************************************************************************
* ���ƣ�syn6288_play()
* ���ܣ�
* ������s -- ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void syn6288_play(char *s)
{
  int i;
  int len = strlen(s);
  unsigned char c = 0;  
  unsigned char head[] = {0xFD,0x00,0x00,0x01,0x00};            //���ݰ�ͷ  
  head[1] = (len + 3) >> 8;
  head[2] = (len + 3) & 0xff;
  for (i=0; i<5; i++){
    uart_send_char(head[i]);
    c ^= head[i];
  }
  for (i=0; i<len; i++){
    uart_send_char(s[i]);
    c ^= s[i];
  }
  uart_send_char(c);
}

/*******************************************************************************
* ����: hex2unicode()
* ����: ��16����unicode�ַ���ת����bin��ʽ
* ����: 
* ����: 
* �޸�:
* ע��: 
*******************************************************************************/
char *hex2unicode(char *str)
{       
  static char uni[64];
  int n = strlen(str)/2;
  if (n > 64) n = 64;
  
  for (int i=0; i<n; i++) {
    unsigned int x = 0;
    for (int j=0; j<2; j++) {
      char c = str[2*i+j];
      char o;
      if (c>='0' && c<='9') o = c - '0';
      else if (c>='A' && c<='F') o = 10+(c-'A');
      else if (c>='a' && c<='f') o = 10+(c-'a');
      else o = 0;
      x = (x<<4) | (o&0x0f);
    }
    uni[i] = x;
  }
  uni[n] = 0;
  return uni;
}

/*******************************************************************************
* syn6288_play_unicode()
* ���ܣ�
* ������s -- ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*******************************************************************************/
void syn6288_play_unicode(uint16_t *s, int len)
{
  int i;
  char c = 0;  
  unsigned char head[] = {0xFD,0x00,0x00,0x01,0x03};            //���ݰ�ͷ 
  
  head[1] = (len*2 + 3) >> 8;
  head[2] = (len*2 + 3) & 0xff;
  for (i=0; i<5; i++)
  {
    uart_send_char(head[i]);
    c ^= head[i];
  }
  for (i=0; i<len; i++)
  {
    uart_send_char(s[i]>>8);
    c ^= s[i]>>8;
    uart_send_char(s[i]&0xff);
    c ^= s[i]&0xff;
  }
  uart_send_char(c);
}

/*******************************************************************************
* ���ƣ�syn6288_stop()
* ���ܣ�ֹͣ�ϳ�
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*******************************************************************************/
void syn6288_stop(void)
{
  unsigned char orderBuf[5] = {0xFD, 0x00, 0x02, 0x02, 0xFD};
  uart_sendString(orderBuf, 5);
}

/*******************************************************************************
* ���ƣ�syn6288_suspend()
* ���ܣ���ͣ�ϳ�
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*******************************************************************************/
void syn6288_suspend(void)
{
  unsigned char orderBuf[5] = {0xFD, 0x00, 0x02, 0x03, 0xFC};
  uart_sendString(orderBuf, 5);
}

/*******************************************************************************
* ���ƣ�syn6288_suspend()
* ���ܣ��ָ��ϳ�
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*******************************************************************************/
void syn6288_continue(void)
{
  unsigned char orderBuf[5] = {0xFD, 0x00, 0x02, 0x04, 0xFD};
  uart_sendString(orderBuf, 5);
}

/*******************************************************************************
* ���ƣ�syn6288_playMusic()
* ���ܣ��ϳ���������������
* ������musicNum-��������(0-15) �ϳ��ַ���-str
* ���أ�
* �޸ģ�
* ע�ͣ�
*******************************************************************************/
void syn6288_playMusic(unsigned char musicNum, char *str)
{
  unsigned char xorChar = 0;
  int len = strlen(str);
  if(musicNum >= 15)
    musicNum = 15;
  musicNum <<= 3;
  char orderBuf[] = {0xFD, 0x00, 0x00, 0x01, musicNum};
  orderBuf[1] = (len + 3) >> 8;
  orderBuf[2] = (len + 3) & 0xFF;
  for(unsigned char i=0; i<5; i++){
    uart_send_char(orderBuf[i]);
    xorChar ^= orderBuf[i];
  }
  for(unsigned char i=0; i<len; i++)
  {
    uart_send_char(str[i]);
    xorChar ^= str[i];
  }
  uart_send_char(xorChar);
}

/*******************************************************************************
* ���ƣ�syn6288_playHint()
* ���ܣ�������ʾ��
* ������hintNum-��ʾ�����(0-25)
* ���أ�
* �޸ģ�
* ע�ͣ�
*******************************************************************************/
void syn6288_playHint(unsigned char hintNum)
{
  unsigned char xorChar = 0;
  unsigned char orderBuf[5] = {0xFD, 0x00, 0x0D, 0x01, 0x01};
  if(hintNum >= 25)
    hintNum = 25; 
  unsigned char strBuf[10] = {0x5B, 0x78, 0x31, 0x5D, 0x73, 0x6F, 0x75, 0x6e, 0x64, hintNum+0x60};
  for(unsigned char i=0; i<5; i++)
  {
    uart_send_char(orderBuf[i]);
    xorChar ^= orderBuf[i];
  }
  for(unsigned char i=0; i<10; i++)
  {
    uart_send_char(strBuf[i]);
    xorChar ^= strBuf[i];
  }
  uart_send_char(xorChar);
}

/*******************************************************************************
* ���ƣ�syn6288_setVolume()
* ���ܣ���������
* ������vValue-�ϳ����� mValue-������������
* ���أ�
* �޸ģ�
* ע�ͣ�
*******************************************************************************/
void syn6288_setVolume(unsigned char vValue, unsigned char mValue)
{
  unsigned char xorChar = 0;
  unsigned char orderBuf[5] = {0xFD, 0x00, 0x0D, 0x01, 0x01};
  if(vValue >= 16)
    vValue = 16;
  if(mValue >= 16)
    mValue = 16;
  unsigned char configBuf[10] = {0x5B, 0x76, vValue/10+0x30, vValue%10+0x30, 
                                  0x5D, 0x5B, 0x6D, mValue/10+0x30, mValue%10+0x30, 0x5D};
  for(unsigned char i=0; i<5; i++)
  {
    uart_send_char(orderBuf[i]);
    xorChar ^= orderBuf[i];
  }
  for(unsigned char i=0; i<10; i++)
  {
    uart_send_char(configBuf[i]);
    xorChar ^= configBuf[i];
  }
  uart_send_char(xorChar);
}















