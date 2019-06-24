#include "finger.h"

usart_TypeDef usartFinger;
finger_TypeDef fingerStruct;
param_TypeDef paramStruct;
handle_TypeDef handleStruct;

void (*finger_SendByte)(char byte)=NULL;

int finger_SetSendByteCall(void (*func)(char dat))
{
  finger_SendByte = func;
  return 0;
}

void finger_init(void)
{
  // ToDo ���ڳ�ʼ�� baud:57600
	finger_resetParam();
}

void finger_usartSend(unsigned char *buf, unsigned char len)
{
  for(unsigned char i=0; i<len; i++)
    finger_SendByte(buf[i]);
}

// ���ص�ǰָ��ģ��ģʽ
int finger_getCurrMode(void)
{
  return fingerStruct.fingerMode;
}

// ���ص�ǰ��Чģ������
unsigned char finger_getValidTempNum(void)
{
  return fingerStruct.validTempNum;
}

// ���ص�ǰ������
unsigned char finger_getErrorNum(void)
{
  return fingerStruct.errorNum;
}

// ��մ��ڽ���BUFF
void finger_delUsartBuff(void)
{
  memset(usartFinger.usartRecBuff, 0, usartFinger.usartRecLen);
  usartFinger.usartRecLen = 0;
}

// У��ͼ���
unsigned int finger_checkOut(unsigned char *buf, unsigned char start, unsigned int len)
{
  unsigned int check = 0;
  for(unsigned char i=start; i<len; i++)
  {
    check += buf[i];
  }
  return check;
}

// ��ʼ������
void finger_resetParam(void)
{
  paramStruct.led = true;                                    // Ĭ�ϲɼ��ɹ���LEDϨ��
  paramStruct.pretreatment = false;                          // Ĭ�ϲɼ��ɹ���Ԥ����
  paramStruct.back = false;                                  // Ĭ���ڴ���ؼ����践�ص�ǰ״̬
  paramStruct.cover = true;                                  // Ĭ�ϸ����µ�ָ��ģ��
  paramStruct.detection = false;                             // Ĭ�ϲ����ָ��ģ���Ƿ����
  paramStruct.leave = false;                                 // Ĭ��Ҫ����ָ�뿪
}

// ��ȡ����
unsigned int finger_getParam(void)
{
  unsigned int param = 0;
  if(paramStruct.led)
    param |= 0x01;
  else
    param &= ~0x01;
  if(paramStruct.pretreatment)
    param |= 0x02;
  else
    param &= ~0x02;
  if(paramStruct.back)
    param |= 0x04;
  else
    param &= ~0x04;
  if(paramStruct.cover)
    param |= 0x08;
  else
    param &= ~0x08;
  if(paramStruct.detection)
    param |= 0x10;
  else
    param &= ~0x10;
  if(paramStruct.leave)
    param |= 0x20;
  else
    param &= ~0x20;
  return param;
}

void finger_setParam(unsigned char cmd)
{
    paramStruct.led = cmd & 0x01;
    paramStruct.pretreatment = cmd & 0x02;
    paramStruct.back = cmd & 0x04;
    paramStruct.cover = cmd & 0x08;
    paramStruct.detection = cmd & 0x10;
    paramStruct.leave = cmd & 0x20;
}

// ��ȡͼ��
void finger_getImage(void)
{
  unsigned char getImageOrder[12] = {HEADER, ADDR, 0x01, 0x00, 0x03, 0x01, 0x00, 0x05};
  //unsigned char *buf = getImageOrder;
  finger_usartSend(getImageOrder, 12);
}

// ע���û�ȡͼ��
void finger_getRnrollImage(void)
{
  unsigned char getRnrollImageOrder[12] = {HEADER, ADDR, 0x01, 0x00, 0x03, 0x29, 0x00, 0x2D};
  finger_usartSend(getRnrollImageOrder, 12);
}

// ��������
// buffID-������ID��
void finger_getChar(unsigned char buffID)
{
  unsigned int check = 0;
  unsigned char getCharOrder[13] = {HEADER, ADDR, 0x01, 0x00, 0x04, 0x02, buffID, 0x00, 0x00};
  check = finger_checkOut(getCharOrder, 6, 13);
  getCharOrder[11] = check / 256;
  getCharOrder[12] = check % 256;
  finger_usartSend(getCharOrder, 13);
}

// �ϳ�ģ��
void finger_regMB(void)
{
  unsigned char regMBOrder[12] = {HEADER, ADDR, 0x01, 0x00, 0x03, 0x05, 0x00, 0x09};
  //unsigned char *buf = regMBOrder;
  finger_usartSend(regMBOrder, 12);
}

// ����ģ��
// buffID-ģ���/pageID-ָ�ƿ�λ�ú�
void finger_storMB(char buffID, unsigned int pageID)
{
  unsigned int check = 0;
  unsigned char storMBOrder[15] = {HEADER, ADDR, 0x01, 0x00, 0x06, 0x06, buffID, pageID, 0x00, 0x00};
  //unsigned char *buf = storMBOrder;
  check = finger_checkOut(storMBOrder, 6, 15);
  storMBOrder[13] = (unsigned char)check / 256;
  storMBOrder[14] = (unsigned char)check % 256;
  finger_usartSend(storMBOrder, 15);
}

// ����ģ��
// buffID-ģ���/pageID-��ʼλ��/searchNum-��������
void finger_searchMB(unsigned char buffID, unsigned int startPage, unsigned int searchNum)
{
  unsigned int check = 0;
  unsigned char searchMBOrder[17] = {HEADER, ADDR, 0x01, 0x00, 0x08, 0x04, buffID, startPage/256, startPage%256, searchNum/256, searchNum%256, 0x00, 0x00};
  //unsigned char *buf = searchMBOrder;
  check = finger_checkOut(searchMBOrder, 6, 17);
  searchMBOrder[15] = (unsigned char)check / 256;
  searchMBOrder[16] = (unsigned char)check % 256;
  finger_usartSend(searchMBOrder, 17);
}

// ��ȡ������
// pageNum-ҳ��(0-3)
void finger_readIndexTable(unsigned char pageNum)
{
  unsigned int check = 0;
  unsigned char readIndexTableOrder[13] = {HEADER, ADDR, 0x01, 0x00, 0x04, 0x1F, pageNum, 0x00, 0x00};
  check = finger_checkOut(readIndexTableOrder, 6, 13);
  readIndexTableOrder[11] = (unsigned char)check / 256;
  readIndexTableOrder[12] = (unsigned char)check % 256;
  finger_usartSend(readIndexTableOrder, 13);
  fingerStruct.fingerMode = 4;
}

// ��ɫ�ƿ���
// function-ִ�й���/color-��ɫ/num-ִ�д���
void finger_controlBLN(unsigned char function, unsigned char color, unsigned char num)
{
  unsigned int check = 0;
  unsigned char controlBLNOrder[15] = {HEADER, ADDR, 0x01, 0x00, 0x06, 0x3C, function, color, num, 0x00, 0x00};
  //unsigned char *buf = controlBLNOrder;
  check = finger_checkOut(controlBLNOrder, 6, 15);
  controlBLNOrder[13] = check / 256;
  controlBLNOrder[14] = check % 256;
  finger_usartSend(controlBLNOrder, 15);
}

// ��ȡ��Чģ������
void finger_validTempleteNum(void)
{
  unsigned char validTempleteNumOrder[12] = {HEADER, ADDR, 0x01, 0x00, 0x03, 0x1D, 0x00, 0x21};
  finger_usartSend(validTempleteNumOrder, 12);
  fingerStruct.fingerMode = 3;
}

// ɾ��ָ��ģ��
// id-��ʼɾ����id��/num-ɾ���ĸ���
void finger_deletChar(unsigned int id, unsigned int delNum)
{
  unsigned int check = 0;
  unsigned char deletCharOrder[16] = {HEADER, ADDR, 0x01, 0x00, 0x07, 0x0C, id/256, id%256, delNum/256, delNum%256, 0x00, 0x00};
  check = finger_checkOut(deletCharOrder, 6, 16);
  deletCharOrder[14] = check / 256;
  deletCharOrder[15] = check % 256;
  finger_usartSend(deletCharOrder, 16);
}

// ���ָ�ƿ�
void finger_empty(void)
{
  unsigned char empty[12] = {HEADER, ADDR, 0x01, 0x00, 0x03, 0x0d, 0x00, 0x11};
  finger_usartSend(empty, 12);
}

// ���ģ���Ƿ�������
void finger_getChipEcho(void)
{
  unsigned char getChipEchoOrder[12] = {HEADER, ADDR, 0x01, 0x00, 0x03, 0x35, 0x00 ,0x39};
  finger_usartSend(getChipEchoOrder, 12);
}

// ����ͼ�����ݰ�
void finger_upImage(void)
{
  unsigned char upImageOrder[12] = {HEADER, ADDR, 0x01, 0x00, 0x03, 0x0A, 0x00, 0x0E};
  finger_usartSend(upImageOrder, 12);
}

// ȡ��
void finger_cancel(void)
{
  unsigned char cancelOrder[12] = {HEADER, ADDR, 0x01, 0x00, 0x03, 0x30, 0x00, 0x34};
  //unsigned char *buf = cancelOrder;
  finger_usartSend(cancelOrder, 12);
  fingerStruct.fingerMode = 0;
}

// ��ȡ��ָ֤��ID��
// stepNum-��ǰ������
void finger_getFingerID(unsigned char stepNum)
{
  switch(stepNum)
  {
  case 0: finger_getRnrollImage(); break;                            // ��ȡͼ��
  case 1: finger_getChar(1); break;                                  // ��������
  case 2: finger_searchMB(1, 0, 100); break;                         // ����ģ��
  default: finger_cancel(); break;
  }
}

// �Զ�ע��ģ��
// id-ID��/inputNum-¼�����
void finger_autoEnroll(unsigned int id, unsigned char inputNum)
{
  unsigned int check = 0, param = 0;
  unsigned char autoEnrollOrder[17] = {HEADER, ADDR, 0x01, 0x00, 0x08, 0x31, id/256, id%256, inputNum, 0x00, 0x00, 0x00, 0x00};
  
  finger_cancel();
  param = finger_getParam();
  autoEnrollOrder[13] = param / 256;
  autoEnrollOrder[14] = param % 256;
  check = finger_checkOut(autoEnrollOrder, 6, 16);
  autoEnrollOrder[15] = (unsigned char)check / 256;
  autoEnrollOrder[16] = (unsigned char)check % 256;
  finger_usartSend(autoEnrollOrder, 17);
  handleStruct.enrollFlag = 0;
  handleStruct.enrollNum = 0;
  fingerStruct.fingerMode = 1;
}

// �Զ���ָ֤��
// id-ID��/level-��ȫ�ȼ�
void finger_autoIdentify( unsigned int id, unsigned char level)
{
  unsigned char check = 0;
  unsigned char param = 0;
  unsigned char autoIdentifyOrder[17] = {HEADER, ADDR, 0x01, 0x00, 0x08, 0x32, level, id/256, id%256, 0x00, 0x00, 0x00, 0x00};
  
  finger_cancel();
  param = finger_getParam();
  autoIdentifyOrder[13] = param / 256;
  autoIdentifyOrder[14] = param % 256;
  check = finger_checkOut(autoIdentifyOrder, 6, 17);
  autoIdentifyOrder[15] = (unsigned char)check / 256;
  autoIdentifyOrder[16] = (unsigned char)check % 256;
  finger_usartSend(autoIdentifyOrder, 17);
  handleStruct.identifyFlag = 0;
  fingerStruct.fingerMode = 2;
}

// ɾ���ַ���
//buf-��Ҫɾ�����ַ���/len-��ȡ�����ݳ���/bufLen-buf���ݳ���
void finger_delbuf(unsigned char *buf, unsigned char len, unsigned char bufLen)
{
  for(unsigned char i=0; i<bufLen; i++)
  {
    buf[i] = buf[len++];
  }
}

// �Զ���ָ֤�ƽ���
// ���ش�����
unsigned char finger_errorAnalysis(unsigned char ch1, unsigned char ch2)
{
  unsigned char num = 0;
  if(ch1 == 0x00)                                             // �ɹ�
  {
    if(fingerStruct.fingerMode == 1)
    {
      if(ch2 == 0x01)                                         // �ȴ���ͼ�ɹ�
        num = 20;
      else if(ch2 == 0x02)                                    // �ϳ������ɹ�
        num = 21;
      else if(ch2 == 0x03)                                    // ָ��¼��ɹ����ȴ���ָ�뿪���������ã�
        num = 22;
      else if(ch2 == 0x04)                                    // �ϳ�ģ��ɹ�
        num = 23;
      else if(ch2 == 0x05)                                    // ָ���ظ����
        num = 24;
      else if(ch2 == 0x06)                                    // �ǼǸ�ģ������
        num = 25;
    }
    else if(fingerStruct.fingerMode == 2)
    {
      if(ch2 == 0x01)                                         // �ɼ��ɹ�
        num = 26;                                           
      else if(ch2 == 0x05)                                    // ��֤�ɹ�
        num = 27;
    }
  }
  else if(ch1 == 0x01)
    num = 1;                                                // ʧ��
  else if(ch1 == 0x07)
    num = 2;                                                // ��������ʧ��
  else if(ch1 == 0x09)
    num = 3;                                                // û������ָ��
  else if(ch1 == 0x0A)
    num = 4;                                                // �ϲ�ģ��ʧ��
  else if(ch1 == 0x0B)
    num = 5;                                                // ID�ų�����Χ
  else if(ch1 == 0x17)
    num = 6;                                                // ����ָ��
  else if(ch1 == 0x1F)
    num = 7;                                                // ָ�ƿ�����
  else if(ch1 == 0x22)
    num = 8;                                                // ָ��ģ�岻Ϊ��
  else if(ch1 == 0x23)
    num = 9;                                                // ָ��ģ��Ϊ��
  else if(ch1 == 0x24)
    num = 10;                                                // ָ�ƿ�Ϊ��
  else if(ch1 == 0x25)
    num = 11;                                                // ¼��������ô���
  else if(ch1 == 0x26)
    num = 11;                                                // ��ʱ
  else if(ch1 == 0x27)
    num = 12;                                                // ��ʾָ���Ѵ���
  return num;
}

void finger_fingerStructReset(void)
{
  fingerStruct.pageNum = 0;
  fingerStruct.errorNum = 0;
  fingerStruct.validTempNum = 0;
  fingerStruct.verifyScore = 0;
  handleStruct.enrollFlag=0;
}

int finger_usartInput(char ch)
{
  usartFinger.usartRecBuff[usartFinger.usartRecLen++] = ch;
  return 0;
}

// ���ڽ���
void finger_uartAnalysis(void)
{
  unsigned int orderLen = 0;
  unsigned int lastLen = 0;
  unsigned int check = 0;
  
  if(usartFinger.usartRecLen >= 10)
    for(unsigned int i=0; i<usartFinger.usartRecLen; i++)
    {
      if(usartFinger.usartRecBuff[i] == 0xEF && usartFinger.usartRecBuff[i+1] == 0x01)
      {
        lastLen = i;
        finger_fingerStructReset();
        orderLen = usartFinger.usartRecBuff[lastLen + 7] *256 + usartFinger.usartRecBuff[lastLen + 8];// ��ȡ�����
        if(usartFinger.usartRecLen >= orderLen + lastLen + 9)   // ��ǰ�����Ƿ��ָ�
        {
          check = finger_checkOut(usartFinger.usartRecBuff, lastLen + 6, orderLen + lastLen + 7);
          if((usartFinger.usartRecBuff[orderLen + lastLen + 7] == check / 256) 
             && (usartFinger.usartRecBuff[orderLen + lastLen + 8] == check % 256)) // ���У��ɹ�
          {
            fingerStruct.errorNum = finger_errorAnalysis(usartFinger.usartRecBuff[lastLen + 9], usartFinger.usartRecBuff[lastLen + 10]);
            if(fingerStruct.errorNum == 25)                     // �ɼ�ָ�Ƴɹ�
            {
              handleStruct.enrollFlag = 1;
              finger_cancel();
            }
            else if(fingerStruct.errorNum == 27)                // ��ָ֤�Ƴɹ�
            {
              handleStruct.identifyFlag = 1;
              finger_cancel();
            }
            if(fingerStruct.fingerMode == 1 && usartFinger.usartRecBuff[lastLen + 9] == 0x00)    // ��ȡ��ǰ¼��Ĵ���
            {
              handleStruct.enrollNum = usartFinger.usartRecBuff[lastLen + 11];
            }
            /*else if(fingerStruct.fingerMode == 5) // �ֶ���ָ֤�ƴ���
            {
              if(usartFinger.usartRecBuff[lastLen + 8] == 0x03 && usartFinger.usartRecBuff[lastLen + 9] == 0x00)
              {
                fingerStruct.fingerMode = 6;
                getFingerID(1);
              }
              else
                getFingerID(0);
            }
            else if(fingerStruct.fingerMode == 6)
            {
              if(usartFinger.usartRecBuff[lastLen + 9] == 0x00)
              {
                fingerStruct.fingerMode = 7;
                getFingerID(2);
              }
              else
              {
                if(usartFinger.usartRecBuff[lastLen + 9] == 0x01)
                  fingerStruct.errorNum = 13;
                else if(usartFinger.usartRecBuff[lastLen + 9] == 0x06)
                  fingerStruct.errorNum = 14;
                else if(usartFinger.usartRecBuff[lastLen + 9] == 0x07)
                  fingerStruct.errorNum = 15;
                else if(usartFinger.usartRecBuff[lastLen + 9] == 0x15)
                  fingerStruct.errorNum = 16;
                cancel();
              }
            }
            else if(fingerStruct.fingerMode == 7)
            {
              if(usartFinger.usartRecBuff[lastLen + 9] == 0x00) // ��֤�ɹ�
              {
                fingerStruct.fingerMode = 0;
                fingerStruct.pageNum = usartFinger.usartRecBuff[lastLen + 10] * 256 + usartFinger.usartRecBuff[lastLen + 11];
                fingerStruct.verifyScore = usartFinger.usartRecBuff[lastLen + 12] * 256 + usartFinger.usartRecBuff[lastLen + 13];
                fingerStruct.errorNum = 27;
              }
              else                                    // ��֤ʧ��
              {
                if(usartFinger.usartRecBuff[lastLen + 9] == 0x01)
                  fingerStruct.errorNum = 13;
                else if(usartFinger.usartRecBuff[lastLen + 9] == 0x09)
                  fingerStruct.errorNum = 17;
                cancel();
              }
            }*/
            finger_delbuf(usartFinger.usartRecBuff, orderLen + lastLen + 9, usartFinger.usartRecLen);
            usartFinger.usartRecLen -= orderLen + lastLen + 9;
            i = 0;
          }
        }
      }
    }
}

int finger_getIdentifyInfo(void)
{
  if(fingerStruct.errorNum == 27)
  {
    finger_fingerStructReset();
    return 1;
  }
  else if((fingerStruct.errorNum <= 12) && (fingerStruct.errorNum != 0))
  {
    finger_fingerStructReset();
    return -1;
  }
  return 0;
}

int finger_getEnrollInfo(void)
{
  if(handleStruct.enrollFlag == 0 && handleStruct.enrollNum == 0)
  {
    return -1;
  }
  else if(handleStruct.enrollFlag == 1)
  {
    finger_fingerStructReset();
    finger_cancel();
    return 0;
  }
  else
    return handleStruct.enrollNum;
}

/*
PROCESS(finger_process, "finger_process");

PROCESS_THREAD(finger_process, ev, data)
{
    static struct etimer finger_et;
    static struct etimer cancel_et;
    PROCESS_BEGIN();
    uart3_init(57600);
    etimer_set(&finger_et, 100);
    while(1)
    {
        PROCESS_WAIT_EVENT();
        static unsigned char fingerNum = 0;
        if(ev == key_event)
        {
            switch(*((unsigned char*)data))
            {
            case K1_PREESED: autoEnroll(fingerNum, 4, &paramStruct); break;
            case K2_PREESED: getFingerID(0); fingerStruct.fingerMode = 5; break;
            case K3_PREESED: fingerNum++; break;
            case K4_PREESED: cancel(); break;
            default: cancel(); break;
            }
        }
        if(ev == PROCESS_EVENT_TIMER)
        {
            if(etimer_expired(&finger_et))
            {
                etimer_set(&finger_et, 100);
                uartAnalysis(usartFinger.usartRecBuff, usartFinger.usartRecLen);
                if(fingerStruct.errorNum == 25)
                {
                    etimer_set(&cancel_et, 1000);
                    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&cancel_et));
                    cancel();                           // �ǼǸ�ģ������
                }
                else if(fingerStruct.errorNum == 27)
                {
                    
                }
                else
                {
                    switch(fingerStruct.errorNum)
                    {
                    case 1: break;                      // ʧ��
                    case 2: break;                      // ��������ʧ��
                    case 3: break;                      // û������ָ��
                    case 4: break;                      // �ϲ�ģ��ʧ��
                    case 5: break;                      // ID�ų�����Χ
                    case 6: break;                      // ����ָ��
                    case 7: break;                      // ָ�ƿ�����
                    case 8: break;                      // ָ��ģ�岻Ϊ��
                    case 9: break;                      // ָ��ģ��Ϊ��
                    case 10: break;                      // ָ�ƿ�Ϊ��
                    case 11: break;                      // ��ʱ
                    case 12: break;                      // ��ʾָ���Ѵ���
                    case 13: break;                     // �հ�����
                    case 14: break;                      // ָ��̫�Ҷ�����������
                    case 15: break;                      // ������̫������������
                    case 16: break;                      // ͼ�񻺳���û����Чԭʼͼ��������ͼ��
                    case 17: break;                     // û������������ʱ��Ӧҳ����÷�
                    
                    case 20: break;                     // �ȴ���ͼ�ɹ�
                    case 21: break;                     // �ϳ������ɹ�
                    case 22: break;                     // ָ��¼��ɹ����ȴ���ָ�뿪���������ã�
                    case 23: break;                     // �ϳ�ģ��ɹ�
                    case 24: break;                     // ָ���ظ����
                    case 26: break;                     // �ɼ��ɹ�
                    //case 27: break;                     // ��֤�ɹ�
                    }
                }
            }
        }
    }
    
    PROCESS_END();
}
*/





