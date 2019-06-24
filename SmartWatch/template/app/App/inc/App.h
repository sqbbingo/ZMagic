#ifndef _app_h_
#define _app_h_

#include "stm32f4xx.h"
#include "contiki.h"
#include "stdio.h"
#include "string.h"
#include "DIALOG.h"
#include "ff.h"
#include "SysCalendar.h"
#include "sensor_process.h"

#include "View.h"

#define   BSP_OK              (uint32_t)     0       // ����
#define   BSP_GTP             (uint32_t)0x0004       // ����������   
#define   BSP_SD              (uint32_t)0x0010       // SD������
#define   BSP_SPI_FLASH       (uint32_t)0x0020       // FLASH��������
#define   BSP_FILE_SYSTEM     (uint32_t)0x0040       // FLASH��������
#define   BSP_GUI             (uint32_t)0x0100       // emwin���д��� 
#define   BSP_WM8978          (uint32_t)0x0200       //  The WM8978 drive cannot work           
#define   BSP_ALLOC           (uint32_t)0x0400       //  �洢�� alloc ����
#define   BSP_RTC             (uint32_t)0x0800       //  RTC��������
#define   BSP_NETWORK         (uint32_t)0x1000       // ��̫������
#define   BSP_CAMERA          (uint32_t)0x2000       // ����ͷ����


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


#define QrEncode_WindowSize 145

typedef struct
{
  uint8_t font;
  uint8_t language;
}SystemSet_t;

typedef struct
{
  uint32_t flag;
  uint32_t* ram;
}RAM_t;

extern uint32_t bsp_result;
extern FATFS fs[2];											/* Work area (file system object) for logical drives */	
extern SystemSet_t System;

extern process_event_t AppOpenEvent;
extern process_event_t AppCloseEvent;
extern process_event_t AppInputEvent;

char ProcessIsRun(struct process *p);
short UTF8_GetWide(char* str,short PixelSize);
void DISPLAY_RENCODE_TO_TFT(short xs,short ys,uint8_t *qrcode_data);

#endif