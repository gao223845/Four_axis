#include "iic_mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "beep.h"
#include "usart.h"
#include "delay.h"
#include "Struct_all.h"



/**  
  *  ���ܣ�MPU��ʼ����װ
  *  ��ڲ�����
  *  ����ֵ��
  */

void MPU6050_Init(void)
{
	MPU_Init();//�Զ���MPU��ʼ��
	while(mpu_dmp_init())
		BEEP =! BEEP;//dmp���ʼ��		
}

/**  
  *  ���ܣ�MPU��ȡ����
  *  ��ڲ������ṹ�� &out_angle,&acc,&gyro
  *  ����ֵ��
  */
void MPU_GetData(void)
{
	if(mpu_dmp_get_data()==0)
	{	
		Get_Radian(&gyro,&SI_gyro);
	}
}
/**  
  *  ���ܣ�MPU gyro��ԭʼ����ת��Ϊ����
  *  ��ڲ������ṹ�� &out_angle,&acc,&gyro
  *  ����ֵ��
  */
void Get_Radian(struct _gyro *Gyro_in,struct _SI_float *Gyro_out)
{
	Gyro_out->x = (float)(Gyro_in->x * RawData_to_Radian);
	Gyro_out->y = (float)(Gyro_in->y * RawData_to_Radian);
	Gyro_out->z = (float)(Gyro_in->z * RawData_to_Radian);
}

/**  
  *  ���ܣ�IIC��ʱ����
  *  ��ڲ�������
  *  ����ֵ����
  */
void MPU_IIC_Delay(void)
{
	delay_us(2);
}
/**  
  *  ���ܣ���ģ��IIC���ų�ʼ�� PB10.SCL  PB11.SDA
  *  ��ڲ�������
  *  ����ֵ����
  */
void MPU_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);
	
}
/**  
  *  ���ܣ�IIC��ʼ�ź� SCL���ָߵ�ƽ��SDA�Ӹߵ�ƽ���䵽�͵�ƽ
  *  ��ڲ�������
  *  ����ֵ����
  */
void MPU_IIC_Start(void)
{
	MPU_SDA_OUT();
	MPU_IIC_SDA=1;
	MPU_IIC_SCL=1;
	MPU_IIC_Delay();
	MPU_IIC_SDA=0;
	MPU_IIC_Delay();
	MPU_IIC_SCL=0;//ǯ��IIC���ߣ�׼�����ͻ��������	
}
/**  
  *  ���ܣ�IIC�����ź� SCL���ָߵ�ƽ��SDA�ӵ͵�ƽ���䵽�ߵ�ƽ
  *  ��ڲ�������
  *  ����ֵ����
  */	
void MPU_IIC_Stop(void)
{
	MPU_SDA_OUT();
	MPU_IIC_SCL=0;
	MPU_IIC_SDA=0;
	MPU_IIC_Delay();
	MPU_IIC_SCL=1;
	MPU_IIC_SDA=1;
	MPU_IIC_Delay();
}
/**  
  *  ���ܣ��ȴ�Ӧ���ź�ACK
  *  ��ڲ�������
  *  ����ֵ��0������Ӧ��ɹ���1������Ӧ��ʧ��
  */
uint8_t MPU_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime = 0;
	MPU_SDA_IN();
	MPU_IIC_SDA=1;MPU_IIC_Delay();
	MPU_IIC_SCL=1;MPU_IIC_Delay();
	while(MPU_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
	MPU_IIC_SCL=0;
	return 0;
}
/**  
  *  ���ܣ�����ACKӦ��
  *  ��ڲ�������
  *  ����ֵ����
  */
void MPU_IIC_Ack(void)
{
	MPU_IIC_SCL=0;
	MPU_SDA_OUT();
	MPU_IIC_SDA=0;
	MPU_IIC_Delay();
	MPU_IIC_SCL=1;
	MPU_IIC_Delay();
	MPU_IIC_SCL=0;
}
/**  
  *  ���ܣ�������ACKӦ��
  *  ��ڲ�������
  *  ����ֵ����
  */
void MPU_IIC_NAck(void)
{
	MPU_IIC_SCL=0;
	MPU_SDA_OUT();
	MPU_IIC_SDA=1;
	MPU_IIC_Delay();
	MPU_IIC_SCL=1;
	MPU_IIC_Delay();
	MPU_IIC_SCL=0;
}
/**  
  *  ���ܣ�IIC����һ���ֽڣ�8 bit��
  *  ��ڲ�������
  *  ����ֵ�����شӻ�����Ӧ��0����Ӧ��1����Ӧ��
  */
void MPU_IIC_Send_Byte(uint8_t txd)
{
	uint8_t t;
	MPU_SDA_OUT();
	MPU_IIC_SCL=0;//����IIC����ʱ�ӿ�ʼ���ݴ���
	for(t=0;t<8;t++)
	{
		MPU_IIC_SDA=(txd&0x80)>>7;
		txd<<=1;
		MPU_IIC_SCL=1;
		MPU_IIC_Delay();
		MPU_IIC_SCL=0;
		MPU_IIC_Delay();
	}
}
/**  
  *  ���ܣ�IIC��ȡһ���ֽڣ�8 bit��
  *  ��ڲ�����ack��Ӧ��
  *  ����ֵ��һ���ֽڣ�8 bit��
  */
uint8_t MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MPU_SDA_IN();
	for(i=0;i<8;i++)
	{
		MPU_IIC_SCL=0;
		MPU_IIC_Delay();
		MPU_IIC_SCL=1;
		receive<<=1;
		if(MPU_READ_SDA)
			receive++;
		MPU_IIC_Delay();
	}
	if(!ack)
		MPU_IIC_NAck();//����NACK
	else
		MPU_IIC_Ack();//����ACK
	return receive;
}
/**  
  *  ���ܣ�IICдһ���ֽ�
  *  ��ڲ�����reg���Ĵ�����ַ��data������
  *  ����ֵ��0��������1������
  */
uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data)
{
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//ADDR����һλ+д����
	if(MPU_IIC_Wait_Ack())//�ȴ�Ӧ��
	{
		MPU_IIC_Stop();
		return 1;
	}
	MPU_IIC_Send_Byte(reg);
	MPU_IIC_Wait_Ack();
	MPU_IIC_Send_Byte(data);
	if(MPU_IIC_Wait_Ack())
	{
		MPU_IIC_Stop();
		return 1;
	}
	MPU_IIC_Stop();
	return 0;
}
  
/**  
  *  ���ܣ�IIC��һ���ֽ�
  *  ��ڲ�����reg���Ĵ�����ַ
  *  ����ֵ������
  */
uint8_t MPU_Read_Byte(uint8_t reg)
{
	uint8_t res;
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//�����ַ+д����
	MPU_IIC_Wait_Ack();
	MPU_IIC_Send_Byte(reg);//�Ĵ�����ַ
	MPU_IIC_Wait_Ack();
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);
	MPU_IIC_Wait_Ack();
	res = MPU_IIC_Read_Byte(0);
	MPU_IIC_Stop();
	return res;
}
/**  
  *  ���ܣ�IIC����д
  *  ��ڲ�����addr�������ַ��reg���Ĵ�����ַ��len��д�볤�ȣ�buf������
  *  ����ֵ��0��������1������
  */
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	uint8_t i;
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|0);
	if(MPU_IIC_Wait_Ack())
	{
		MPU_IIC_Stop();
		return 1;
	}
	MPU_IIC_Send_Byte(reg);
	MPU_IIC_Wait_Ack();
	for(i=0;i<len;i++)
	{
		MPU_IIC_Send_Byte(buf[i]);
		if(MPU_IIC_Wait_Ack())
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
	MPU_IIC_Stop();
	return 0;
}
/**  
  *  ���ܣ�IIC������
  *  ��ڲ�����addr�������ַ��reg���Ĵ�����ַ��len����ȡ���ȣ�buf����������
  *  ����ֵ��0,������1������
  */
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|0);
	if(MPU_IIC_Wait_Ack())
	{
		MPU_IIC_Stop();
		return 1;
	}
	MPU_IIC_Send_Byte(reg);
	MPU_IIC_Wait_Ack();
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|1);
	MPU_IIC_Wait_Ack();
	while(len)
	{
		if(len==1)
			*buf=MPU_IIC_Read_Byte(0);
		else		
			*buf=MPU_IIC_Read_Byte(1);		
		len--;
		buf++;
	}
	MPU_IIC_Stop();
	return 0;
}
/**  
  *  ���ܣ���ʼ��MPU6050
  *  ��ڲ�������
  *  ����ֵ��0���ɹ�������������
  */
uint8_t MPU_Init(void)
{
//	uint8_t res;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//��ֹJTAG��ʹPA15��Ϊ��ͨIO
	
	MPU_AD0_CTRL = 0;//����MPU6050��AD0Ϊ�͵�ƽ���ӻ���ַ=0x68��
					 //					 �ߵ�ƽ��		  0x69��
//	MPU_IIC_Init();
//	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0x80);//��λ�ӻ�
//	delay_ms(100);
//	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0x00);//���Ѵӻ�
//	MPU_Set_Gyro_Fsr(3);//�����Ǵ����� +-2000dps
//	MPU_Set_Accel_Fsr(0);//���ٶȴ����� +-2g
//	MPU_Set_Rate(50);
//	MPU_Write_Byte(MPU_INT_EN_REG,0x00);//�ر������ж�
//	MPU_Write_Byte(MPU_USER_CTRL_REG,0x00);//IIC��ģʽ�ر�
//	MPU_Write_Byte(MPU_FIFO_EN_REG,0x00);//�ر�FIFO
//	MPU_Write_Byte(MPU_INTBP_CFG_REG,0x80);//INT�͵�ƽ��Ч
//	res = MPU_Read_Byte(MPU_DEVICE_ID_REG);
//	if(res == MPU_ADDR)
//	{
//		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0x01);//����CLKSEL PLL X��Ϊ�ο�
//		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0x00);//���ٶȼƺ�������ʹ��
//		MPU_Set_Rate(50);
//	}
//	else
//		return 1;
	return 0;
}
/**  
  *  ���ܣ����������Ǵ����������̷�Χ
  *  ��ڲ�����fsr,0,+-250dps
  *				   1,+-500dps
  *				   2,+-1000dps
  *				   3,+-2000dps	
  *  ����ֵ��0,�ɹ���1��ʧ��
  */
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);
}
/**  
  *  ���ܣ����ü��ٶȴ����������̷�Χ
  *  ��ڲ�����fsr��0,+-2g
  *				    1,+-4g
  *				    2,+-8g
  *				    3,+-16g	
  *  ����ֵ��0,�ɹ���1��ʧ��
  */
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);
}
/**  
  *  ���ܣ��������ֵ�ͨ�˲���
  *  ��ڲ�����lpf����ͨ�˲���Ƶ�ʣ�Hz��
  *  ����ֵ��0,�ɹ���1��ʧ��
  */
uint8_t MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6;
	return MPU_Write_Byte(MPU_CFG_REG,data);
}
/**  
  *  ���ܣ����ò����ʣ�Fs=1KHz��
  *  ��ڲ�����rate=[4,1000]Hz
  *  ����ֵ��0,���óɹ���1������ʧ��
  */
uint8_t MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);
	return MPU_Set_LPF(rate/2);
}
/**  
  *  ���ܣ���ȡMPU�ڲ��¶�
  *  ��ڲ�������
  *  ����ֵ���¶�*100
  */
short MPU_Get_Temperature(void)
{
	uint8_t buf[2];
	short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf);
	raw=((uint16_t)buf[0]<<8)|buf[1];
	temp = 36.53+((double)raw)/340;
	return temp*100;
}
/**  
  *  ���ܣ����������ԭʼֵ
  *  ��ڲ�����gx,gy,gz,������XYZ��ԭʼ���ݣ������ţ�
  *  ����ֵ��0���ɹ���1��ʧ��
  */
uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
	uint8_t buf[6],res;
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res == 0)
	{
		*gx=((uint16_t)buf[0]<<8)|buf[1];
		*gy=((uint16_t)buf[2]<<8)|buf[3];
		*gz=((uint16_t)buf[4]<<8)|buf[5];
	}
	return res;
}
/**  
  *  ���ܣ���ü��ٶ�ԭʼֵ
  *  ��ڲ�����ax,ay,az,���ٶȼ�XYZ��ԭʼ���ݣ���
  *  ����ֵ��0��������1������
  */
uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
	uint8_t buf[6],res;
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res ==0)
	{
		*ax=((uint16_t)buf[0]<<8)|buf[1];
		*ay=((uint16_t)buf[2]<<8)|buf[3];
		*az=((uint16_t)buf[4]<<8)|buf[5];
	}
	return res;
}

