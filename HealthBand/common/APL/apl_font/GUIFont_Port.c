/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   XBF��ʽ����emwin�����ӿ�
  ******************************************************************************
  */

#include "GUIFont_Port.h"   
#include "w25qxx.h"
#include "ff.h"

/*�ֿ�洢·��*/
#if (XBF_FONT_SOURCE == USE_FLASH_FONT)

  #define FONT_FLAG_ADDR  (29*4096)
	static const uint32_t  FONT_XINSONGTI_12_ADDR	 = (30*4096);
	static const uint32_t  FONT_XINSONGTI_16_ADDR	 = (380*4096);
	static const uint32_t  FONT_XINSONGTI_24_ADDR	 = (780*4096);

#elif (XBF_FONT_SOURCE == USE_SDCARD_FONT)

	static const char  FONT_STORAGE_ROOT_DIR[] =  "0:";
	static const char FONT_XINSONGTI_12_ADDR[] =  "0:Font/ST12.xbf";
	static const char FONT_XINSONGTI_16_ADDR[] =  "0:Font/ST16.xbf";
	static const char FONT_XINSONGTI_24_ADDR[] =  "0:Font/ST24.xbf";

#elif (XBF_FONT_SOURCE == USE_FLASH_FILESYSTEM_FONT)
	
	static const char FONT_STORAGE_ROOT_DIR[] =   "1:";
	static const char FONT_XINSONGTI_12_ADDR[] =  "0:Font/ST12.xbf";
	static const char FONT_XINSONGTI_16_ADDR[] =  "0:Font/ST16.xbf";
	static const char FONT_XINSONGTI_24_ADDR[] =  "0:Font/ST24.xbf";

#endif


/*�ֿ�ṹ��*/
GUI_XBF_DATA 	XBF_XINSONGTI_12_Data;
GUI_FONT     	FONT_XINSONGTI_12;

GUI_XBF_DATA 	XBF_XINSONGTI_16_Data;
GUI_FONT     	FONT_XINSONGTI_16;

GUI_XBF_DATA 	XBF_XINSONGTI_24_Data;
GUI_FONT     	FONT_XINSONGTI_24;


/*�ֿ�洢���ļ�ϵͳʱ��Ҫʹ�õı���*/
#if (XBF_FONT_SOURCE == USE_SDCARD_FONT || XBF_FONT_SOURCE == USE_FLASH_FILESYSTEM_FONT)
	extern FATFS fs[2];										/* Work area (file system object) for logical drives */
	static FIL fFont_file;										/* file objects */
	static FRESULT fFont_result; 
	static UINT fFont_num;            				/* File R/W count */
#endif

 /**
  * @brief  ��ȡ�������ݵĻص�����
  * @param  Offset��Ҫ��ȡ��������XBF�ļ��е�ƫ��λ��
  * @param  NumBytes��Ҫ��ȡ���ֽ���
	* @param  pVoid���Զ������ݵ�ָ��
  * @param  pBuffer���洢��ȡ���ݵ�ָ��
  * @retval 0 �ɹ�, 1 ʧ��
  */
static int _cb_FONT_XBF_GetData(U32 Offset, U16 NumBytes, void * pVoid, void * pBuffer)
{
#if (XBF_FONT_SOURCE == USE_FLASH_FONT)
	
	//��pVoid�л�ȡ�ֿ�Ĵ洢��ַ(pvoid��ֵ��GUI_XBF_CreateFont�д���)
	W25QXX_Read(pBuffer,*(uint32_t *)pVoid+Offset,NumBytes);
	
	return 0;
	
#elif (XBF_FONT_SOURCE == USE_SDCARD_FONT || XBF_FONT_SOURCE == USE_FLASH_FILESYSTEM_FONT)
	
	//��pVoid�л�ȡ�ֿ�Ĵ洢��ַ(pvoid��ֵ��GUI_XBF_CreateFont�д���)
	fFont_result = f_open(&fFont_file , (char *)pVoid, FA_OPEN_EXISTING | FA_READ);

	if ( fFont_result == FR_OK ) 
	{
			f_lseek (&fFont_file, Offset);		//ָ��ƫ��
		
			//��ȡ����
			fFont_result = f_read( &fFont_file, pBuffer, NumBytes, &fFont_num );
			
			f_close(&fFont_file);
			
			return 0;  
	}    
	else
			return 1; 

#endif

}

short LanguageDeviceInit()
{
#if (XBF_FONT_SOURCE == USE_FLASH_FONT)
  
  unsigned char buf[2]={0};
  W25QXX_Init();
  W25QXX_Read(buf,FONT_FLAG_ADDR,2);
  if((buf[0]==0xA0)&&(buf[1]==0x0A))
    return 0;
  return 1;
  
#elif (XBF_FONT_SOURCE == USE_SDCARD_FONT || XBF_FONT_SOURCE == USE_FLASH_FILESYSTEM_FONT)
	
  fFont_result = f_mount(&fs[0],FONT_STORAGE_ROOT_DIR,1);
	if ( fFont_result == FR_OK ) 
  {
    fFont_result = f_open(&fFont_file , FONT_XINSONGTI_16_ADDR, FA_OPEN_EXISTING);
    if ( fFont_result == FR_OK ) 
    {
			f_close(&fFont_file);
      return 0;
    }
  }
  return 1;
  
#endif
}



 /**
  * @brief  ����XBF����
  * @param  ��
  * @retval ��
  */
short Creat_XBF_Font(void) 
{	
  static char fontInit=0;
  if(LanguageDeviceInit()) return 1;
  if(fontInit) return 0;
	//
	// ����12
	//
	GUI_XBF_CreateFont(&FONT_XINSONGTI_12,    					// GUI_FONT ����ṹ��ָ��
					   &XBF_XINSONGTI_12_Data,         						// GUI_XBF_DATA �ṹ��ָ��
					   GUI_XBF_TYPE_PROP_EXT, 										// ��������
					   _cb_FONT_XBF_GetData,  							// ��ȡ�������ݵĻص�����
					   (void *)&FONT_XINSONGTI_12_ADDR);        	// Ҫ������ص��������Զ�������ָ�룬�˴������ֿ�ĵ�ַ
	//
	// ����16
	//
	GUI_XBF_CreateFont(&FONT_XINSONGTI_16,    					// GUI_FONT ����ṹ��ָ��
					   &XBF_XINSONGTI_16_Data,         						// GUI_XBF_DATA �ṹ��ָ��
					   GUI_XBF_TYPE_PROP_EXT, 										// ��������
					   _cb_FONT_XBF_GetData,  							// ��ȡ�������ݵĻص�����
					   (void *)&FONT_XINSONGTI_16_ADDR);        	// Ҫ������ص��������Զ�������ָ�룬�˴������ֿ�ĵ�ַ
	//
	// ����24
	//
	GUI_XBF_CreateFont(&FONT_XINSONGTI_24,    				// GUI_FONT ����ṹ��ָ��
					 &XBF_XINSONGTI_24_Data,         						// GUI_XBF_DATA �ṹ��ָ��
					 GUI_XBF_TYPE_PROP_EXT, 										// ��������
					 _cb_FONT_XBF_GetData,  							// ��ȡ�������ݵĻص�����
					 (void *)&FONT_XINSONGTI_24_ADDR);        	// Ҫ������ص��������Զ�������ָ�룬�˴������ֿ�ĵ�ַ
  fontInit=1;
  return 0;
}


 /**
  * @brief  GBKתUTF8
  * @param  src��������ַ�����GBK��ʽ��
  * @param  str��������ַ�����UTF8��ʽ��
  * @retval ��
  */
short GBK2UTF8(char *src, char *str)
{
	uint32_t i=0,num=0;
	uint16_t gbkdata=0;
	uint16_t UCbuffer[1024]={0};
	for(i=0,num=0;src[i]!='\0';num++)
	{
		if((uint8_t)src[i]>0x80)
		{
			gbkdata=src[i+1]+src[i]*256;
			UCbuffer[num]=ff_convert(gbkdata,1);
			i+=2;
		}
		else
		{
			UCbuffer[num]=0x00ff&src[i];
			i+=1;
		}
	}
	UCbuffer[num]='\0';
	GUI_UC_ConvertUC2UTF8(UCbuffer,2*num+2,str,num*3);
  num*=3;
	str[num]=0;
  return num;
}

/*************************************************************************************************
* ��UTF8����ת����Unicode��UCS-2LE������  �͵�ַ���λ�ֽ�
* ������
*    char* pInput     �����ַ���
*    char*pOutput   ����ַ���
* ����ֵ��ת�����Unicode�ַ������ֽ�������������򷵻�-1
**************************************************************************************************/
//utf8תunicode
int Utf8ToUnicode(char* pInput, char* pOutput)
{
	int outputSize = 0; //��¼ת�����Unicode�ַ������ֽ���
 
	while (*pInput)
	{
		if (*pInput > 0x00 && *pInput <= 0x7F) //�����ֽ�UTF8�ַ���Ӣ����ĸ�����֣�
		{
			*pOutput = *pInput;
			 pOutput++;
			*pOutput = 0; //С�˷���ʾ���ڸߵ�ַ�0
		}
		else if (((*pInput) & 0xE0) == 0xC0) //����˫�ֽ�UTF8�ַ�
		{
			char high = *pInput;
			pInput++;
			char low = *pInput;
			if ((low & 0xC0) != 0x80)  //����Ƿ�Ϊ�Ϸ���UTF8�ַ���ʾ
			{
				return -1; //��������򱨴�
			}
 
			*pOutput = (high << 6) + (low & 0x3F);
			pOutput++;
			*pOutput = (high >> 2) & 0x07;
		}
		else if (((*pInput) & 0xF0) == 0xE0) //�������ֽ�UTF8�ַ�
		{
			char high = *pInput;
			pInput++;
			char middle = *pInput;
			pInput++;
			char low = *pInput;
			if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80))
			{
				return -1;
			}
			*pOutput = (middle << 6) + (low & 0x3F);//ȡ��middle�ĵ���λ��low�ĵ�6λ����ϳ�unicode�ַ��ĵ�8λ
			pOutput++;
			*pOutput = (high << 4) + ((middle >> 2) & 0x0F); //ȡ��high�ĵ���λ��middle���м���λ����ϳ�unicode�ַ��ĸ�8λ
		}
		else //���������ֽ�����UTF8�ַ������д���
		{
			return -1;
		}
		pInput ++;//������һ��utf8�ַ�
		pOutput ++;
		outputSize += 2;
	}
	//unicode�ַ������棬������\0
	*pOutput = 0;
	 pOutput++;
	*pOutput = 0;
	return outputSize;
}

/*********************************************END OF FILE**********************/
