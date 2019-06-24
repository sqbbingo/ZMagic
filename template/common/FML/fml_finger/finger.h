#ifndef __FINGER_H_
#define __FINGER_H_

#include "stm32f4xx.h"
#include "string.h"
#include "usart.h"

#define MAXRECBYTE      128

#define HEADER          0xEF, 0x01
#define ADDR            0xFF, 0xFF, 0xFF, 0xFF

#define LED_Breathe     0x01                                    // LED����-����
#define LED_Glint       0x02                                    // LED����-��˸
#define LED_LongON      0x03                                    // LED����-����
#define LED_LongOFF     0x04                                    // LED����-����
#define LED_RED         0x04                                    // LED��ɫ-��ɫ
#define LED_Green       0x02                                    // LED��ɫ-��ɫ
#define LED_Blue        0x01                                    // LED��ɫ-��ɫ
#define LED_RG          0x06                                    // LED��ɫ-���̵�
#define LED_RB          0x05                                    // LED��ɫ-������
#define LED_GB          0x03                                    // LED��ɫ-������
#define LED_RGB         0x07                                    // LED��ɫ-��������
#define LED_OFF         0x00                                    // LED��ɫ-ȫ��

#define false           0
#define true            1

typedef struct{
  unsigned char validTempNum;
  unsigned char errorNum;
  unsigned int pageNum;               // ָ����֤ҳ��
  unsigned int verifyScore;          // �÷�
  int fingerMode;
}finger_TypeDef;

typedef struct{
  unsigned char usartRecBuff[MAXRECBYTE];
  unsigned int usartRecLen;
}usart_TypeDef;

typedef struct{
  unsigned char led : 1;                                                  // 0-LED ������1-LED ��ȡͼ��ɹ�����     
  unsigned char pretreatment : 1;                                         // 0-��ͼͼ��Ԥ����1-��ͼͼ��Ԥ����
  unsigned char back : 1;                                                 // ¼��������Ƿ�Ҫ��ģ���ڹؼ����践�ص�ǰ״̬��0-Ҫ�󷵻أ�1-��Ҫ�󷵻�
  unsigned char cover : 1;                                                // �µ�ָ��ģ���Ƿ񸲸ǣ�1-���ǣ�0-������
  unsigned char detection : 1;                                            // ����µ�ָ��ģ���Ƿ��Ѿ����ڣ� 1-��⣬0-�����
  unsigned char leave : 1;                                                // ¼�������,�Ƿ�Ҫ����ָ�뿪�Ž�����һ��ָ��ͼ��ɼ�:0-Ҫ���뿪,1-��Ҫ���뿪
}param_TypeDef;

typedef struct{
  unsigned char enrollNum : 6;
  unsigned char enrollFlag : 1;
  unsigned char identifyFlag : 1;
}handle_TypeDef;

void finger_init(void);
int finger_SetSendByteCall(void (*func)(char dat));
int finger_usartInput(char ch);//���ڽ��ջص�
void finger_uartAnalysis(void);//��ѯ����
void finger_cancel(void);//ȡ����ǰ����
void finger_autoEnroll(unsigned int id, unsigned char inputNum);//ע��ָ�ƣ�id���ɼ�����
void finger_autoIdentify(unsigned int id, unsigned char level);//����һ����֤
void finger_resetParam(void);
unsigned int finger_getParam(void);
void finger_setParam(unsigned char cmd);
void finger_deletChar(unsigned int id, unsigned int delNum);//ɾ��ָ�ƣ���ʼid��ɾ������
int finger_getEnrollInfo(void);//��ȡע����
int finger_getIdentifyInfo(void);//��ȡ��֤���

#endif
