#ifndef _lora_net_h_
#define _lora_net_h_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "rfUart.h"
#include "zxbee.h"
#include "rfUart.h"
#include "sensor_process.h"

#if 0

//#define LORA_CONFIG_NODEID  0xE852          //�ڵ��ַ:0x01-0xFE
#define LORA_CONFIG_NETID   50              //����ID:0-254
#define LORA_CONFIG_FP      435             //��Ƶ
#define LORA_CONFIG_PV      15              //���书��:1-20
#define LORA_CONFIG_SF      8               //��Ƶ����:6-12
#define LORA_CONFIG_CR      1               //������:1-4
#define LORA_CONFIG_PS      15              //ǰ���볤��
#define LORA_CONFIG_BW      5               //����:0-9
#define LORA_CONFIG_HOP     0               //��Ƶ����:0/1
//��Ƶ��
#define LORA_CONFIG_HOPTAB  "431,435,431,435,431,435,431,435,431,435"

#endif

PROCESS_NAME(lora_process);

unsigned short loraGet_fp(void);
unsigned char loraGet_pv(void);
unsigned char loraGet_sf(void);
unsigned char loraGet_cr(void);
unsigned char loraGet_ps(void);
unsigned char loraGet_bw(void);
unsigned char loraGet_hop(void);
unsigned char* loraGet_hopTab(void);
unsigned short loraGet_netID(void);
unsigned short loraGet_nodeID(void);

#endif