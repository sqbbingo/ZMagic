/*********************************************************************************************
* �ļ���rfUart.h
* ���ߣ�fuyou 
* ˵����ԭ��������ߵĴ���ͨ������
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
#ifndef __rfUart_h_
#define __rfUart_h_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include "contiki.h"

/*********************************************************************************************
rfConfigMacro
*********************************************************************************************/
//���ڽ���BUF��С
#define RF1_BUF_SIZE 128
#define RF2_BUF_SIZE 128

//���ڵ�����Ϣ��
#define RF_UART_DEBUG   1

//xlabToolsʹ��
#define XLABTOOLS_ON    0

//xlabTools����ʱ���ڴ�ӡ������
#if XLABTOOLS_ON
#define UART_INFO_DEBUG   0
#else
#define UART_INFO_DEBUG   RF_UART_DEBUG
#endif

#if UART_INFO_DEBUG
#define RF_PRINT(...)    printf(__VA_ARGS__)
#else
#define RF_PRINT(...)
#endif

extern process_event_t uart_command_event;
PROCESS_NAME(rfUartProcess);

void rfUartSendByte(uint8_t index,char ch);
//�����߷����ַ���
void rfUartSendString(uint8_t index,char *buf, int len);
//�����߷���͸������
uint8_t rfUartSendData(uint8_t index, uint8_t* DataACK, char *dat);

uint8_t RF1_SendData(char* dat);
uint8_t RF2_SendData(char* dat);
void RFSendData(char* dat);

#endif
