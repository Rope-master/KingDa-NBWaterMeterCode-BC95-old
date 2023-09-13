/*******************************************************************************
Copyright: 
File name: usart.h
Description: ������غ���������
Author: ��ѧ��cy
Version: 
Date: 
History:       
*******************************************************************************/
#ifndef __USART_H__
#define __USART_H__
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include "include.h"
/*******************************************************************************
�궨����
*******************************************************************************/
#define USART1_HALF_DUPLEX           //����1��˫��
//#define USART1_FULL_DUPLEX            //����1ȫ˫��

//#define USART2_HALF_DUPLEX           //����2��˫��
#define USART2_FULL_DUPLEX           //����2ȫ˫��

//#define USART3_HALF_DUPLEX           //����3��˫��
#define USART3_FULL_DUPLEX           //����3ȫ˫��

#define USART1_RX_ENABLE           USART1->CR2 |= 0X04;
#define USART1_RX_DISABLE          USART1->CR2 &= 0XFB;

#define USART2_RX_ENABLE           USART2->CR2 |= 0X04;
#define USART2_RX_DISABLE          USART2->CR2 &= 0XFB;

#define USART3_RX_ENABLE           USART3->CR2 |= 0X04;
#define USART3_RX_DISABLE          USART3->CR2 &= 0XFB;

#define USART1_TX_MAX            128//���ͻ�������С�궨��
#define USART1_RX_MAX            256//���ջ�������С�궨��

#define USART2_TX_MAX            1055//���ͻ�������С�궨��
#define USART2_RX_MAX            256//���ջ�������С�궨��

#define USART3_TX_MAX            256//���ͻ�������С�궨��
#define USART3_RX_MAX            256//���ջ�������С�궨��
/*******************************************************************************
�������Ͷ���
*******************************************************************************/
typedef struct USART1_struct                            //����1�ṹ��
{
  unsigned char RxBuffer[USART1_RX_MAX];               //������ջ���
  bool RxPend;                                          //���ڽ��չ����־
  bool RxBusy;                                          //����æ��־
  unsigned short RxLength;                            //�������ݳ���
  
  unsigned char TxBuffer[USART1_TX_MAX];               //���ͻ�����
  bool TxBusy;                                          //����æ��־
  unsigned short TxLength;                            //�������ݳ���
  unsigned short TcLength;                            //�ѷ��͵����ݳ���  
}USART1_TypeDef;

typedef struct USART2_struct                            //�ô���2�ṹ��
{
  unsigned char RxBuffer[USART2_RX_MAX];               //������ջ���
  bool RxPend;                                          //���ڽ��չ����־
  bool RxBusy;                                          //����æ��־
  unsigned short RxLength;                            //�������ݳ���
  
  unsigned char TxBuffer[USART2_TX_MAX];               //���ͻ�����
  bool TxBusy;                                          //����æ��־
  unsigned short TxLength;                            //�������ݳ���
  unsigned short TcLength;                            //�ѷ��͵����ݳ���
}USART2_TypeDef;

typedef struct USART3_struct                            //����3�ṹ��
{
  unsigned char RxBuffer[USART3_RX_MAX];               //������ջ���
  bool RxPend;                                          //���ڽ��չ����־
  bool RxBusy;                                          //����æ��־
  unsigned short RxLength;                            //�������ݳ���
  
  unsigned char TxBuffer[USART3_TX_MAX];               //���ͻ�����
  bool TxBusy;                                          //����æ��־
  unsigned short TxLength;                            //�������ݳ���
  unsigned short TcLength;                            //�ѷ��͵����ݳ���                                                 
}USART3_TypeDef;
/*******************************************************************************
�ⲿ����������
*******************************************************************************/
extern USART1_TypeDef  Usart1;
extern USART2_TypeDef  Usart2;
extern USART3_TypeDef  Usart3;
/*******************************************************************************
����������
*******************************************************************************/
void USART1_Config(u32 BaudRate);
void USART2_Config(u32 BaudRate);
void USART3_Config(u32 BaudRate);

void USART1_RX_IRQHandler(void);                                                   
void USART1_TX_IRQHandler(void);                                                   
ErrorStatus USART1_TX(uint8_t *buff,uint16_t length);
//void USART1_TX(void);
uint16_t USART1_RX(uint8_t *buff);
void USART1_Free(void);

void USART2_RX_IRQHandler(void);                                                   
void USART2_TX_IRQHandler(void);                                                   
//ErrorStatus USART2_TX(unsigned char *buff,unsigned short length);
void USART2_TX(void);
//void USART2_TX_IQ(unsigned char *buff,unsigned short length);
uint16_t USART2_RX(uint8_t *buff);
void USART2_Free(void);

void USART3_RX_IRQHandler(void);                                                   
void USART3_TX_IRQHandler(void);                                                   
ErrorStatus USART3_TX(uint8_t *buff,uint16_t length);
uint16_t USART3_RX(uint8_t *buff);
void USART3_Free(void);
/******************************************************************************/
#endif
/*************************************END**************************************/
