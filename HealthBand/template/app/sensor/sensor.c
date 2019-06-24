/*********************************************************************************************
* 文件：sensor.c
* 作者：
* 说明：通用传感器控制接口程序
* 修改：
* 注释：
*********************************************************************************************/
#include "sensor.h"
#include "Relay.h"
#include "BatteryVoltage.h"
#include "lte_zhiyun.h"
#include "fml_imu/lis3dh.h"
#include "fml_InfraredTemp/mlx90615.h"	
#include "apl_HeartRate/apl_HeartRate.h"
#include "bloodpressure.h"

unsigned char D0 = 0xff;                                        // 主动上报使能
unsigned char D1 = 0;                                           // 默认关闭控制类传感器
unsigned int V0 = 30;                                           // 主动上报时间间隔，单位秒,0不主动上报
char V4[20] = {0};
char V5[20] = {0};
char V6[20] = {0};
float A0 = 0;                                                   // 体温
int32_t A1 = 0;                                                 // 血氧
int32_t A2 = 0;                                                 // 心率
int16_t A3 = 0;                                                 // 血压收缩压
int16_t A4 = 0;                                                 // 血压舒张压
int A5 = 0;                                                     // 跌倒状态
int A6 = 0;                                                     // 步数
float A7=0;                                                     // 电压

/*********************************************************************************************
* 名称：sensor_type()
* 功能：
* 参数：
* 返回：返回传感器类型，3字节字符串表示
* 修改：
* 注释：
*********************************************************************************************/
char *sensor_type(void)
{
  return SENSOR_TYPE;                                           //返回传感器类型
}

/*********************************************************************************************
* 名称：updateA0
* 功能：更新A0的值
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void updateA0(void)
{
  mlx90615_GetTmep(&A0);
}

/*********************************************************************************************
* 名称：updateA1
* 功能：更新A1的值
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void updateA1(void)
{
  HeartRate_Get(&A1,&A2);
}

/*********************************************************************************************
* 名称：updateA2
* 功能：更新A2的值
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void updateA2(void)
{
  HeartRate_Get(&A1,&A2);
}

/*********************************************************************************************
* 名称：updateA3
* 功能：更新A3的值
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void updateA3(void)
{
  A3 = SBP_Get();
}

/*********************************************************************************************
* 名称：updateA4
* 功能：更新A4的值
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void updateA4(void)
{
  A4 = DBP_Get();
}

/*********************************************************************************************
* 名称：updateA5
* 功能：更新A5的值
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void updateA5(void)
{
  static clock_time_t time = 0;
  
  if(time == 0)
  {
    if(get_lis3dh_tumbleStatus() == 1)
    {
      A5 = 1;
      time = clock_time();
    }
    else
      A5 = 0;
  }
  else if((clock_time() - time) > 3000)
    time = 0;
}

/*********************************************************************************************
* 名称：updateA6
* 功能：更新A6的值
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void updateA6(void)
{
  A6 = get_lis3dh_stepCount();
}

/*********************************************************************************************
* 名称：updateA7
* 功能：更新A7的值
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void updateA7(void)
{
  A7 = BatteryVotage_Get();                                   // 更新A7 电池电压
}

/*********************************************************************************************
* 名称：sensor_init()
* 功能：
* 参数：
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
void sensor_init(void)
{
  relay_init();                                                 //初始化控制类传感器（继电器）
  mlx90615_init();
  lis3dh_init();
  blood_init();
}


/*********************************************************************************************
* 名称：sensor_control()
* 功能：传感器控制
* 参数：cmd - 控制命令
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void sensor_control(unsigned char cmd)
{
  
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
* 名称：sensorLinkOn
* 功能：
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void sensorLinkOn()
{
  char buf[64]={0};
  updateA0();
  updateA1();
  updateA2();
  updateA3();
  updateA4();
  updateA5();
  updateA6();
  updateA7();
  
  zxbeeBegin();
  if (D0 & 0x01)
  {
    sprintf(buf, "%.1f", A0);
    zxbeeAdd("A0", buf);
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
  if (D0 & 0x10)
  {
    sprintf(buf, "%d", A4);
    zxbeeAdd("A4", buf);
  }
  if (D0 & 0x20)
  {
    sprintf(buf, "%d", A5);
    zxbeeAdd("A5", buf);
  }
  if (D0 & 0x40)
  {
    sprintf(buf, "%d", A6);
    zxbeeAdd("A6", buf);
  }
  if (D0 & 0x80)
  {
    sprintf(buf, "%.1f", A7);
    zxbeeAdd("A7", buf);
  }
  char *p = zxbeeEnd();
  if (p != NULL)
  {
    RFSendData(p);                                              //发送数据
  }
}

/*********************************************************************************************
* 名称：sensor_poll()
* 功能：轮询传感器，并主动上报传感器数据
* 参数：t: 调用次数
* 返回：
* 修改：
* 注释：此函数每秒钟调用1次，t为调用次数
*********************************************************************************************/
void sensor_poll(unsigned int t)
{
  char buf[64]={0};
  updateA0();
  updateA1();
  updateA2();
  updateA3();
  updateA4();
  updateA5();
  updateA6();
  updateA7();
  
  if (V0 != 0)
  {
    if (t % V0 == 0)
    {
      zxbeeBegin();
      if (D0 & 0x01)
      {
        sprintf(buf, "%.1f", A0);
        zxbeeAdd("A0", buf);
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
      if (D0 & 0x10)
      {
        sprintf(buf, "%d", A4);
        zxbeeAdd("A4", buf);
      }
      if (D0 & 0x20)
      {
        sprintf(buf, "%d", A5);
        zxbeeAdd("A5", buf);
      }
      if (D0 & 0x40)
      {
        sprintf(buf, "%d", A6);
        zxbeeAdd("A6", buf);
      }
      if (D0 & 0x80)
      {
        sprintf(buf, "%.1f", A7);
        zxbeeAdd("A7", buf);
      }
      char *p = zxbeeEnd();
      if (p != NULL)
      {
        RFSendData(p);                                          //发送数据
      }
    }
  }
}

/*********************************************************************************************
* 名称：sensor_check()
* 功能：周期性检查函数，可设定轮询时间
* 参数：无
* 返回：设置轮询的时间，单位ms,范围:1-65535
* 修改：
* 注释：此函数用于需要快速做出相应的传感器
*********************************************************************************************/
unsigned short sensor_check()
{
  static uint8_t LastA5 = 0;
  char buf[32] = {0};
  
  run_lis3dh_arithmetic();
  
  updateA5();
  if((A5 != LastA5))
  {
    LastA5 = A5;
    
    zxbeeBegin();
    if (D0 & 0x20)
    {
      sprintf(buf, "%d", A5);
      zxbeeAdd("A5", buf);
    }
    char *p = zxbeeEnd();
    if (p != NULL)
    {
      RFSendData(p);                                            //发送数据
    }
  }
  return 100;                                                   //返回值决定下次调用时间，此处为100ms
}

/*********************************************************************************************
* 名称：z_process_command_call()
* 功能：处理上层应用发过来的指令
* 参数：ptag: 指令标识 D0，D1， A0 ...
*       pval: 指令值， “？”表示读取，
*       obuf: 指令处理结果存放地址
* 返回：>0指令处理结果返回数据长度，0：没有返回数据，<0：不支持指令。
* 修改：
* 注释：
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
  if (memcmp(ptag, "CD1", 3) == 0)  				            //若检测到CD1指令
  {
    int v = atoi(pval);                                     //获取CD1数据
    D1 &= ~v;                                               //更新D1数据
    sensor_control(D1);                                     //更新电磁阀直流电机状态
  }
  if (memcmp(ptag, "OD1", 3) == 0)  				            //若检测到OD1指令
  {
    int v = atoi(pval);                                     //获取OD1数据
    D1 |= v;                                                //更新D1数据
    sensor_control(D1);                                     //更新电磁阀直流电机状态
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
      //
    }
    else
    {
      //
    }
  }
  if (memcmp(ptag, "V2", 2) == 0)
  {
    if (pval[0] == '?')
    {
      //
    }
    else
    {
      //
    }
  }
  if (memcmp(ptag, "V3", 2) == 0)
  {
    if (pval[0] == '?')
    {
      //
    }
    else
    {
      //
    }
  }
  if (memcmp(ptag, "A0", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "A0=%.1f", A0);
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
      ret = sprintf(obuf, "A4=%d", A4);
    }
  }
  if (memcmp(ptag, "A5", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "A5=%d", A5);
    }
  }
  if (memcmp(ptag, "A6", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "A6=%d", A6);
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

