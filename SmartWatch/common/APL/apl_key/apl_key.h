/*********************************************************************************************
* �ļ���fml_key.h
* ���ߣ�fuyou
* ˵����������Ӧ���ļ�
* ���ܣ�
* �޸ģ�
*********************************************************************************************/
#ifndef __APL_KEY_H__
#define __APL_KEY_H__

#include "fml_key.h"
#include "contiki.h"

PROCESS_NAME(key);
extern process_event_t key_event;
extern process_event_t keyUp_event;

#endif