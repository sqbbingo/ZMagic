/*********************************************************************************************
* �ļ���sensor.h
* ���ߣ�
* ˵����ͨ�ô��������ƽӿڳ���ͷ�ļ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
#ifndef _sensor_h_
#define _sensor_h_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "zxbee.h"

#define SENSOR_TYPE  "710"                                      // ����������

char *sensor_type(void);
void sensor_init(void);
void sensor_poll(unsigned int t);
unsigned short sensor_check(void);
void sensorLinkOn(void);

#endif