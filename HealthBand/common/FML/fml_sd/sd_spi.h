#ifndef _sd_spi_h_
#define _sd_spi_h_


#include "stm32f4xx.h"
#include "spi.h"	


// SD�����Ͷ���
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06	   
// SD��ָ���	   
#define CMD0    0       //����λ
#define CMD1    1
#define CMD8    8       //����8 ,SEND_IF_COND
#define CMD9    9       //����9 ,��CSD����
#define CMD10   10      //����10,��CID����
#define CMD12   12      //����12,ֹͣ���ݴ���
#define CMD16   16      //����16,����SectorSize ����0x00
#define CMD17   17      //����17,��sector
#define CMD18   18      //����18,�� Multi sector
#define CMD23   23      //����23,���ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24   24      //����24,дsector
#define CMD25   25      //����25,дMulti sector
#define CMD41   41      //����41,����0x00
#define CMD55   55      //����55,����0x01
#define CMD58   58      //����58,��OCR��Ϣ
#define CMD59   59      //����59,ʹ��/��ֹCRC,Ӧ����0x00
//����д���Ӧ������
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD����Ӧ��־��
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF
 					

#define SD_SPI_NSS_RCC    RCC_AHB1Periph_GPIOD
#define SD_SPI_NSS_GPIO   GPIOD
#define SD_SPI_NSS_PIN    GPIO_Pin_3

#define SD_CS_L           (SD_SPI_NSS_GPIO->BSRRH=SD_SPI_NSS_PIN)
#define SD_CS_H           (SD_SPI_NSS_GPIO->BSRRL=SD_SPI_NSS_PIN)
 
void SPI_init(void);
uint8_t SD_WaitReady(void);							
uint8_t SD_GetResponse(uint8_t Response);					
uint8_t SD_Init_Config(void);			  					
uint8_t SD_GetCID(uint8_t *cid_data);                    
uint8_t SD_GetCSD(uint8_t *csd_data);  				
uint32_t SD_GetSectorCount(void);                  
uint8_t SD_ReadDisk(uint8_t*buf,uint32_t sector,uint16_t cnt);		
uint8_t SD_WriteDisk(uint8_t*buf,uint32_t sector,uint16_t cnt);		

#endif


