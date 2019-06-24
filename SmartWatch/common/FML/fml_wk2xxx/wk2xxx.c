#include "wk2xxx.h"

/*����һ��������*/

static uint8_t Wk2114_IrqStatus = 0;
static char Wk2114_Buffer = 0;

int Wk2114_UartIrqCallback(char byte)
{
  Wk2114_IrqStatus = 1;
  Wk2114_Buffer = byte;
  return 0;
}

//��ȡ�������� 
int Wk2114_ReadByte(void) 
{  
  uint16_t timeout = 0;
  
  while(Wk2114_IrqStatus == 0)
  {
    delay_us(1);
    timeout++;
    if(timeout > 899)
      return 0;
  }
  Wk2114_IrqStatus = 0;
  return Wk2114_Buffer;
}

//ͨ������д�� 1 ���ֽڵ����ݣ�byte Ϊ���� 
void Wk2114_WriteByte(unsigned char byte) 
{ 
  uart2_putc(byte);
  delay_us(50);//��ʱ�Ƚ���Ҫ
} 


void WK2114_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                //����
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;              //������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;            //100MHz
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //��Ӧ����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIO
  
  Exti2_Init(EXTI_PortSourceGPIOD);
  Exti2IrqCall_Set(Wk2114_ExIrqCallback);
  
  uart2_init(460800);//wk2114������,������Ҫ��һ��
  uart2_set_input(Wk2114_UartIrqCallback);
  
	Wk2114_config();
}

void Wk2114_Rest()
{
  WK2114_REST_L;
  delay_ms(300);
  WK2114_REST_H;
  delay_ms(20);
}

void Wk2114_RestInit()
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  
  RCC_AHB1PeriphClockCmd(WK2114_REST_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = WK2114_REST_PIN;
  GPIO_Init(WK2114_REST_GPIO, &GPIO_InitStructure);
  
  WK2114_REST_H;
}


/********************************************************************************************/

/**************************************Wk2114WriteReg***********************************/
//�������ܣ�д�Ĵ���������ǰ���ǸüĴ�����д��ĳЩ�Ĵ��������д1�����ܻ��Զ���1������������ֲ�)
//������port:Ϊ�Ӵ��ڵ���(C0C1)
//      reg:Ϊ�Ĵ����ĵ�ַ(A3A2A1A0)
//      byte:Ϊд��Ĵ���������
//ע�⣺���Ӵ��ڱ���ͨ������£���FDATд������ݻ�ͨ��TX�������
//*************************************************************************/
static void Wk2114WriteReg(unsigned char port,unsigned char reg,unsigned char byte)
{	 
  Wk2114_WriteByte(((port-1)<<4)+reg);	//дָ�����ָ��Ĺ��ɼ������ֲ�
  Wk2114_WriteByte(byte);//д����
}


/*************************************Wk2114ReadReg************************************/
//�������ܣ����Ĵ�������
//������port:Ϊ�Ӵ��ڵ���(C0C1)
//      reg:Ϊ�Ĵ����ĵ�ַ(A3A2A1A0)
//      rec_bytea:Ϊ��ȡ���ļĴ���ֵ
//ע�⣺���Ӵ��ڱ���ͨ������£���FDAT��ʵ���Ͼ��Ƕ�ȡuart��rx���յ�����
/*************************************************************************/
static unsigned char Wk2114ReadReg(unsigned char port,unsigned char reg)
{	 
  unsigned char rec_bytea=0;
  Wk2114_WriteByte(0x40+((port-1)<<4)+reg); //дָ�����ָ��Ĺ��ɼ������ֲ�
  rec_bytea=Wk2114_ReadByte();	 //���շ��صļĴ���ֵ									
  return rec_bytea;
}



/**************************************Wk2114writeFIFO***********************************/
//�������ܣ�дFIFO�������ú���д������ݻ�ͨ��uart��TX���ͳ�ȥ)
//������port:Ϊ�Ӵ��ڵ���(C0C1)
//      *byte��Ϊд������ָ��
//      num��Ϊд�����ݵĸ�����������16���ֽڣ�N3N2N1N0��
/*************************************************************************/
//void Wk2114writeFIFO(unsigned char port,unsigned char *send_da,unsigned char num)
//{	 
//    unsigned char i;
//    Wk2114_WriteByte(0x80+((port-1)<<4)+(num-1)); //дָ��,����ָ��ɼ������ֲ�
//    for(i=0;i<num;i++)
//    {
//        Wk2114_WriteByte( *(send_da+i));//д����
//    }
//}

/***************************************Wk2114readFIFO**********************************/
//�������ܣ���FIFO�������ú�����ȡ��������FIFO�����е����ݣ�ʵ������uart��rx���յ�����)
//������port:Ϊ�Ӵ��ڵ���(C0C1)
//      *byte��Ϊ��������ָ��
//      num��Ϊ�������ݵĸ�����������16���ֽڣ�N3N2N1N0��
/*************************************************************************/
//unsigned char *Wk2114readFIFO(unsigned char port,unsigned char num)
//{
//    unsigned char n;
//    unsigned char byte[256];
//    Wk2114_WriteByte(0xc0+((port-1)<<4)+(num-1));
//    for(n=0;n<num;n++)
//    {	
//        byte[n]=Wk2114_ReadByte();	
//    }
//    return byte;
//}

/******************************Wk2114PortInit*******************************************/
//�������ܣ���������Ҫ���ʼ��һЩоƬ�����Ĵ�����
/*********************************************************************************/
void Wk2114PortInit(unsigned char port)
{
  unsigned char gena,grst,gier,sier,scr;
	//ʹ���Ӵ���ʱ��
  gena=Wk2114ReadReg(WK2XXX_GPORT,WK2XXX_GENA); 
	switch (port)
  {
  case 1://ʹ���Ӵ���1��ʱ��
    gena|=WK2XXX_UT1EN;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GENA,gena);
    break;
  case 2://ʹ���Ӵ���2��ʱ��
    gena|=WK2XXX_UT2EN;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GENA,gena);
    break;
  case 3://ʹ���Ӵ���3��ʱ��
    gena|=WK2XXX_UT3EN;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GENA,gena);
    break;
  case 4://ʹ���Ӵ���4��ʱ��
    gena|=WK2XXX_UT4EN;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GENA,gena);
    break;
  }	
	//�����λ�Ӵ���
	grst=Wk2114ReadReg(WK2XXX_GPORT,WK2XXX_GRST); 
	switch (port)
  {
  case 1://�����λ�Ӵ���1
    grst|=WK2XXX_UT1RST;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GRST,grst);
    break;
  case 2://�����λ�Ӵ���2
    grst|=WK2XXX_UT2RST;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GRST,grst);
    break;
  case 3://�����λ�Ӵ���3
    grst|=WK2XXX_UT3RST;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GRST,grst);
    break;
  case 4://�����λ�Ӵ���4
    grst|=WK2XXX_UT4RST;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GRST,grst);
    break;
  }	
  //ʹ���Ӵ����жϣ������Ӵ������жϺ��Ӵ����ڲ��Ľ����жϣ��������жϴ���
	gier=Wk2114ReadReg(WK2XXX_GPORT,WK2XXX_GIER); 
	switch (port)
  {
  case 1:
    gier|=WK2XXX_UT1RST;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GIER,gier);
    break;
  case 2:
    gier|=WK2XXX_UT2RST;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GIER,gier);
    break;
  case 3:
    gier|=WK2XXX_UT3RST;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GIER,gier);
    break;
  case 4:
    gier|=WK2XXX_UT4RST;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GIER,gier);
    break;
  }	 
  //ʹ���Ӵ��ڽ��մ����жϺͳ�ʱ�ж�
  sier=Wk2114ReadReg(port,WK2XXX_SIER); 
  sier |= WK2XXX_RFTRIG_IEN|WK2XXX_RXOUT_IEN;
  sier |= WK2XXX_RFTRIG_IEN;
  Wk2114WriteReg(port,WK2XXX_SIER,sier);
  // ��ʼ��FIFO�����ù̶��жϴ���
  Wk2114WriteReg(port,WK2XXX_FCR,0XFF);
  //���������жϴ��㣬��������������Ч����ô����FCR�Ĵ����жϵĹ̶��жϴ��㽫ʧЧ
  Wk2114WriteReg(port,WK2XXX_SPAGE,1);//�л���page1
  Wk2114WriteReg(port,WK2XXX_RFTL,0X01);//���ý��մ���1���ֽ�
  Wk2114WriteReg(port,WK2XXX_TFTL,0xff);//���÷��ʹ���Ϊ256���ֽ�
  Wk2114WriteReg(port,WK2XXX_SPAGE,0);//�л���page0 
  //ʹ���Ӵ��ڵķ��ͺͽ���ʹ��
  scr=Wk2114ReadReg(port,WK2XXX_SCR); 
  scr|=WK2XXX_TXEN|WK2XXX_RXEN;
  Wk2114WriteReg(port,WK2XXX_SCR,scr);
}
/******************************Wk2114Close*******************************************/
//�������ܣ���������رյ�ǰ�Ӵ��ڣ��͸�λ��ʼֵ��
/*********************************************************************************/

void Wk2114Close(unsigned char port)
{
  unsigned char gena,grst;
	//��λ�Ӵ���
	grst=Wk2114ReadReg(WK2XXX_GPORT,WK2XXX_GRST); 
	switch (port)
  {
  case 1://�����λ�Ӵ���1
    grst&=~WK2XXX_UT1RST;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GRST,grst);
    break;
  case 2://�����λ�Ӵ���2
    grst&=~WK2XXX_UT2RST;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GRST,grst);
    break;
  case 3://�����λ�Ӵ���3
    grst&=~WK2XXX_UT3RST;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GRST,grst);
    break;
  case 4://�����λ�Ӵ���4
    grst&=~WK2XXX_UT4RST;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GRST,grst);
    break;
  }	
	//�ر��Ӵ���ʱ��
  gena=Wk2114ReadReg(WK2XXX_GPORT,WK2XXX_GENA); 
	switch (port)
  {
  case 1://ʹ���Ӵ���1��ʱ��
    gena&=~WK2XXX_UT1EN;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GENA,gena);
    break;
  case 2://ʹ���Ӵ���2��ʱ��
    gena&=~WK2XXX_UT2EN;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GENA,gena);
    break;
  case 3://ʹ���Ӵ���3��ʱ��
    gena&=~WK2XXX_UT3EN;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GENA,gena);
    break;
  case 4://ʹ���Ӵ���4��ʱ��
    gena&=~WK2XXX_UT4EN;
    Wk2114WriteReg(WK2XXX_GPORT,WK2XXX_GENA,gena);
    break;
  }	
}
/**************************Wk2114SetBaud*******************************************************/
//�������ܣ������Ӵ��ڲ����ʺ������˺����в����ʵ�ƥ��ֵ�Ǹ���11.0592Mhz�µ��ⲿ��������
// port:�Ӵ��ں�
// baud:�����ʴ�С.�����ʱ�ʾ��ʽ��
//
/**************************Wk2114SetBaud*******************************************************/
void Wk2114SetBaud(unsigned char port,int baud)
{  
	unsigned char baud1,baud0,pres,scr;
	//���²�������Ӧ�ļĴ���ֵ�������ⲿʱ��Ϊ11.0592������¼������ã����ʹ������������Ҫ���¼���
	switch (baud) 
	{
  case 600:
    baud1=0x4;
    baud0=0x7f;
    pres=0;
    break;
  case 1200:
    baud1=0x2;
    baud0=0x3F;
    pres=0;
    break;
  case 2400:
    baud1=0x1;
    baud0=0x1f;
    pres=0;
    break;
  case 4800:
    baud1=0x00;
    baud0=0x8f;
    pres=0;
    break;
  case 9600:
    baud1=0x00;
    baud0=0x47;
    pres=0;
    break;
  case 19200:
    baud1=0x00;
    baud0=0x23;
    pres=0;
    break;
  case 38400:
    baud1=0x00;
    baud0=0x11;
    pres=0;
    break;
  case 76800:
    baud1=0x00;
    baud0=0x08;
    pres=0;
    break; 
    
  case 1800:
    baud1=0x01;
    baud0=0x7f;
    pres=0;
    break;
  case 3600:
    baud1=0x00;
    baud0=0xbf;
    pres=0;
    break;
  case 7200:
    baud1=0x00;
    baud0=0x5f;
    pres=0;
    break;
  case 14400:
    baud1=0x00;
    baud0=0x2f;
    pres=0;
    break;
  case 28800:
    baud1=0x00;
    baud0=0x17;
    pres=0;
    break;
  case 57600:
    baud1=0x00;
    baud0=0x0b;
    pres=0;
    break;
  case 115200:
    baud1=0x00;
    baud0=0x05;
    pres=0;
    break;
  case 230400:
    baud1=0x00;
    baud0=0x02;
    pres=0;
    break;
  default:
    baud1=0x00;
    baud0=0x00;
    pres=0;
  }
	//�ص��Ӵ����շ�ʹ��
	scr=Wk2114ReadReg(port,WK2XXX_SCR); 
	Wk2114WriteReg(port,WK2XXX_SCR,0);
	//���ò�������ؼĴ���
	Wk2114WriteReg(port,WK2XXX_SPAGE,1);//�л���page1
	Wk2114WriteReg(port,WK2XXX_BAUD1,baud1);
	Wk2114WriteReg(port,WK2XXX_BAUD0,baud0);
	Wk2114WriteReg(port,WK2XXX_PRES,pres);
	Wk2114WriteReg(port,WK2XXX_SPAGE,0);//�л���page0 
	//ʹ���Ӵ����շ�ʹ��
	Wk2114WriteReg(port,WK2XXX_SCR,scr);
}


void Wk2114BaudAdaptive()
{
  Wk2114_Rest();
  Wk2114_WriteByte(0x55);
  delay_ms(200);
  
}

void Wk2114_config()
{
	//uint32_t baud = B38400;
  
  Wk2114_RestInit();
  Wk2114BaudAdaptive();
  
//	Wk2114PortInit(1);          //��ʼ������
//	Wk2114SetBaud(1,baud);  //�����Ӵ��ڲ�����
  
//	Wk2114PortInit(2);
//	Wk2114SetBaud(2,baud);
  
//	Wk2114PortInit(3);
//	Wk2114SetBaud(3,baud);
  
//	Wk2114PortInit(4);
//	Wk2114SetBaud(4,baud);	
  
	delay_ms(10);
}



/*WK2114�Ӵ��ڷ�������*/
void Wk2114_Uart1SendByte(char byte)
{
  Wk2114WriteReg(1,WK2XXX_FDAT,byte);	
}

void Wk2114_Uart2SendByte(char byte)
{
  Wk2114WriteReg(2,WK2XXX_FDAT,byte);	
}

void Wk2114_Uart3SendByte(char byte)
{
  Wk2114WriteReg(3,WK2XXX_FDAT,byte);	
}

void Wk2114_Uart4SendByte(char byte)
{
  Wk2114WriteReg(4,WK2XXX_FDAT,byte);	
}



int (*Wk2114_SlaveRecv[4])(char byte) = {NULL};

uint8_t Wk2114_SlaveRecv_Set(uint8_t index,int (*func)(char byte))
{
  if((index==0) || (index>4))
    return 1;
  Wk2114_SlaveRecv[index-1] = func;
  return 0;
}


void Wk2114_ExIrqCallback()
{
  uint8_t status=0;
  char byte1,byte2,byte3,byte4;
  
	do
	{
    status=Wk2114ReadReg(1,WK2XXX_GIFR);//�ж��ж�Դ
    
		if(status&WK2XXX_UT1INT)//�ж��Ӵ���1
		{
			while((Wk2114ReadReg(1,WK2XXX_FSR)&WK2XXX_RDAT))//�жϽ���fifo���Ƿ������ݣ�ֱ����FIFO����
			{
				byte1 = Wk2114ReadReg(1,WK2XXX_FDAT);
        if(Wk2114_SlaveRecv[0] != NULL)
          Wk2114_SlaveRecv[0](byte1);
			}	
		}
    
		if(status&WK2XXX_UT2INT)//�ж��Ӵ���2
		{
			while((Wk2114ReadReg(2,WK2XXX_FSR)&WK2XXX_RDAT))
			{
				byte2=Wk2114ReadReg(2,WK2XXX_FDAT);
        if(Wk2114_SlaveRecv[1] != NULL)
          Wk2114_SlaveRecv[1](byte2);
			}	
		} 
    
		if(status&WK2XXX_UT3INT)  //�ж��Ӵ���3
		{
			while((Wk2114ReadReg(3,WK2XXX_FSR)&WK2XXX_RDAT))
			{
				byte3 = Wk2114ReadReg(3,WK2XXX_FDAT);
        if(Wk2114_SlaveRecv[2] != NULL)
          Wk2114_SlaveRecv[2](byte3);
			}	
		}
    
		if(status&WK2XXX_UT4INT) //�ж��Ӵ���4
		{
			while((Wk2114ReadReg(4,WK2XXX_FSR)&WK2XXX_RDAT))
			{
				byte4 = Wk2114ReadReg(4,WK2XXX_FDAT);
        if(Wk2114_SlaveRecv[3] != NULL)
          Wk2114_SlaveRecv[3](byte4);
			}	
		}
	}
	while((0x0f&Wk2114ReadReg(1,WK2XXX_GIFR))!=0);  //�ж��Ƿ����жϣ�û���ж����˳�
}

