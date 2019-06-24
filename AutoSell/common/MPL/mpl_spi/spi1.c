#include "spi.h"	 

#define FAST_SPI 1

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
/*********************************************************************************************
* ���ƣ�SPI1_Init
* ���ܣ�SPI1��ʼ��
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void SPI1_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//ʹ��SPI1ʱ��
  RCC_AHB1PeriphClockCmd(SPI1_GPIO_CLK, ENABLE);//ʹ��GPIOʱ��
  //GPIO��ʼ������
  GPIO_InitStructure.GPIO_Pin = SPI1_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(SPI1_SCK_GPIO, &GPIO_InitStructure);//��ʼ��
  
  GPIO_InitStructure.GPIO_Pin = SPI1_MISO_PIN;
  GPIO_Init(SPI1_MISO_GPIO, &GPIO_InitStructure);//��ʼ��
  
  GPIO_InitStructure.GPIO_Pin = SPI1_MOSI_PIN;
  GPIO_Init(SPI1_MOSI_GPIO, &GPIO_InitStructure);//��ʼ��
	
  GPIO_PinAFConfig(SPI1_SCK_GPIO,SPI1_SCK_SOURCE_PIN,GPIO_AF_SPI1); //����Ϊ SD_SPI
  GPIO_PinAFConfig(SPI1_MISO_GPIO,SPI1_MISO_SOURCE_PIN,GPIO_AF_SPI1); //����Ϊ SD_SPI
  GPIO_PinAFConfig(SPI1_MOSI_GPIO,SPI1_MOSI_SOURCE_PIN,GPIO_AF_SPI1); //����Ϊ SD_SPI
	
  SPI_I2S_DeInit(SPI1);
	
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
  SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
  SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
  
  SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
  SPI1_ReadWriteByte(0xff);
}   

/*********************************************************************************************
* ���ƣ�SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
* ���ܣ�SPI1�ٶ����ú���
* ������SPI_BaudRatePrescaler
* ���أ�
* �޸ģ�
* ע�ͣ�SPI�ٶ�=fAPB2/��Ƶϵ��
SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
fAPB2ʱ��һ��Ϊ84Mhz
*********************************************************************************************/
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI1->CR1|=1<<6; 		//SPI�豸ʹ��	  
} 

/*********************************************************************************************
* ���ƣ�SPI1_ReadWriteByte(u8 TxData)
* ���ܣ�SPI1 ��дһ���ֽ�
* ������TxData:Ҫд����ֽ�
* ���أ���ȡ�����ֽ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
//#pragma inline

u8 SPI1_ReadWriteByte(u8 TxData)
{
  uint16_t timeout = 0;
#if (FAST_SPI==0)
  
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)//�ȴ���������
  {
    timeout++;
    if(timeout>1999) return 0;
  }  
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ��byte����
  timeout = 0;
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//�ȴ�������һ��byte  
  {
    timeout++;
    if(timeout>1999) return 0;
  }  
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����	
  
#else
  
	while((SPI1->SR&1<<1)==0)		//�ȴ��������� 
  {
    timeout++;
    if(timeout > 4999) return 0;
  }
	SPI1->DR=TxData;              //����һ��byte  
  timeout = 0;
	while((SPI1->SR&1<<0)==0)		//�ȴ�������һ��byte  
  {
    timeout++;
    if(timeout > 4999) return 0;
  }
 	return SPI1->DR;          		//�����յ�������		
	
#endif
}
