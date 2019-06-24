#include "BatteryVoltage.h"


#define ARR_LEN 10 /*���鳤��*/

double BatteryVoltage = 0;
double CpuTemperature = 0;

/*********************************************************************************************
* ����: BatteryVotage_Get
* ����: ��ȡ��ص�ѹ
* ����: ��
* ����: ��ص�ѹֵ����λ���أ�V��
* �޸�:
* ע��:
*********************************************************************************************/
double BatteryVotage_Get()
{
  return BatteryVoltage;
}

double CpuTemperature_Get()
{
  return CpuTemperature;
}

#define elemType uint32_t /*Ԫ������*/
 
/* ð������ */
/* 1. �ӵ�ǰԪ����������αȽ�ÿһ������Ԫ�أ��������򽻻� */
/* 2. ������Ԫ�ؾ��ظ����ϲ��裬ֱ�����һ��Ԫ�� */
/* elemType arr[]: ����Ŀ������; int len: Ԫ�ظ��� */
void bubbleSort (elemType arr[], int len) 
{
  elemType temp;
  int i, j;
  for (i=0; i<len-1; i++) /* ��ѭ��Ϊ����������len��������len-1�� */
  {
    for (j=0; j<len-1-i; j++)  /* ��ѭ��Ϊÿ�˱ȽϵĴ�������i�˱Ƚ�len-i�� */
    {
      if (arr[j] > arr[j+1])  /* ����Ԫ�رȽϣ��������򽻻�������Ϊ������ң�����֮�� */
      {
        temp = arr[j];
        arr[j] = arr[j+1];
        arr[j+1] = temp;
      }
    }
  }
}

/*********************************************************************************************
����������
*********************************************************************************************/
PROCESS(PowerProcess, "PowerProcess");

/*********************************************************************************************
* ����: PROCESS_THREAD()
* ����: ���ش������������
* ����: onboard_sensors, ev, data
* ����: ��
* �޸�:
* ע��:
*********************************************************************************************/
PROCESS_THREAD(PowerProcess, ev, data)
{
  static struct etimer VBATT_timer;
  static short tick=0;
  static uint32_t AdcValue=0;
  static uint32_t AdcDataBuffer[ARR_LEN]={0};
  
  PROCESS_BEGIN();
  
  //BspADC_Init();
  
  CpuTemperature = Get_CpuTemprateX(10);
  AdcValue = AdcGetX(1,10);
  BatteryVoltage = (double)AdcValue*(3.3/4095.0)*13.6/3.6;
  AdcValue=0;
  
  while(1)
  {
    etimer_set(&VBATT_timer,50);
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);  
    
    CpuTemperature = Get_CpuTemprateX(10);
    AdcDataBuffer[tick] = AdcGetX(1,10);
    tick++;
    if(tick>=ARR_LEN)
    {
      bubbleSort(AdcDataBuffer,ARR_LEN);
      for(short i=1;i<ARR_LEN-1;i++)
      {
        AdcValue+=AdcDataBuffer[i];
      }
      AdcValue=(uint16_t)(AdcValue/(ARR_LEN-2));
      BatteryVoltage = (double)AdcValue*(3.3/4095.0)*13.6/3.6;
      tick=0;
      AdcValue=0;
    }
  }
  
  PROCESS_END();
}