/*********************************************************************************************
* �ļ���relay.c
* ���ߣ�
* ˵�����̵�����������
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/

/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include "relay.h"

static unsigned char relay_status = 0;

/*********************************************************************************************
* ���ƣ�relay_init()
* ���ܣ��̵�����������ʼ��
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void relay_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;                          //����һ��GPIO_InitTypeDef���͵Ľṹ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);         //�����̵�����ص�GPIO����ʱ��
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;      //ѡ��Ҫ���Ƶ�GPIO����
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                //�������ŵ��������Ϊ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                 //��������ģʽΪ���ģʽ
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;                //��������Ϊ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;             //������������Ϊ2MHz
    
    GPIO_Init(GPIOC, &GPIO_InitStructure);                        //��ʼ��GPIO����
    GPIO_ResetBits(GPIOC,GPIO_Pin_12 | GPIO_Pin_13);
}


unsigned char relay_GetStatus(void)
{
    return relay_status;
}


/*********************************************************************************************
* ���ƣ�void relay_control(unsigned char cmd)
* ���ܣ��̵�����������
* ��������������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void relay_control(unsigned char cmd)
{ 
    relay_status |= cmd;
    if(cmd & 0x01)
        GPIO_SetBits(GPIOC,GPIO_Pin_12);
    else
        GPIO_ResetBits(GPIOC,GPIO_Pin_12);
    if(cmd & 0x02)
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
    else
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}


void relay_on(unsigned int s)
{
    relay_status |= s;
    if (s & 0x01) 
    {
        GPIOC->BSRRL = GPIO_Pin_12;
    }
    if (s & 0x02) 
    {
        GPIOC->BSRRL = GPIO_Pin_13;
    }
}

void relay_off(unsigned int s)
{
    relay_status &= ~s;
    if (s & 0x01) 
    {
        GPIOC->BSRRH = GPIO_Pin_12;
    }
    if (s & 0x02) 
    {
        GPIOC->BSRRH = GPIO_Pin_13;
    }
}