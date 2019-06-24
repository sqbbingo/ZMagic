/*********************************************************************************************
* �ļ���fml_key.c
* ���ߣ�fuyou
* ˵���������Ĺ���ģ�������ļ�
* ���ܣ��ṩ���������ӿ�
* �޸ģ�
*********************************************************************************************/
/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include "fml_key.h"

/*********************************************************************************************
* ���ƣ�key_init
* ���ܣ������ܽų�ʼ��
* ��������
* ���أ���
* �޸ģ���
*********************************************************************************************/
void key_GpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;                          //����һ��GPIO_InitTypeDef���͵Ľṹ��
    
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                //�������ŵ��������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                  //��������ģʽΪ����ģʽ
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                  //��������Ϊ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;            //������������Ϊ100MHz
    
    RCC_AHB1PeriphClockCmd( K1_CLK, ENABLE);                    //����KEY��ص�GPIO����ʱ��
    GPIO_InitStructure.GPIO_Pin = K1_PIN;                       //ѡ��Ҫ���Ƶ�GPIO����
    GPIO_Init(K1_PORT, &GPIO_InitStructure);                    //��ʼ��GPIO����
    
    RCC_AHB1PeriphClockCmd( K2_CLK, ENABLE);                    //����KEY��ص�GPIO����ʱ��
    GPIO_InitStructure.GPIO_Pin = K2_PIN;                       //ѡ��Ҫ���Ƶ�GPIO����
    GPIO_Init(K2_PORT, &GPIO_InitStructure);                    //��ʼ��GPIO����
    
    RCC_AHB1PeriphClockCmd( K3_CLK, ENABLE);                    //����KEY��ص�GPIO����ʱ��
    GPIO_InitStructure.GPIO_Pin = K3_PIN;                       //ѡ��Ҫ���Ƶ�GPIO����
    GPIO_Init(K3_PORT, &GPIO_InitStructure);                    //��ʼ��GPIO����
}


/*********************************************************************************************
* ���ƣ�key_get_status
* ���ܣ������ܽų�ʼ��
* ��������
* ���أ�key_status
* �޸ģ�
*********************************************************************************************/
uint8_t key_GetStatus(void)
{
    uint8_t key_status = 0;
    if(GPIO_ReadInputDataBit(K1_PORT,K1_PIN) == 0)                //�ж�PB12���ŵ�ƽ״̬
        key_status |= K1_PRESSED;                                   //�͵�ƽkey_status bit0λ��1
    if(GPIO_ReadInputDataBit(K2_PORT,K2_PIN) == 0)                //�ж�PB13���ŵ�ƽ״̬
        key_status |= K2_PRESSED;                                   //�͵�ƽkey_status bit1λ��1
    if(GPIO_ReadInputDataBit(K3_PORT,K3_PIN) == 0)                //�ж�PB14���ŵ�ƽ״̬
        key_status |= K3_PRESSED;                                   //�͵�ƽkey_status bit2λ��1
    return key_status;
}

