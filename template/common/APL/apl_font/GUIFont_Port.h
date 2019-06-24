#ifndef __GUIFONT_PORT_H
#define	__GUIFONT_PORT_H

#include "GUI.h"
#include "stm32f4xx.h"

//����XBF����洢��λ�ã�
//FLASH���ļ�ϵͳ�����Ƽ��� 	USE_FLASH_FONT             	0 
//SD���ļ�ϵͳ����							USE_SDCARD_FONT							1
//FLASH�ļ�ϵͳ����					  USE_FLASH_FILESYSTEM_FONT		2

#define XBF_FONT_SOURCE				      0

//���ٶ���죩�ֿ���FLASH�ķ��ļ�ϵͳ����ʹ��ǰ��Ҫ��FLASH�ض���ַ���������ļ�
//���ٶ��еȣ��ֿ�洢��SD���ļ�ϵͳ���򣬵��ԱȽϷ��㣬ֱ��ʹ�ö������ӵ��Կ��������ļ�����
//���ٶ��������ֿ�洢��FLASH�ļ�ϵͳ����ʹ��ǰ��Ҫ��FLASH�ļ�ϵͳ�洢�����ļ�������Ϊ��ʾ�����Ƽ�ʹ��

#define USE_FLASH_FONT							0	
#define USE_SDCARD_FONT							1
#define USE_FLASH_FILESYSTEM_FONT   2


/*֧�ֵ��ֿ�����*/
extern GUI_FONT     FONT_XINSONGTI_12;
extern GUI_FONT     FONT_XINSONGTI_16;
extern GUI_FONT     FONT_XINSONGTI_24;


short Creat_XBF_Font(void) ;
short GBK2UTF8(char *src, char *str);

#endif /* __GUIFONT_PORT_H */
