/*******************************************************************************
Copyright: 
File name: uplink_protocol.h
Description: ����ͨ��Э����غ���������
Author: ��ѧ��cy
Version: 
Date: 
History:       
*******************************************************************************/
#ifndef __UPLINK_PROTOCOL_H__
#define __UPLINK_PROTOCOL_H__
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include "include.h"
/*******************************************************************************
�궨����
*******************************************************************************/
#define UPLINK_RX_MAX           64     //���н�������ֽ���
#define UPLINK_TX_MAX           200     //���з�������ֽ���
#define UPLINK_TIMEROUT_TIME    10000    //���г�ʱʱ��10s
/*******************************************************************************
�������Ͷ���
*******************************************************************************/
/*��Ϣ�붨��*/
typedef enum    
{
  MessageId_Idle        = 0x0000,       //����
  MessageId_0001        = 0x0001,       //�ϱ�״̬����
  MessageId_0002        = 0x0002,       //�ϱ���������
}UplinkProtocol_MessageId_TypeDef;

/*����Э��ṹ�嶨��*/
typedef  struct
{
  bool IncidentPend;                           //�¼������־ 
  UplinkProtocol_MessageId_TypeDef MsgId;       //��ϢID
//  uint8_t RxBuffer[UPLINK_RX_MAX];      //���ջ�����
//  uint8_t RxLength;                     //���ճ���
  uint8_t TxBuffer[UPLINK_TX_MAX];      //���ͻ�����
  uint16_t TxLength;                    //���ͳ���
  uint8_t TimeoutTimes;                 //��ʱ����
}UplinkProtocol_Struct;
/*******************************************************************************
�ⲿ����������
*******************************************************************************/
extern UplinkProtocol_Struct UP;
/*******************************************************************************
����������
*******************************************************************************/
void UplinkProtocol_Init(void);
void UplinkProtocol_DeInit(void);
void UplinkProtocol_TimeoutEnd(void);
void UplinkProtocol_StartReport(void);
void UplinkProtocol_TxProcess(void);
void UplinkProtocol_TxSuccess(void);
void UplinkProtocol_RxProcess(uint8_t *buff,uint16_t length);
void UplinkProtocol_ReportMessageId_0001(void);
void UplinkProtocol_ReportMessageId_0002(void);
void UplinkProtocol_Delay(void);
void UplinkProtocol_Timeout(void);
bool UplinkProtocol_IsReporting(void);

/******************************************************************************/
#endif
/*************************************END**************************************/


