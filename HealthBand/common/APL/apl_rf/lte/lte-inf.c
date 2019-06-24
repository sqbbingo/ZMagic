#include "lte-inf.h"
#include "rfUart.h"

/*gsm�ײ�ӿں���*/

#define RECV_BUF_SIZE   256  //contiki ringbuf ����ܳ���256
static struct ringbuf _recv_ring;

int gsm_recv_ch(char ch)
{
  return ringbuf_put(&_recv_ring, ch);
}

void gsm_inf_init(void)
{
  static uint8_t _recv[RECV_BUF_SIZE];
  
  ringbuf_init(&_recv_ring, _recv, (uint8_t)RECV_BUF_SIZE);
}

void gsm_uart_write(char *buf, int len)
{
  for (int i=0; i<len; i++) {
    rfUartSendByte(2,buf[i]);
  }
}

int gsm_uart_read_ch(void)
{
  return ringbuf_get(&_recv_ring);
}