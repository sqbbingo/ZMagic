#ifndef _app_h_
#define _app_h_

#include "stm32f4xx.h"
#include "contiki.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "DIALOG.h"
#include "View.h"
#include "ff.h"
#include "SysCalendar.h"

#define   BSP_OK              (uint32_t)     0       // ����
#define   BSP_GTP             (uint32_t)0x0001       // ����������   
#define   BSP_SD              (uint32_t)0x0002       // SD������
#define   BSP_SPI_FLASH       (uint32_t)0x0004       // FLASH��������
#define   BSP_FILE_SYSTEM     (uint32_t)0x0008       // FLASH��������
#define   BSP_GUI             (uint32_t)0x0010       // emwin���д��� 
#define   BSP_WM8978          (uint32_t)0x0020       //  The WM8978 drive cannot work           
#define   BSP_ALLOC           (uint32_t)0x0040       //  �洢�� alloc ����
#define   BSP_RTC             (uint32_t)0x0080       //  RTC��������
#define   BSP_RF1             (uint32_t)0x0100       // �����豸1����
#define   BSP_RF2             (uint32_t)0x0200       // �����豸2����


#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                0
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO               1
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                2
#endif

#define  APP_CFG_TRACE_LEVEL             TRACE_LEVEL_DBG
#define  APP_CFG_TRACE                   printf

#define  BSP_CFG_TRACE_LEVEL             TRACE_LEVEL_DBG
#define  BSP_CFG_TRACE                   printf

#define  APP_TRACE_INFO(x)               ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_CFG_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)                ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_CFG_TRACE x) : (void)0)

#define  BSP_TRACE_INFO(x)               ((BSP_CFG_TRACE_LEVEL  >= TRACE_LEVEL_INFO) ? (void)(BSP_CFG_TRACE x) : (void)0)
#define  BSP_TRACE_DBG(x)                ((BSP_CFG_TRACE_LEVEL  >= TRACE_LEVEL_DBG)  ? (void)(BSP_CFG_TRACE x) : (void)0)


typedef struct
{
  uint8_t font;
  uint8_t language;
}SystemSet_t;

extern uint32_t bsp_result;
extern FATFS fs[2];											/* Work area (file system object) for logical drives */	
extern SystemSet_t System;

extern process_event_t AppOpenEvent;
extern process_event_t AppCloseEvent;
extern process_event_t AppInputEvent;

char ProcessIsRun(struct process *p);
void DISPLAY_RENCODE_TO_TFT(short xs,short ys,uint8_t *qrcode_data,short WinSize);

#endif