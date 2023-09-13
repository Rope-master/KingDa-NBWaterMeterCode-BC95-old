/*********************************************************************************
Copyright: 
File name: rs485.h
Description: ������غ���������
Author: ����һ��ˮ
Version: 
Date: 
History:       
*********************************************************************************/
#ifndef __FRAM_H__
#define __FRAM_H__
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include "include.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/
/*����Ĵ���*/
#define WRSR     0x01		//write status registerд״̬�Ĵ���
#define WRITE    0x02		//write memory dataд����
#define READ     0x03		//read memory data������
#define WRDI     0x04		// write disable��ֹд����
#define RDSR     0x05		//read status register ��״̬�Ĵ���
#define WREN     0x06		//set write enable latch����
/*����CS��WP��HOLD*/
#define FRAM_PWR_Set()      GPIO_SetBits(GPIOB,GPIO_Pin_3);
#define FRAM_PWR_Rst()      GPIO_ResetBits(GPIOB,GPIO_Pin_3);

#define  cs_reset        GPIO_ResetBits(GPIOB, GPIO_Pin_4)
#define  cs_set  	 GPIO_SetBits(GPIOB, GPIO_Pin_4)
/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/
/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/
/*********************************************************************************************************
����������
*********************************************************************************************************/
/*�ڲ�����*/
static void delay10us(void);
static void SPI_init(void);
void fram_init(void);
static void fram_off(void);
static void SPI_write_byte(unsigned char byte);
static unsigned char SPI_read_byte(void);
static void FM_WriteEnable(void);
static void FM_WriteDisable(void);

/*�ⲿ����*/
void FM_WriteByte(unsigned int addr, unsigned char data);
unsigned char FM_ReadByte(unsigned int addr);
void FM_Write(unsigned long addr, unsigned char *pheader, unsigned int length);
void FM_Read(unsigned long addr, unsigned char *pheader, unsigned int length);
/********************************************************************************************************/
#endif
/******************************************END***********************************************************/