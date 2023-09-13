/*****************************************************************************************************
Copyright: 
File name: fram.c  
Description: fram
Author: һ��ˮ
Version: 
Date: 
History:       
*****************************************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "fram.h"
/*********************************************************************************
����������
*********************************************************************************/
/*********************************************************************************
��������������
*********************************************************************************/
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
/*********************************************************************************
˽�б���������
*********************************************************************************/ 

/*********************************************************************************
���Ա���������
*********************************************************************************/
/*********************************************************************************
�ڲ�����������
*********************************************************************************/

/*********************************************************************************
���ܴ��붨����
*********************************************************************************/
/**************************************************************************************************
 * @fn      delay10us
 *
 * @brief   framͨѶ��С��ʱ
 *
 * @param   void
 *
 * @return  void
 *
 **************************************************************************************************/
static void delay10us(void)
{
  unsigned char a;
  for(a=16;a>0;a--);
}
/**************************************************************************************************
 * @fn      SPI_init
 *
 * @brief   SPI1�ӿڳ�ʼ��
 *
 * @param   void
 *
 * @return  void
 *
 **************************************************************************************************/
static void SPI_init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_SPI1 , ENABLE);                 
    SPI_DeInit(SPI1);	
    SPI_Init(SPI1,
             SPI_FirstBit_MSB,
             SPI_BaudRatePrescaler_4,
             SPI_Mode_Master,
             SPI_CPOL_Low,
             SPI_CPHA_1Edge,
             SPI_Direction_2Lines_FullDuplex,
             SPI_NSS_Soft,
             (uint8_t)0x07);	      	
    SPI_Cmd(SPI1, ENABLE);	 
}

/**************************************************************************************************
 * @fn      fram_init
 *
 * @brief   fram��ʼ��
 *
 * @param   void ������Դ����ʼ��spi
 *
 * @return  void
 *
 **************************************************************************************************/
void fram_init(void)
{
  FRAM_PWR_Set();
  SPI_init();//��ʼ��SPI1
  GPIO_Init(GPIOB,GPIO_Pin_7,GPIO_Mode_In_PU_No_IT);
}

/**************************************************************************************************
 * @fn      fram__off
 *
 * @brief   fram�ر�
 *
 * @param   void
 *
 * @return  void
 *
 **************************************************************************************************/
static void fram_off(void)
{
  FRAM_PWR_Rst();
  SPI_DeInit(SPI1);//ע��SPI2
  GPIO_Init(GPIOB,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Slow);
}

/**************************************************************************************************
 * @fn      SPI_write_byte(unsigned char byte)
 *
 * @brief   SPIдһ���ֽ�
 *
 * @param   unsigned char byte
 *
 * @return  void
 *
 **************************************************************************************************/
static void SPI_write_byte(unsigned char byte)
{
  while((SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE))==RESET);     //�ȴ��������      
	SPI_SendData(SPI1, byte);
  while ((SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE))==RESET) ; 
        SPI_ReceiveData(SPI1);  /*��ȡ�����淢�����ݲ�������Ч����*/ 
}


/**************************************************************************************************
 * @fn      SPI_write_byte(unsigned char byte)
 *
 * @brief   SPI��һ���ֽ�
 *
 * @param   void
 *
 * @return  unsigned char byte;
 *
 **************************************************************************************************/
static unsigned char SPI_read_byte(void)
{
  unsigned char byte;
  while (SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE) == RESET) ; 
    SPI_SendData(SPI1,0xFF);  /*  ����һ����Ч�ֽڣ��Բ�������ʱ��*/ 
  while((SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE))==RESET);     //�ȴ��������
    byte=SPI_ReceiveData(SPI1) ;
    //SPI_ClearFlag(SPI1,SPI_FLAG_RXNE);  //SPIF(SPSTAT.7)��־ͨ���������д�롰1����0      
  return (byte);		
}

/**************************************************************************************************
 * @fn      FM_WriteEnable
 *
 * @brief   дʹ��
 *
 * @param   void
 *
 * @return  void
 *
 **************************************************************************************************/
static void FM_WriteEnable(void) //дʹ�� ��д��
{      
  cs_set;
  delay10us();
  cs_reset;
  delay10us();
  SPI_write_byte(WREN);
  cs_set;	
}

/**************************************************************************************************
 * @fn      FM_WriteDisable
 *
 * @brief   дʧ��
 *
 * @param   void
 *
 * @return  void
 *
 **************************************************************************************************/
static void FM_WriteDisable(void)//����д��
{     
  cs_reset;
  SPI_write_byte(WRDI);
  cs_set;	
}

/**************************************************************************************************
 * @fn      FM_WriteByte(unsigned int addr, unsigned char dat)
 *
 * @brief   ��ָ����ַд��1byte������
 *
 * @param   unsigned int addr, unsigned char data
 *
 * @return  void
 *
 **************************************************************************************************/
void FM_WriteByte(unsigned int addr, unsigned char data)
{
  fram_init();
//  disableInterrupts();
  FM_WriteEnable();
  cs_reset;
  SPI_write_byte(WRITE);
  SPI_write_byte((addr & 0xFF00) >> 8);
  SPI_write_byte(addr & 0x00FF);  
  SPI_write_byte(data);
  cs_set;
  FM_WriteDisable();
//  enableInterrupts(); 
  fram_off();
}

/**************************************************************************************************
 * @fn      FM_ReadByte(unsigned int addr)
 *
 * @brief   ��ָ����ַ����1byte������
 *
 * @param   void
 *
 * @return  unsigned char temp;
 *
 **************************************************************************************************/
unsigned char FM_ReadByte(unsigned int addr)
{
  //fram_init();
  unsigned char temp;
  cs_set;
  delay10us();
  cs_reset;
  delay10us();
  SPI_write_byte(READ);
  SPI_write_byte((addr & 0xFF00) >> 8);
  SPI_write_byte(addr & 0x00FF);
      temp = SPI_read_byte();   
  cs_set;
  //fram_off();
  return(temp);
}

/**************************************************************************************************
 * @fn      FM_Write(unsigned long addr, unsigned char *pheader, unsigned int length)
 *
 * @brief   ��ָ����ַ��ʼд��length byte������
 *
 * @param   unsigned long addr, unsigned char *pheader, unsigned int length
 *
 * @return  void
 *
 **************************************************************************************************/
void FM_Write(unsigned long addr, unsigned char *pheader, unsigned int length)
{    
  unsigned char i;
  fram_init();
//  disableInterrupts();
  FM_WriteEnable();
  delay10us();
  cs_reset;
  delay10us();
  SPI_write_byte(WRITE);
  SPI_write_byte((addr & 0xFF00) >> 8);
  SPI_write_byte(addr & 0x00FF);

  for(i = 0;i < length;i ++)  
  {
    SPI_write_byte(pheader[i]);
  }	
  cs_set;	
  delay10us();
  FM_WriteDisable();
  delay10us();
//  enableInterrupts(); 
  fram_off();
}

/**************************************************************************************************
 * @fn      FM_Read(unsigned long addr, unsigned char *pheader, unsigned int length)
 *
 * @brief   ��ָ����ַ��ʼ����length byte������
 *
 * @param   unsigned long addr, unsigned char *pheader, unsigned int length
 *
 * @return  void
 *
 **************************************************************************************************/
void FM_Read(unsigned long addr, unsigned char *pheader, unsigned int length)
{
  unsigned int i;
  fram_init();
  
  for(i = 0;i < length;i ++)
  {
    pheader[i] = FM_ReadByte(addr+i);
  }
  
  fram_off();	  
}
