
/*********************************************************************************************
* �ļ���zhiyun.c
* ���ߣ�xuzhy 
* ˵����
*       
*       
*      
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
#include "lte_zhiyun.h"
#include "w25qxx.h"
#include "zxbee.h"
#include "sensor.h"

#define DEBUG 1

#if DEBUG
#define Debug(x...) printf(x)
#else
#define Debug(x...) do{}while(0)
#endif

#define LTE_CONFIG_ADDR   4096

static tcp_t* tcp_con;
static struct etimer timer; //
extern lte_config_t  lteConfig;


lte_config_t  lteConfig = {
  .id = AID,
  .key = AKEY,
  .ip = CONFIG_ZHIYUN_IP, 
  .port = CONFIG_ZHIYUN_PORT,
};


/*********************************************************************************************
* ���ƣ�lteConfig_init(void)
* ���ܣ����ó�ʼ������
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int lteConfig_init(void)
{
  uint8_t FlashFlag = 0;
  
  W25QXX_Init();
  W25QXX_Read(&FlashFlag,LTE_CONFIG_ADDR,1);
  if(FlashFlag != 0x5A)
  {
    FlashFlag = 0x5A;
    W25QXX_Write(&FlashFlag,LTE_CONFIG_ADDR,1);
    W25QXX_Write((uint8_t*)&lteConfig.id,LTE_CONFIG_ADDR+4,sizeof(lteConfig));
  }
  else
  {
    W25QXX_Read((uint8_t*)&lteConfig.id,LTE_CONFIG_ADDR+4,sizeof(lteConfig));
  }
  
  return 0;
}


/*********************************************************************************************
* ���ƣ�
* ���ܣ�
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void lteConfig_save(void)
{
  uint8_t FlashFlag = 0;
  
  W25QXX_Read(&FlashFlag,LTE_CONFIG_ADDR,1);
  if(FlashFlag != 0x5A)
  {
    FlashFlag = 0x5A;
    W25QXX_Write(&FlashFlag,LTE_CONFIG_ADDR,1);
  }
  W25QXX_Write((uint8_t*)&lteConfig.id,LTE_CONFIG_ADDR+4,sizeof(lteConfig));
}


uint8_t Lte_GetTcpConnect()
{
  if (tcp_con->status == TCP_STATUS_CONNECTED){
    return 1;
  }
  return 0;
}

/*********************************************************************************************
* ���ƣ�package_auth
* ���ܣ�����������֤���ݱ�
* ������
* ���أ���֤���ݱ�����
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int package_auth(void)
{
  /*
  * ��֤���ݱ���ʽ
  *{"method":"authenticate","uid":"12345","key":"3Wl6cI8TNi8fMkhCXkZzXzCTsst4LgqY","addr":"adfbc"}
  */
  char *pbuf = gsm_tcp_buf();
  if (pbuf == NULL) {
    Debug("package_auth(): error tcp buffer busy.\r\n");
    return -1;
  }
  sprintf(pbuf,
          "{\"method\":\"authenticate\",\"uid\":\"%s\",\"key\":\"%s\",\"addr\":\"LTE:%s\",\"version\":\"0.1.0\", \"autodb\":true}",
          lteConfig.id, lteConfig.key, gsm_info.imei);
  
  return strlen(pbuf);
}
/*********************************************************************************************
* ���ƣ�package_heartbeat
* ���ܣ������������ݱ�
* ������
* ���أ��������ݱ�����
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int package_heartbeat(void)
{
  char *pbuf = gsm_tcp_buf();
  static unsigned int seq = 0;
  if (pbuf == NULL) {
    Debug("package_heartbeat(): error tcp buffer busy.\r\n");
    return -1;
  }
  sprintf(pbuf, "{\"method\":\"echo\", \"timestamp\":%u, \"seq\":%u}", 
          clock_time(), seq++);
  return strlen(pbuf);
}

/*********************************************************************************************
* ���ƣ�package_data
* ���ܣ����ɴ����������ݱ�,������
* ������
* ���أ����������ݱ�����
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int package_data_send(char *zxbee)
{
  if (tcp_con->status != TCP_STATUS_CONNECTED){
    return -1;
  }
  char *pbuf = gsm_tcp_buf();
  if (pbuf == NULL) {
    Debug("package_data(): error tcp buffer busy.\r\n");
    return -2;
  }
  int len = sprintf(pbuf, "{\"method\":\"sensor\",\"data\":\"%s\"}", zxbee); 
  gsm_tcp_send(tcp_con, len);
  /*�޸�������ʱ��*/
  etimer_set(&timer, CLOCK_SECOND*60);
  return len;
}

/*********************************************************************************************
* ���ƣ�package_decode
* ���ܣ������û��������ݱ�
* ������buf���������ݱ�����
*       len�����ݱ�����
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
static void package_decode(char *buf, int len)
{
  //  if (buf[len-1] != 0) {
  //    Debug("error: zhiyun  package format\r\n");
  //    return;
  //  }
  buf[len-1] = 0;
  len --;
  if (strstr(buf, "method") && strstr(buf, "authenticate_rsp")) {
    if (strstr(buf, "status") && strstr(buf, "ok")) {
      void sensorLinkOn(void);
      sensorLinkOn();
    }
  } else
    if (strstr(buf, "method") && strstr(buf, "control")) {
      char *pdat = strstr(buf, "data");
      if (pdat) {
        pdat = strstr(pdat, ":");
        pdat += 2;
        buf[len - 2] = 0;
        Debug("zhiyun --> RD: %s\r\n", pdat);
        _zxbee_onrecv_fun(2,pdat,strlen(pdat));//recv data
      }
    }
}

/*********************************************************************************************
* ���ƣ�zhiyun_send
* ���ܣ�zxbee�ײ㷢�ͽӿ�
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
short zhiyun_send(char *pkg)
{
  return package_data_send(pkg);
}


lte_config_t Lte_GetConfig()
{
  return lteConfig;
}

/*********************************************************************************************
* ���ƣ�u_zhiyun
* ���ܣ�lteģ�������Ʒ����������߳�
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
PROCESS(u_zhiyun, "u_zhiyun");

PROCESS_THREAD(u_zhiyun, ev, data)
{
  
  PROCESS_BEGIN();
  
  tcp_con = tcp_alloc();
  lteConfig_init();
  etimer_set(&timer, CLOCK_SECOND); 
  while (1) {
    PROCESS_YIELD();
    if (ev == evt_tcp) {
      tcp_t *ptcp = (tcp_t *) data;
      if (ptcp->pdat != NULL) {
        package_decode(ptcp->pdat, ptcp->datlen);
        free(ptcp->pdat);
        ptcp->pdat = NULL;
      } else if (ptcp->status == TCP_STATUS_CONNECTED) {
        Debug("tcp(%u) connected\r\n", ptcp->id);
        
        int len = package_auth();
        gsm_tcp_send(ptcp, len);
        
        etimer_set(&timer, CLOCK_SECOND*60); //��������ʱ��
      } else if (ptcp->status == TCP_STATUS_CLOSED) {
        Debug("zhiyun tcp(%u) closed\r\n", ptcp->id);
        
        etimer_set(&timer, CLOCK_SECOND); //������ʱ
      }
    }
    
    if (ev == PROCESS_EVENT_TIMER && etimer_expired(&timer)) {
      if (gsm_info.ppp_status != 2) {
        etimer_set(&timer, CLOCK_SECOND);
      } else {
        if (tcp_con->status == TCP_STATUS_CLOSED || tcp_con->status == TCP_STATUS_OPEN) {
          Debug("zhiyun tcp(%u) open\r\n", tcp_con->id);
          gsm_tcp_open(tcp_con, CONFIG_ZHIYUN_IP, CONFIG_ZHIYUN_PORT);
          etimer_set(&timer, CLOCK_SECOND*5); //��ʱ����
        } 
        if (tcp_con->status == TCP_STATUS_CONNECTED) {
          
          /*����������*/
          int len = package_heartbeat();
          gsm_tcp_send(tcp_con, len);
          etimer_set(&timer, CLOCK_SECOND*60);
        }
      }
    }
  }
  PROCESS_END();
}

