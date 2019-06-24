/*********************************************************************************************
* �ļ���sensor.c
* ���ߣ�
* ˵����ͨ�ô��������ƽӿڳ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
#include "sensor.h"

#include "BatteryVoltage.h"
#include "lte_zhiyun.h"
#include "fml_gps/GPS.h"
#include "fml_light/apds9900.h"
#include "fml_imu/lis3dh.h"
#include "fml_VibratingMotor/VibratingMotor.h"
#include "SysCalendar.h"
#include "AlarmClockDLG.h"
#include "PhoneDLG.h"
//#include "AppFT.h"

unsigned char D0 = 0xff;                                        // �����ϱ�ʹ�ܣ�Ĭ��ֻ����A0�����ϱ�
unsigned char D1 = 0;                                           // Ĭ�Ϲرտ����ഫ����
unsigned int V0 = 30;                                           // �����ϱ�ʱ��������λ��,0�������ϱ�
unsigned char V6 = 0;
char A0[32] = {0};                                              // γ��&����
uint8_t A1 = 0;                                                 // �������״̬
uint8_t A2 = 0;                                                 // ����״̬
uint32_t A3 = 0;                                                // ����
float A7=0;                                                     // �����¶ȡ�ʪ�ȡ���ѹ

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
  char lat[16] = {0}, lng[16] = {0};                            //γ�ȣ�����
  if(gps_get(lat,lng))
    sprintf(A0,"%s&%s", lat, lng);
  else
    //sprintf(A0,"%s&%s", "NoSignal", "NoSignal");
    sprintf(A0,"30.52&114.31");
}

/*********************************************************************************************
* ���ƣ�updateA1
* ���ܣ�����A1��ֵ
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void updateA1(void)
{
  if(get_apds9900Lux() > 0xffff)
    A1 = 1;
  else
    A1 = 0;
}

/*********************************************************************************************
* ���ƣ�updateA2
* ���ܣ�����A2��ֵ
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void updateA2(void)
{
  static clock_time_t time = 0;
  
  if(time == 0)
  {
    if(get_lis3dh_tumbleStatus() == 1)
    {
      A2 = 1;
      time = clock_time();
    }
    else
      A2 = 0;
  }
  else if((clock_time() - time) > 3000)
    time = 0;
}

/*********************************************************************************************
* ���ƣ�updateA3
* ���ܣ�����A3��ֵ
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void updateA3(void)
{
  A3 = get_lis3dh_stepCount();
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
  gps_init();
  apds9900_init();
  lis3dh_init();
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
  
  if(cmd & 0x01)
    VibratingMotor_Set(1);
  else
    VibratingMotor_Set(0);
  
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
  updateA1();
  updateA2();
  updateA3();
  updateA7();
  
  zxbeeBegin();
  if (D0 & 0x01)
  {
    zxbeeAdd("A0", A0);
  }
  if (D0 & 0x02)
  {
    sprintf(buf, "%d", A1);
    zxbeeAdd("A1", buf);
  }
  if (D0 & 0x04)
  {
    sprintf(buf, "%d", A2);
    zxbeeAdd("A2", buf);
  }
  if (D0 & 0x08)
  {
    sprintf(buf, "%d", A3);
    zxbeeAdd("A3", buf);
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
  updateA1();
  updateA2();
  updateA3();
  updateA7();
  
  if (V0 != 0)
  {
    if (t % V0 == 0)
    {
      zxbeeBegin();
      if (D0 & 0x01)
      {
        zxbeeAdd("A0", A0);
      }
      if (D0 & 0x02)
      {
        sprintf(buf, "%d", A1);
        zxbeeAdd("A1", buf);
      }
      if (D0 & 0x04)
      {
        sprintf(buf, "%d", A2);
        zxbeeAdd("A2", buf);
      }
      if (D0 & 0x08)
      {
        sprintf(buf, "%d", A3);
        zxbeeAdd("A3", buf);
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
  static uint8_t LastA1 = 0,LastA2 = 0;
  char buf[32] = {0};
  
  run_lis3dh_arithmetic();
  
  updateA1();
  updateA2();
  
  if((A1 != LastA1))
  {
    LastA1 = A1;
    
    zxbeeBegin();
    if (D0 & 0x02)
    {
      sprintf(buf, "%d", A1);
      zxbeeAdd("A1", buf);
    }
    char *p = zxbeeEnd();
    if (p != NULL)
    {
      RFSendData(p);                                        //��������
    }
  }
  
  if((A2 != LastA2))
  {
    LastA2 = A2;
    
    zxbeeBegin();
    if (D0 & 0x04)
    {
      sprintf(buf, "%d", A2);
      zxbeeAdd("A2", buf);
    }
    char *p = zxbeeEnd();
    if (p != NULL)
    {
      RFSendData(p);                                        //��������
    }
  }
  
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
  if (memcmp(ptag, "CD1", 3) == 0)  				            //����⵽CD1ָ��
  {
    int v = atoi(pval);                                     //��ȡCD1����
    D1 &= ~v;                                               //����D1����
    sensor_control(D1);                                     //���µ�ŷ�ֱ�����״̬
  }
  if (memcmp(ptag, "OD1", 3) == 0)  				            //����⵽OD1ָ��
  {
    int v = atoi(pval);                                     //��ȡOD1����
    D1 |= v;                                                //����D1����
    sensor_control(D1);                                     //���µ�ŷ�ֱ�����״̬
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
      char V3[128] = {0};
      Phone_ZxbeeGet(V3);
      ret = sprintf(obuf, "V3=%s", V3);
    }
    else
    {
      Phone_ZxbeeSet(pval);
    }
  }
  if (memcmp(ptag, "V6", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "V6=%d", V6);
    }
    else
    {
      V6 = atoi(pval);
      //FTSetResponseFlag(V6);
    }
  }
  if (memcmp(ptag, "A0", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "A0=%s", A0);
    }
  }
  if (memcmp(ptag, "A1", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "A1=%d", A1);
    }
  }
  if (memcmp(ptag, "A2", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "A2=%d", A2);
    }
  }
  if (memcmp(ptag, "A3", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "A3=%d", A3);
    }
  }
  if (memcmp(ptag, "A4", 2) == 0)
  {
    if (pval[0] == '?')
    {
      //
    }
  }
  if (memcmp(ptag, "A5", 2) == 0)
  {
    if (pval[0] == '?')
    {
    }
  }
  if (memcmp(ptag, "A6", 2) == 0)
  {
    if (pval[0] == '?')
    {
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

