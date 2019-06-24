#ifndef __SCCB_H
#define __SCCB_H
#include "sys.h"


//IO��������
#define SCCB_SDA_IN()  {GPIOD->MODER&=~(3<<(13*2));GPIOD->MODER|=0<<13*2;}	//PD13 ����
#define SCCB_SDA_OUT() {GPIOD->MODER&=~(3<<(13*2));GPIOD->MODER|=1<<13*2;} 	//PD13 ���


//IO��������	 
#define SCCB_SCL    		PDout(11)	 	//SCL
#define SCCB_SDA    		PDout(13) 		//SDA	 

#define SCCB_READ_SDA   PDin(13)  		//����SDA    
#define SCCB_ID   			0X60  			//OV2640��ID

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);
#endif













