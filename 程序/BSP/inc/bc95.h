/*******************************************************************************
Copyright: 
File name: BC95.h
Description: BC95��غ���������
Author: ��ѧ��cy
Version: 
Date: 
History: 
*******************************************************************************/
#ifndef __BC95_H__
#define __BC95_H__
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include "include.h"
/*******************************************************************************
�궨����
*******************************************************************************/
//BC95��Դ����
#define BC95_PWR_Set        GPIO_SetBits(GPIOE,GPIO_Pin_2);
#define BC95_PWR_Rst        GPIO_ResetBits(GPIOE,GPIO_Pin_2);
//BC95��λ����
#define BC95_RST_Set        GPIO_ResetBits(GPIOE,GPIO_Pin_1);
#define BC95_RST_Rst        GPIO_SetBits(GPIOE,GPIO_Pin_1);
//BC95״ָ̬ʾ��
//#define BC95_LED_ON        GPIO_SetBits(GPIOD, GPIO_Pin_6)
#define BC95_LED_OFF        GPIO_ResetBits(GPIOD, GPIO_Pin_6)

#define         RECV_BUFF_SIZE               256//���ջ�������С
#define         SEND_BUFF_SIZE               1050//���ͻ�������С
#define         BC95_TIMEROUT_NUM            10  //��ʱ����
#define         BC95_TIMEROUT_TIME           2000 //��ʱʱ�� ��λms

/*******************************************************************************
�������Ͷ���
*******************************************************************************/
/*BC95��������ö�ٶ���*/
typedef enum 
{
  BC95_IDLE,            //����
  BC95_SHUTDOWN,        //�ػ�
  BC95_RECONNECT,       //����

  NRB,                  //����
  AT,                   //ͬ��������
  CMEE,                 //�����ƶ��ն˴���
  CFUN0,                //������С����
  NCSEARFCN,            //�������Ƶ��
  CFUN1,                //����ȫ����
  CGSN,                 //��ѯIMEI 
  NCCID,                //��ѯCCID
  CIMI,                 //��ѯIMSI
  NCDP,                 //����CDP������
  NSMI,                 //�رշ�����Ϣָʾ
  NNMI,                 //�رս�����Ϣָʾ
  CGATT1,               //�������總��
  CGATT,                //��ѯ���總��
  CEREG,                //��ѯ����ע��
  CSQ,                  //��ѯ�ź�ǿ��
  NUESTATS,             //��ѯģ��״̬
  CCLK,                 //��ѯʵʱʱ��
  NMSTATUS,             //��ѯ��Ϣע��״̬
  QLWULDATAEX,          //����CON/NON��Ϣ
  NMGS,                 //������Ϣ
  NQMGR,                //��ѯ��Ϣ����״̬
  NMGR                  //������Ϣ
}BC95_ProcessStruct;

struct BC95_Str//BC95 �ܽṹ��
{
//  bool FirmwareUpdate;                         //FALSE:���У�TRUE:�̼����� 
  bool IncidentPend;                           //FALSE:���У�TRUE:�¼�����
  BC95_ProcessStruct Process;                   //���ӽ���
  uint8_t RxBuffer[RECV_BUFF_SIZE];             //���ջ�����
  uint16_t RxLength;                            //���ճ���
//  uint8_t TxBuffer[SEND_BUFF_SIZE];           //���ջ�����
//  uint16_t TxLength;                          //���ͳ���
  uint8_t CCID[20];                             //CCID  
  uint8_t IMEI[15];                             //IMEI        
  uint8_t IMSI[15];                             //IMSI
  uint8_t CSQ;                                  //�ź�ǿ��
  int16_t RSRP;                                 //�źŽ��չ���
  int16_t SNR;                                  //�����
  uint8_t ECL;                                  //��ǿ���ǵȼ�
  uint32_t CELLID;                              //С��ID
  uint32_t EARFCN;                              //Ƶ��    Range 0-262143
  uint16_t PCI;                                 //����С����ʶ Range 0-503
  uint8_t ErrorStep;                            //���ϲ���
  uint16_t ErrorCode;                           //���ϴ���
  uint8_t TimeoutNum;                           //��ʱ����
  uint8_t ReconnectTimes;                       //��������
};
 
/*******************************************************************************
�ⲿ����������
*******************************************************************************/
extern struct BC95_Str BC95;//BC95 �õļĴ���
/*******************************************************************************
����������
*******************************************************************************/
void BC95_Init(void); 
void BC95_Reset(void);
void BC95_Start(void);
void BC95_DeInit(void);
void BC95_Reconnect(void);
void BC95_ShutDown(void);
void BC95_Process(void);
void BC95_TxStr(unsigned char *Data,unsigned short Len);
void BC95_TxMessage_NMGS(uint8_t *buff,uint16_t length);
void BC95_RxData(uint8_t *buff,uint16_t length);
void BC95_GetErrorInfo(uint8_t *buff,uint16_t length);
void BC95_Timeout(void);
void BC95_Delay_CallBack(void);
void BC95_InquireMsgRecv(void);
void BC95_GetSignalParas(uint8_t *buff, uint16_t length);
void BC95_ACK(uint8_t mid[4]);
/******************************************************************************/
#endif
/*************************************END**************************************/