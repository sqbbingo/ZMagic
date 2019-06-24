/*********************************************************************************************
* �ļ���ft6x36.c
* ���ߣ�
* ˵������������
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include "fml_lcd/ft6x36.h"
#include "soft_iic.h"
#include "exti.h"

#if 1
#define UART_DEBUG      printf
#else
#define UART_DEBUG
#endif


void (*TouchIrq)(void) = NULL;
void TouchIrqSet(void (*func)(void))
{
  TouchIrq = func;
}

void ft6x36_touch_irq(void);

TOUCH_Dev_t TouchDev={
  .direction = 1,
  .wide = LCD_HIGH,
  .high = LCD_WIDE,
  .id = 0,
};


/*********************************************************************************************
* ���ƣ�ft6x36_read_reg()
* ���ܣ�ft6x36������
* ������r -- ���͵�ָ��
*       v -- ���յ�������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int ft6x36_read_reg(char r)
{
  int v;
  
  I2C_Start();
  I2C_WriteByte((FT6X36_IIC_ADDR<<1)|0x00);
  if (I2C_WaitAck()) 
  {
    I2C_Stop();
    goto error;
  }
  I2C_WriteByte(r);
  if (I2C_WaitAck()) 
  {
    I2C_Stop();
    goto error;
  }
  I2C_Start();
  I2C_WriteByte((FT6X36_IIC_ADDR<<1)|0x01);
  if (I2C_WaitAck()) 
  {
    I2C_Stop();
    goto error;
  }
  v = I2C_ReadByte();
  I2C_NoAck();
  I2C_Stop();
  
  return v&0xff;
  
error:
  return -1;
}

/*********************************************************************************************
* ���ƣ�ft6x36_write_reg()
* ���ܣ�ft6x36д����
* ������r -- ���͵�ָ��
*       v -- ���͵�ָ��
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int ft6x36_write_reg(char r, char v) 
{
  I2C_Start();
  I2C_WriteByte((FT6X36_IIC_ADDR<<1)|0x00);
  if (I2C_WaitAck()) 
  {
    I2C_Stop();
    return -1;
  }
  I2C_WriteByte(r);
  if (I2C_WaitAck()) 
  {
    I2C_Stop();
    return -1;
  }
  I2C_WriteByte(v);
  if (I2C_WaitAck()) 
  {
    I2C_Stop();
    return -1;
  }
  I2C_Stop();
  return 0;
}


/*********************************************************************************************
* ���ƣ�ft6x36_init()
* ���ܣ�ft6x36��ʼ��
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int ft6x36_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  //reset io
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                 //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;            //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                  //����
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);                        //��ʼ��
  GPIO_SetBits(GPIOC,GPIO_Pin_5);
  
  //irq io
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                //����
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                  //������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;            //100MHz
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOE, &GPIO_InitStructure);                        //��ʼ��
  
  Exti1_Init(EXTI_PortSourceGPIOE);//EXTI init
  Exti1IrqCall_Set(ft6x36_touch_irq);
  
  I2C_GPIOInit();
  
  int r;
  r = ft6x36_read_reg(0xA8);
  if (r < 0)
  {
    UART_DEBUG("error: ft6x36 read reg 0\r\n");
    return 1;
  }
  if (!(r == 0x11 || r == 0x23))
  {
    UART_DEBUG("error: can't find ic ft6236\r\n");
    return 2;
  }  
  ft6x36_write_reg(0, 0);
  ft6x36_write_reg(0x80, 22);
  r = ft6x36_read_reg(0x80); 
  ft6x36_write_reg(0x88, 12);
  return 0;
}

void ft6x36_TouchScan()
{
  uint8_t i=0;
  int status = ft6x36_read_reg(2);
  if(status > TOUCH_NUM) status = TOUCH_NUM;
  if(status > 0)
  {
    TouchDev.status = status;
    
    for(i=0;i<status;i++)
    {
      TouchDev.x[i]=(ft6x36_read_reg(3+(6*i))&0x0f);
      TouchDev.x[i]<<=8;
      TouchDev.x[i]+=ft6x36_read_reg(4+(6*i));
      
      TouchDev.y[i]=(ft6x36_read_reg(5+(6*i))&0x0f);
      TouchDev.y[i]<<=8;
      TouchDev.y[i]+=ft6x36_read_reg(6+(6*i));
    }
    
    if(TouchDev.direction)
    {
      uint16_t temp=0;
      for(i=0;i<TOUCH_NUM;i++)
      {
        temp = TouchDev.y[i];
        TouchDev.y[i] = TouchDev.x[i];
        TouchDev.x[i] = TouchDev.wide-temp;
      }
    }      
  }
  else
  {
    for(i=0;i<TOUCH_NUM;i++)
    {
      TouchDev.x[i] = 0xffff;
      TouchDev.y[i] = 0xffff;
    }
  }
}


void ft6x36_touch_irq(void)
{
  if(TouchIrq != NULL)
    TouchIrq();
}
