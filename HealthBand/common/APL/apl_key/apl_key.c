/*********************************************************************************************
* �ļ���fml_Key.c
* ���ߣ�fuyou
* ˵����������Ӧ���ļ�
* ���ܣ��ṩ������⹦��,�㲥�����¼�
* �޸ģ�
*********************************************************************************************/
/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include "apl_key.h"

process_event_t key_event;//��������
process_event_t keyUp_event;//�����ͷ�

PROCESS(KeyProcess, "KeyProcess");

/*********************************************************************************************
* ���ƣ�PROCESS_THREAD(KeyProcess, ev, data)
* ���ܣ������������
* ������KeyProcess, ev, data
* ���أ���
* �޸ģ���
*********************************************************************************************/
PROCESS_THREAD(KeyProcess, ev, data)
{
  PROCESS_BEGIN();
  
  static struct etimer Key_timer;
  static char KeyStatus=0,KeyFlag=1;
  
  key_GpioInit();
  key_event = process_alloc_event();
  keyUp_event = process_alloc_event();
  
  while (1)
  {
    KeyStatus = key_GetStatus();
    if(KeyStatus==0)
    {
      KeyFlag&=~0x7f;
      if(KeyFlag&0x80)
      {
        KeyFlag&=~0x80;
        process_post(PROCESS_BROADCAST, keyUp_event, &KeyStatus);
      }
    }
    else
    {
      KeyFlag|=0x80;
      if((KeyFlag&0x7f)%4==0)
      {
        KeyFlag&=~0x7f;
        process_post(PROCESS_BROADCAST, key_event, &KeyStatus);
      }
      KeyFlag++;
      if((KeyFlag&0x7f)>39) KeyFlag&=~0x7f;
    }
    etimer_set(&Key_timer,50);
    PROCESS_WAIT_EVENT_UNTIL(ev==PROCESS_EVENT_TIMER);  
  }
  
  PROCESS_END();
}

