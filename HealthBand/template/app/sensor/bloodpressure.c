#include "bloodpressure.h"
#include "usart.h"

#if (BP_VERSION == 2)
int heartrate_p = 0;
#endif

//����ѹ������ѹ������
int16_t SBP=0, DBP=0, HR=0;

int16_t SBP_Get()
{
  return SBP;
}

int16_t DBP_Get()
{
  return DBP;
}

int16_t HR_Get()
{
  return HR;
}

#if BP_VERSION==1
static void package_decode(char *p)
{
  printf("%s \r\n[", p);
  
  for (int i=0; i<strlen(p); i++)
  {
    printf("%02X ", p[i]);
  }
  printf("]\r\n");
  if (strcmp(p, "999") == 0)
  {
    //process_post(&blood, ev_finish, NULL);
    blood_result();
  }
  if (memcmp(p, "S1", 2) == 0)
  {
    if (memcmp(&p[10], "M03", 3) == 0)
    {
      char p1[4], p2[4], p3[4];
      p1[0] = p[15];
      p1[1] = p[16];
      p1[2] = p[17];
      p1[3] = 0;
      
      p2[0] = p[18];
      p2[1] = p[19];
      p2[2] = p[20];
      p2[3] = 0;
      
      p3[0] = p[21];
      p3[1] = p[22];
      p3[2] = p[23];
      p3[3] = 0;
      
      SBP = atoi(p1);
      DBP = atoi(p2);
      HR = atoi(p3);
    }
  }
}
#endif

static int _on_recv(char ch)
{
  static char rxbuf[256];
  static int rlen = 0;
  static unsigned char error = 0;
  static unsigned char flag = 0;
#if (BP_VERSION == 1)
  static int offset = 0;
  if (ch == 0x0D) offset = 0;
  
  if (offset == 0 && ch == 0x02)
  {
    rxbuf[0] = 0x02;
    offset = 1;
  }
  else if (offset > 0 && offset<256)
  {
    rxbuf[offset++] = ch;
    if (ch == 0x03)
    {
      rxbuf[offset-1] = '\0';
      package_decode(&rxbuf[1]);
    }
  }
  else
  {
    offset = 0;
  }
#elif (BP_VERSION == 2)
  if(ch == 0xFF && flag == 0) {
    flag = 1;
  }
  else if(ch == 0xCD && flag == 1) {
    flag = 2;
  }
  else if(ch == 0x08 && flag == 2) {
    flag = 3;
  }
  else if(ch == 0x04 && flag == 2) {
    error = 1;
  }
  else if(error == 1){                                        // ���󱨸����
    rxbuf[rlen++] = ch;
    if(rlen >= 3){
      if(rxbuf[2] == 0x00)                                   // ��ʾ����������Ч������
        error = 2;
      else if(rxbuf[2] == 0x01)
        error = 3;                                               // ��ʾ 11S  �ڴ������� 50mmHg( ����û��� 
      else if(rxbuf[2] == 0x02)
        error = 4;                                               // ��ʾ���������ֵ����
      else if(rxbuf[2] == 0x03)
        error = 5;                                               // ��ʾ����ѹ������ 295mmHg . ���볬ѹ����
      else if(rxbuf[2] == 0x04)
        error = 6;                                               // ��ʾ��Ԥ���ࣨ�������ƶ���˵���ȣ�
      rlen = 0;
      flag = 0;
      error = 0;
    }
  }
  else if(flag == 3){
    rxbuf[rlen++] = ch;
    if(rlen >= 7){
      if(rxbuf[2] == 0x01){                                  // ���ʲ�����
        error = 1;
        rlen = 0;
        flag = 0;
      }
      else{
        SBP = rxbuf[3];                                       // ����ѹ
        DBP = rxbuf[5];                                       // ����ѹ
        HR = rxbuf[6];                                       // ����
        rlen = 0;
        flag = 0;
      }
    }
  }
  else{
    rlen = 0;
    flag = 0;
  }
#endif
  return 0;
}

/*********************************************************************************************
* ���ƣ�blood_init()
* ���ܣ�Ѫѹ��������ʼ��
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void blood_init(void)
{
#if (BP_VERSION==1)
  uart3_init(4800);
#else
  uart3_init(115200);
#endif
  uart3_set_input(_on_recv);
}

void blood_start(void)
{
#if (BP_VERSION==1)
  char start_cmd[]= {0x02,0x30,0x31,0x3B,0x3B,0x44,0x37,0x03};      //��ʼ��������
  
  SBP = DBP = HR = 0;
  
  for (int i=0; i<8; i++)
  {
    uart3_putc(start_cmd[i]);
  }
#else
  char start_cmd[]= {0xFF,0xCD,0x03,0xA3,0xA0};      //��ʼ��������
  
  SBP = DBP = 0;
  
  for (int i=0; i<5; i++)
  {
    uart3_putc(start_cmd[i]);
  }
#endif
}
void blood_stop(void)
{
#if (BP_VERSION==1)
  char stop_cmd[] = {0x58};
  
  for (int i=0; i<1; i++)
  {
    uart3_putc(stop_cmd[i]);
  }
#else
  char stop_cmd[] = {0xFF,0xCD,0x03,0xA4,0xA1};
  
  for (int i=0; i<5; i++)
  {
    uart3_putc(stop_cmd[i]);
  }
#endif
}

void blood_result(void)
{
  char get_result_cmd[] = {0x02,0x31,0x38,0x3B,0x3B,0x44,0x46,0x03};//��ȡ�������ָ��
  
  
  for (int i=0; i<8; i++)
  {
    uart3_putc(get_result_cmd[i]);
  }
}




