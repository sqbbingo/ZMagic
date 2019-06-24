/*********************************************************************************************
* �ļ���sensor.c
* ���ߣ�
* ˵����ͨ�ô��������ƽӿڳ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
#include "sensor.h"
#include "Relay.h"
#include "BatteryVoltage.h"
#include "lte_zhiyun.h"
#include "fml_stepmotor/fml_stepmotor.h"
#include "SellDLG.h"

uint8_t D0 = 0xff;                                              // �����ϱ�ʹ�ܣ�Ĭ��ֻ����A0�����ϱ�
uint8_t D1 = 0;                                                 // Ĭ�Ϲرտ����ഫ����
uint16_t V0 = 30;                                               // �����ϱ�ʱ��������λ��,0�������ϱ�
double A0 = 0;                                                  // �����ܽ��
float A7=0;                                                     // ���ص�ѹ

/*********************************************************************************************
* ���ƣ�sensor_type()
* ���ܣ�
* ������
* ���أ����ش��������ͣ�3�ֽ��ַ�����ʾ
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
char *sensor_type(void)
{
  return SENSOR_TYPE;                                           //���ش���������
}

/*********************************************************************************************
* ���ƣ�updateA0
* ���ܣ�����A0��ֵ
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void updateA0(void)
{
  A0 = Sell_MoneyTotalGet();
}

/*********************************************************************************************
* ���ƣ�updateA7
* ���ܣ�����A7��ֵ
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void updateA7(void)
{
  A7 = BatteryVotage_Get();                                   // ����A7 ��ص�ѹ
}


/*********************************************************************************************
* ���ƣ�sensor_init()
* ���ܣ�
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void sensor_init(void)
{
  relay_init();                                                 //��ʼ�������ഫ�������̵�����
}


/*********************************************************************************************
* ���ƣ�sensor_control()
* ���ܣ�����������
* ������cmd - ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void sensor_control(unsigned char cmd)
{
  if((cmd & (3<<0)) == (3<<0))
  {
    Stepmotor_Run(1,1,1000);
  }
  else if(cmd & (1<<0))
  {
    Stepmotor_Run(1,0,1000);
  }
  
  if((cmd & (3<<2)) == (3<<2))
  {
    Stepmotor_Run(2,1,1000);
  }
  else if(cmd & (1<<2))
  {
    Stepmotor_Run(2,0,1000);
  }
  
  if((cmd & (3<<4)) == (3<<4))
  {
    Stepmotor_Run(3,1,1000);
  }
  else if(cmd & (1<<4))
  {
    Stepmotor_Run(3,0,1000);
  }
  
  D1 &= ~0x3f;
    
  
  if(cmd & 0x40)
  {
    relay_on(1);
  }
  else
  {
    relay_off(1);
  }
  if(cmd & 0x80)
  {
    relay_on(2);
  }
  else
  {
    relay_off(2);
  }
}

/*********************************************************************************************
* ���ƣ�sensorLinkOn
* ���ܣ�
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void sensorLinkOn()
{
  char buf[64]={0};
  updateA0();
  updateA7();
  
  zxbeeBegin();
  if (D0 & 0x01)
  {
    sprintf(buf, "%.2f", A0);
    zxbeeAdd("A0", buf);
  }
  if (D0 & 0x80)
  {
    sprintf(buf, "%.1f", A7);
    zxbeeAdd("A7", buf);
  }
  char *p = zxbeeEnd();
  if (p != NULL)
  {
    RFSendData(p);                             //��������
  }
}

/*********************************************************************************************
* ���ƣ�sensor_poll()
* ���ܣ���ѯ���������������ϱ�����������
* ������t: ���ô���
* ���أ�
* �޸ģ�
* ע�ͣ��˺���ÿ���ӵ���1�Σ�tΪ���ô���
*********************************************************************************************/
void sensor_poll(unsigned int t)
{
  char buf[64]={0};
  updateA0();
  updateA7();
  
  if (V0 != 0)
  {
    if (t % V0 == 0)
    {
      zxbeeBegin();
      if (D0 & 0x01)
      {
        sprintf(buf, "%.2f", A0);
        zxbeeAdd("A0", buf);
      }
      if (D0 & 0x80)
      {
        sprintf(buf, "%.1f", A7);
        zxbeeAdd("A7", buf);
      }
      char *p = zxbeeEnd();
      if (p != NULL)
      {
        RFSendData(p);                             //��������
      }
    }
  }
}

/*********************************************************************************************
* ���ƣ�sensor_check()
* ���ܣ������Լ�麯�������趨��ѯʱ��
* ��������
* ���أ�������ѯ��ʱ�䣬��λms,��Χ:1-65535
* �޸ģ�
* ע�ͣ��˺���������Ҫ����������Ӧ�Ĵ�����
*********************************************************************************************/
unsigned short sensor_check()
{
  return 100;                                                 //����ֵ�����´ε���ʱ�䣬�˴�Ϊ100ms
}

/*********************************************************************************************
* ���ƣ�z_process_command_call()
* ���ܣ������ϲ�Ӧ�÷�������ָ��
* ������ptag: ָ���ʶ D0��D1�� A0 ...
*       pval: ָ��ֵ�� ��������ʾ��ȡ��
*       obuf: ָ�������ŵ�ַ
* ���أ�>0ָ������������ݳ��ȣ�0��û�з������ݣ�<0����֧��ָ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int z_process_command_call(char* ptag, char* pval, char* obuf)
{
  int ret = -1;
  
  if (memcmp(ptag, "D0", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "D0=%d", D0);
    }
  }
  if (memcmp(ptag, "CD0", 3) == 0)
  {
    int v = atoi(pval);
    if (v > 0)
    {
      D0 &= ~v;
    }
  }
  if (memcmp(ptag, "OD0", 3) == 0)
  {
    int v = atoi(pval);
    if (v > 0)
    {
      D0 |= v;
    }
  }
  if (memcmp(ptag, "D1", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "D1=%d", D1);
    }
  }
  if (memcmp(ptag, "CD1", 3) == 0)  				                    //����⵽CD1ָ��
  {
    int v = atoi(pval);                                         //��ȡCD1����
    D1 &= ~v;                                                   //����D1����
    sensor_control(D1);                                         //���µ�ŷ�ֱ�����״̬
  }
  if (memcmp(ptag, "OD1", 3) == 0)  				                    //����⵽OD1ָ��
  {
    int v = atoi(pval);                                         //��ȡOD1����
    D1 |= v;                                                    //����D1����
    sensor_control(D1);                                         //���µ�ŷ�ֱ�����״̬
  }
  if (memcmp(ptag, "V0", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "V0=%d", V0);
    }
    else
    {
      V0 = atoi(pval);
    }
  }
  if (memcmp(ptag, "V1", 2) == 0)
  {
    if (pval[0] == '?')
    {
      char V1[128] = {0};
      Calender_ZxbeeGet(V1);
      ret = sprintf(obuf, "V1=%s", V1);
    }
    else
    {
      Calender_ZxbeeSet(pval);
    }
  }
  if (memcmp(ptag, "V2", 2) == 0)
  {
    if (pval[0] == '?')
    {
      char V2[128] = {0};
      Alarm_ZxbeeGet(V2);
      ret = sprintf(obuf, "V2=%s", V2);
    }
    else
    {
      Alarm_ZxbeeSet(pval);
    }
  }
  if (memcmp(ptag, "V3", 2) == 0)
  {
    if (pval[0] == '?')
    {
      char V3[32] = {0};
      SellNum_ZxbeeGet(V3);
      ret = sprintf(obuf, "V3=%s", V3);
    }
    else
    {
      SellNum_ZxbeeSet(pval);
    }
  }
  if (memcmp(ptag, "V4", 2) == 0)
  {
    if (pval[0] == '?')
    {
      char V4[32] = {0};
      SellPrice_ZxbeeGet(V4);
      ret = sprintf(obuf, "V4=%s", V4);
    }
    else
    {
      SellPrice_ZxbeeSet(pval);
    }
  }
  if (memcmp(ptag, "V5", 2) == 0)
  {
    if (pval[0] == '?')
    {
      char V5[32] = {0};
      SellPayment_ZxbeeGet(V5);
      ret = sprintf(obuf, "V5=%s", V5);
    }
    else
    {
      SellPayment_ZxbeeSet(pval);
    }
  }
  if (memcmp(ptag, "A0", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "A0=%.2f", A0);
    }
  }
  if (memcmp(ptag, "A7", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "A7=%.1f", A7);
    }
  }
  return ret;
}

