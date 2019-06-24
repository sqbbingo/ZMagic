/*********************************************************************************************
* �ļ���lis3dh.c
* ���ߣ�Zhoucj 2019.03.07
* ˵����lis3dh��������,��������,�Ʋ�,����
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/

#include <math.h>
#include "arm_math.h"
#include "arm_const_structs.h"
#include "fml_imu/lis3dh.h"
#include "soft_iic.h"

// ����ʹ�� �Ʋ�ʹ�� ����ʹ�� ����״̬ ����״̬ �Ʋ��� �����㷨������� ����ʱ��
lis3dh lis3dhStruct = {1, 1, 0, 0, 0, 0, 10, 60};

int lis3dh_readReg(unsigned char regAddr)
{
  int data;
  I2C_Start();
  I2C_WriteByte(LIS3DH_ADDR & 0xFE);
  if(I2C_WaitAck()) return -1;
  I2C_WriteByte(regAddr);
  if(I2C_WaitAck()) return -1;
  I2C_Start();
  I2C_WriteByte(LIS3DH_ADDR | 0x01);
  if(I2C_WaitAck()) return -1;
  data = I2C_ReadByte();
  I2C_Stop();
  return data;
}

int lis3dh_writeReg(unsigned char regAddr, unsigned char data)
{
  I2C_Start();
  I2C_WriteByte(LIS3DH_ADDR & 0xFE);
  if(I2C_WaitAck()) return -1;
  I2C_WriteByte(regAddr);
  if(I2C_WaitAck()) return -1;
  I2C_WriteByte(data);
  if(I2C_WaitAck()) return -1;
  I2C_Stop();
  return 0;
}

int lis3dh_init(void)
{
  I2C_GPIOInit();
  if(LIS3DH_ID != lis3dh_readReg(LIS3DH_IDADDR))                // ��ȡID
    return -1;
  
  if(lis3dh_writeReg(LIS3DH_CTRL_REG1, 0x97) < 0)               // ��������1.25KHz,����XYZ��
    return -1;
  
  if(lis3dh_writeReg(LIS3DH_CTRL_REG4, 0x10) < 0)               
    return -1;
  return 0;
}

void get_lis3dhInfo(float *accX, float *accY, float *accZ)
{
  char accXL, accXH, accYL, accYH, accZL, accZH;
  accXL = lis3dh_readReg(LIS3DH_OUT_X_L);
  accXH = lis3dh_readReg(LIS3DH_OUT_X_H);
  if(accXH & 0x80)
    *accX = (float)(((int)accXH << 4 | (int)accXL >> 4)-4096)/2048*9.8*4;
  else
    *accX = (float)((int)accXH << 4 | (int)accXL >> 4)/2048*9.8*4;
  accYL = lis3dh_readReg(LIS3DH_OUT_Y_L);
  accYH = lis3dh_readReg(LIS3DH_OUT_Y_H);
  if(accYH & 0x80)
    *accY = (float)(((int)accYH << 4 | (int)accYL >> 4)-4096)/2048*9.8*4;
  else
    *accY = (float)((int)accYH << 4 | (int)accYL >> 4)/2048*9.8*4;
  accZL = lis3dh_readReg(LIS3DH_OUT_Z_L);
  accZH = lis3dh_readReg(LIS3DH_OUT_Z_H);
  if(accZH & 0x80)
    *accZ = (float)(((int)accZH << 4 | (int)accZL >> 4)-4096)/2048*9.8*4;
  else
    *accZ = (float)((int)accZH << 4 | (int)accZL >> 4)/2048*9.8*4;
}

/*********************************************************************************************
* ���ƣ�fallDect()
* ���ܣ�������⴦���㷨
* ������x��y��z - �ֱ�Ϊ���ᴫ������x��y��zֵ
* ���أ�int - ����״̬
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int lis3dh_tumble(float x, float y, float z)
{
  float squareX = x * x;
  float squareY = y * y;
  float squareZ = z * z;
  float value;
  
  value = sqrt(squareX + squareY + squareZ);
  if (value < 5)
    return 1;
  return 0;
}


/*********************************************************************************************
* ���ƣ�int stepcounting(float32_t* test_f32)
* ���ܣ��������㺯��
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int stepcounting(float32_t* test_f32)
{
  uint32_t ifftFlag = 0;                                        //����Ҷ��任��־λ
  uint32_t doBitReverse = 1;                                    //��ת��־λ
  float32_t testOutput[N/2];                                    //�������
  uint32_t i; 
  arm_cfft_f32(&arm_cfft_sR_f32_len64, test_f32, ifftFlag, doBitReverse);//����Ҷ�任
  arm_cmplx_mag_f32(test_f32, testOutput, N/2);  
  float max = 0; 
  uint32_t mi = 0;  
  for (i=0; i<N/2; i++) {
    float a = testOutput[i];
    if (i == 0) a = testOutput[i]/(N);
    else a = testOutput[i]/(N/2);
    if (i != 0 && a > max && i*F_P <= 5.4f) {
        mi = i;
        max = a;
    }
  }
  if (max > 1.5) {
      int sc = 0;
      sc = (int)(mi * F_P * (1.0/Fs)*N);
      if (sc >= 3 && sc < 30) {
       return sc;
      }
  }
  return 0;
}

/*********************************************************************************************
* ���ƣ�lis3dh_step()
* ���ܣ��Ʋ������㷨
* ������x��y��z - �ֱ�Ϊ���ᴫ������x��y��zֵ
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void lis3dh_step(float x, float y, float z)
{
  static unsigned char tick = 0;
  static float acc_input[64*2];
  static unsigned short acc_len = 0;
  static unsigned char step_cnt = 0;
  float a = sqrt(x*x + y*y + z*z);
  acc_input[acc_len * 2] = a;
  acc_input[acc_len*2+1] = 0;
  acc_len++;
  if(acc_len == 64)
    acc_len = 0;
  if(acc_len == 0)
    step_cnt += stepcounting(acc_input);
  tick++;
  if(tick == lis3dhStruct.SedentaryInterval)
  {
    tick = 0;
    lis3dhStruct.stepCount += step_cnt;
    step_cnt = 0;
  }
}

/*********************************************************************************************
* ���ƣ�lis3dh_SedentaryStatus()
* ���ܣ����������㷨
* ������x��y��z - �ֱ�Ϊ���ᴫ������x��y��zֵ
* ���أ�int - ����״̬
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int lis3dh_sedentary(float x, float y, float z)
{
  static float lastX = 0, lastY = 0, lastZ = 9;
  static unsigned char count = 0;
  static unsigned short time = 0;
  if((x - 2 > lastX || x + 2 < lastX) 
     || (y - 2 > lastY || y + 2 < lastY) 
       || (z - 2 > lastZ || z + 2 < lastZ))
  {
    time = 0;
    count = 0;
  }
  else
    count++;
  if(count > lis3dhStruct.SedentaryInterval)
  {
	count= 0;
    time++;
  }
  if(time >= lis3dhStruct.SedentaryTime)
    return 1;
  else
    return 0;
}

/*********************************************************************************************
* ���ƣ�run_lis3dh_arithmetic()
* ���ܣ�����lis3dh�㷨
* ��������
* ���أ�int - 1/-1 ִ�гɹ�/δ�����㷨ʹ��
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int run_lis3dh_arithmetic(void)
{
  if(lis3dhStruct.tumbleEnableStatus || lis3dhStruct.stepEnableStatus 
     || lis3dhStruct.SedentaryEnableStatus)
  {
    float accX, accY, accZ;
    unsigned char runStatus = 0;
    get_lis3dhInfo(&accX, &accY, &accZ);
    if(lis3dhStruct.tumbleEnableStatus)
    {
      if(!(accX == 0 && accY == 0 && accZ == 0))
        lis3dhStruct.tumbleStatus = lis3dh_tumble(accX, accY, accZ);
      else
        lis3dhStruct.tumbleStatus = 0;
      runStatus |= 0x01;
    }
    if(lis3dhStruct.stepEnableStatus)
    {
      lis3dh_step(accX, accY, accZ);
      runStatus |= 0x02;
    }
    if(lis3dhStruct.SedentaryEnableStatus)
    {
      lis3dhStruct.SedentaryStatus = lis3dh_sedentary(accX, accY, accZ);
      runStatus |= 0x04;
    }
    return runStatus;
  }
  else
    return -1;
}

// �����㷨ʹ��
void set_lis3dh_enableStatus(unsigned char cmd)
{
  if((cmd & 0x01) == 0x01)
    lis3dhStruct.tumbleEnableStatus = 1;
  else
    lis3dhStruct.tumbleEnableStatus = 0;
  if((cmd & 0x02) == 0x02)
    lis3dhStruct.stepEnableStatus = 1;
  else
    lis3dhStruct.stepEnableStatus = 0;
  if((cmd & 0x04) == 0x04)
    lis3dhStruct.SedentaryEnableStatus = 1;
  else
    lis3dhStruct.SedentaryEnableStatus = 0;
}

// ��ȡ�Ʋ��㷨ʹ��
unsigned char get_lis3dh_enableStatus(void)
{
  unsigned char status = 0;
  if(lis3dhStruct.tumbleEnableStatus)
    status |= 0x01;
  if(lis3dhStruct.stepEnableStatus)
    status |= 0x02;
  if(lis3dhStruct.SedentaryEnableStatus)
    status |= 0x04;
  return status;
}

// ��ȡ��ǰ�Ʋ���
int get_lis3dh_stepCount(void)
{
  if(lis3dhStruct.stepEnableStatus)
    return lis3dhStruct.stepCount;
  else
    return -1;
}

// ��ȡ����״̬
int get_lis3dh_tumbleStatus(void)
{
  if(lis3dhStruct.tumbleEnableStatus)
    return lis3dhStruct.tumbleStatus;
  else
    return -1;
}

// ��ȡ����״̬
int get_lis3dh_SedentaryStatus(void)
{
  if(lis3dhStruct.SedentaryEnableStatus)
    return lis3dhStruct.SedentaryStatus;
  else
    return -1;
}

// ���þ���ʱ��ϵ��
// �����㷨���м��(��λ��ms)������ʱ��(��λ��s)
void set_lis3dh_SedentaryTime(unsigned short interval, unsigned short time)
{
  lis3dhStruct.SedentaryInterval = 1000 / interval;
  lis3dhStruct.SedentaryTime = time;
}

// ��ռƲ���
void del_lis3dh_stepCount(void)
{
  lis3dhStruct.stepCount = 0;
}
