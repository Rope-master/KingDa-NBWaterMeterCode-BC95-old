/*******************************************************************************
Copyright: 
File name: 24xx128.c
Description: ����EEPROM��غ���������
Author: ��ѧ��cy
Version: 
Date: 
History:       
*******************************************************************************/
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include "24xx128.h"
/*******************************************************************************
����������
*******************************************************************************/
/*******************************************************************************
��������������
*******************************************************************************/
/*******************************************************************************
�ⲿ����������
*******************************************************************************/
/*******************************************************************************
˽�б���������
*******************************************************************************/ 
/*******************************************************************************
���Ա���������
*******************************************************************************/
/*******************************************************************************
�ڲ�����������
*******************************************************************************/
/*******************************************************************************
���ܴ��붨����
*******************************************************************************/
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void I2C_Power_On(void)
{
  GPIO_Init(I2C_PWR_PORT, I2C_PWR_Pin , I2C_PWR_Mode);    //��Դ��������
  GPIO_Init(I2C_SCL_PORT, I2C_SCL_Pin , I2C_SCL_Mode);    //I2C ����
  GPIO_Init(I2C_SDA_PORT, I2C_SDA_Pin , I2C_SDA_Mode);    //I2C ����
  I2C_PWR_SET;
  delay5ms();
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void I2C_Power_Off(void)
{
  GPIO_Init(I2C_PWR_PORT, I2C_PWR_Pin , GPIO_Mode_Out_PP_Low_Slow);    //��Դ��������
  GPIO_Init(I2C_SCL_PORT, I2C_SCL_Pin , GPIO_Mode_Out_PP_Low_Slow);    //I2C ����
  GPIO_Init(I2C_SDA_PORT, I2C_SDA_Pin , GPIO_Mode_Out_PP_Low_Slow);    //I2C ����
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void I2C_Start(void)			//��ʼ�ź�
{
  I2C_SDA_OutMode;
  I2C_SDA_SET;
  I2C_SCL_SET;
  delay5us();
  I2C_SDA_RESET;
  delay5us();
  I2C_SCL_RESET;
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void I2C_Stop(void)			 //ֹͣ�ź�
{
  I2C_SDA_OutMode;
  I2C_SDA_RESET;
  I2C_SCL_SET;
  delay5us();
  I2C_SDA_SET;
  delay5us();
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void I2C_ACK(void)			 //Ӧ��
{
  I2C_SCL_RESET;
  I2C_SDA_OutMode;
  I2C_SDA_RESET;
  delay5us();
  I2C_SCL_SET;
  delay5us();
  I2C_SCL_RESET;
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void I2C_NoACK(void)			 //��Ӧ��
{
  I2C_SCL_RESET;
  I2C_SDA_OutMode;
  delay5us();
  I2C_SDA_SET;
  delay5us();
  I2C_SCL_SET;
  delay5us();
  I2C_SCL_RESET;
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
unsigned char I2C_Wait_Ack(void)    //�ȴ�Ӧ��
{  
  unsigned short ucErrTime=0;  
  I2C_SDA_InMode;
  I2C_SDA_SET;
  delay5us();
  I2C_SCL_SET;
  delay5us();  
  while(I2C_SDA) 
  {  
    ucErrTime++;  
    if(ucErrTime>250)  
    {  
      I2C_Stop();  
      return 1;  
    }  
  }  
  I2C_SCL_RESET;//ʱ�����0        
  return 0;    
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void I2C_Send_Byte(unsigned char txd)  
{                          
  unsigned char t;     
  I2C_SDA_OutMode;        
  I2C_SCL_RESET;//����ʱ�ӿ�ʼ���ݴ���  
  for(t=0;t<8;t++)  
  {                
       
    if((txd&0x80)>>7)  
      I2C_SDA_SET;  
    else  
      I2C_SDA_RESET;  
    txd<<=1;      
    delay5us();  
    I2C_SCL_SET;  
    delay5us(); 
    I2C_SCL_RESET;    
    delay5us();
  }      
  I2C_ACK();
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
unsigned char I2C_Read_Byte(unsigned char ack)  
{  
  unsigned char i,receive=0;  
  I2C_SDA_InMode;
  I2C_SDA_SET; 
  for(i=0;i<8;i++ )  
  {  
    I2C_SCL_RESET;   
    delay5us(); 
    I2C_SCL_SET;  
    receive<<=1;  
    if(I2C_SDA)
      receive++;     
    delay5us();  
  }                      
  if (!ack)  
    I2C_NoACK();//����nACK  
  else  
    I2C_ACK(); //����ACK     
  return receive;  
}  
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void _24XX128_WriteBytes(unsigned int Address,unsigned char *Data,unsigned int num)		  //����ַд��һ�ֽ�����
{
	unsigned int min_num;
	unsigned char addh,addl,j;
	unsigned int i;
	min_num=64-(Address%64);
	if(min_num>num)
		min_num=num;
	if(min_num!=64)
	{
		addh=(Address>>8)&0x00ff;
		addl=Address&0x00ff;
		I2C_Start();							      //��ʼλ
		I2C_Send_Byte(0xa0);				   //дһ�ֽ�����
		I2C_Send_Byte(addh);				   //дһ�ֽڵ�ַ
		I2C_Send_Byte(addl);				   //дһ�ֽڵ�ַ
		for(j=0;j<min_num;j++)
		{
			I2C_Send_Byte(Data[j]);					   //дһ�ֽ�����
		}
		I2C_Stop();							   //����λ	
		Address+=min_num;	
		delay5ms();
		num-=min_num;
	}
	else
		min_num=0;
	if((num/64)!=0)
	{
		for(i=0;i<(num/64);i++)
		{
			addh=(Address>>8)&0x00ff;
			addl=Address&0x00ff;
			I2C_Start();							   //��ʼλ
			I2C_Send_Byte(0xa0);				   //дһ�ֽ�����
			I2C_Send_Byte(addh);				   //дһ�ֽڵ�ַ
			I2C_Send_Byte(addl);				   //дһ�ֽڵ�ַ
			for(j=0;j<64;j++)
			{
				I2C_Send_Byte(Data[i*64+j+min_num]);					   //дһ�ֽ�����
			}
			I2C_Stop();							   //����λ	
			Address+=64;	
			delay5ms();				
		}
	}
	if((num%64)!=0)
	{
		addh=(Address>>8)&0x00ff;
		addl=Address&0x00ff;
		I2C_Start();							   //��ʼλ
		I2C_Send_Byte(0xa0);				   //дһ�ֽ�����
		I2C_Send_Byte(addh);				   //дһ�ֽڵ�ַ
		I2C_Send_Byte(addl);				   //дһ�ֽڵ�ַ
		for(j=0;j<(num%64);j++)
		{
			I2C_Send_Byte(Data[num-(num%64)+j+min_num]);					   //дһ�ֽ�����
		}
		I2C_Stop();							   //����λ	
	//	Address+=64;	
		delay5ms();	
	}		
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void _24XX128_ReadBytes(unsigned int Address,unsigned char *Data,unsigned int num)					   //����ַ����һ�ֽ�����
{
	unsigned char addh,addl;
	unsigned int i;
	addh=(Address>>8)&0x00ff;
	addl=Address&0x00ff;									   
	I2C_Start();									  //��ʼλ 
	I2C_Send_Byte(0xa0);						  //дһ�ֽ�����
	I2C_Send_Byte(addh);						  //дһ�ֽڵ�ַ
	I2C_Send_Byte(addl);						  //дһ�ֽڵ�ַ
	I2C_Start();									  //��ʼλ
//	Order|=0x01;									  //������תΪ������
	I2C_Send_Byte(0xa1);						  //д������
	for(i=0;i<num;i++)
	{
		if(i<(num-1))
			Data[i]=I2C_Read_Byte(1);						  //��ȡһ�����ڣ�����Ч�飩
		else
			Data[i]=I2C_Read_Byte(0);						  //��ȡ���һ���ֽڣ�������Ч�飩
	}
	I2C_Stop();									  //����λ
//	return Data;									  //���ض�ȡֵ
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void delay5us(void)   
{
  unsigned char i = 0;
  for(i=0;i<50;i++);
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void delay5ms(void)   
{
  unsigned int i = 0;
  for(i=0;i<50000;i++);
  for(i=0;i<50000;i++);
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
//END
