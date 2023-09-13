/*********************************************************************************************************
//������
//���ߣ����ƮԾ    ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************************************/

#ifndef __24XX128_H__
#define __24XX128_H__
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include "include.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/
#define I2C_PWR_PORT        GPIOC                                 //eeprom ��Դʹ�� �ߵ�ƽ�� �͵�ƽ��
#define I2C_PWR_Pin         GPIO_Pin_7
#define I2C_PWR_Mode        GPIO_Mode_Out_PP_Low_Slow
#define I2C_PWR_SET         GPIO_SetBits(I2C_PWR_PORT,I2C_PWR_Pin) 
#define I2C_PWR_RESET       GPIO_ResetBits(I2C_PWR_PORT,I2C_PWR_Pin)    

#define I2C_SCL_PORT        GPIOC                                 //eeprom SCL
#define I2C_SCL_Pin         GPIO_Pin_1
#define I2C_SCL_Mode        GPIO_Mode_Out_OD_Low_Slow
#define I2C_SCL_SET         GPIO_SetBits(I2C_SCL_PORT,I2C_SCL_Pin) 
#define I2C_SCL_RESET       GPIO_ResetBits(I2C_SCL_PORT,I2C_SCL_Pin)    

#define I2C_SDA_PORT        GPIOC                                 //eeprom SDA
#define I2C_SDA_Pin         GPIO_Pin_0
#define I2C_SDA_Mode        GPIO_Mode_Out_OD_Low_Slow
#define I2C_SDA_SET         GPIO_SetBits(I2C_SDA_PORT,I2C_SDA_Pin) 
#define I2C_SDA_RESET       GPIO_ResetBits(I2C_SDA_PORT,I2C_SDA_Pin)   
#define I2C_SDA             GPIO_ReadInputDataBit (I2C_SDA_PORT,I2C_SDA_Pin) 
#define I2C_SDA_InMode      I2C_SDA_PORT->DDR &= ~I2C_SDA_Pin
#define I2C_SDA_OutMode     I2C_SDA_PORT->DDR |= I2C_SDA_Pin
/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/
/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/
/*********************************************************************************************************
����������
*********************************************************************************************************/
void delay5us(void);
void delay5ms(void);
void I2C_Start(void);			//��ʼ�ź�
void I2C_Stop(void);			 //ֹͣ�ź�
void I2C_ACK(void);      //ACK�ź�
void I2C_NoACK(void);    //��Ӧ��
unsigned char I2C_Wait_Ack(void);
void I2C_Power_On(void);
void I2C_Power_Off(void);
void I2C_Send_Byte(unsigned char txd);
unsigned char I2C_Read_Byte(unsigned char ack);
void _24XX128_ReadBytes(unsigned int Address,unsigned char *Data,unsigned int num);		       //����ַ��������
void _24XX128_WriteBytes(unsigned int Address,unsigned char *Data,unsigned int num);		       //����ַд������
/********************************************************************************************************/
#endif

/******************************************END********************************************************/
