#ifndef __BC95_H__
#define __BC95_H__
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include "include.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/
#define         RECV_BUFF_SIZE               256//���ջ�������С
#define         SEND_BUFF_SIZE               128//���ͻ�������С
#define         BC95_TIMEROUT_NUM            10  //��ʱ����
#define         BC95_TIMEROUT_TIME           2000 //��ʱʱ�� ��λs

/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/
enum StartProcess_En   //��������
{
  IDLE,                 //����
  BC95_POWER_DOWN,      //����
  BC95_RECONNECT,       //����
  
  NRB,                  //����
  AT,                   //ͬ��������
  CMEE,                 //�����ƶ��ն˴���
  CFUN0,                //������С����
  NCSEARFCN,            //�������Ƶ��
  CFUN1,                //����ȫ����
  CGSN,                 //��ѯIMEI 
  CCID,                 //��ѯCCID
  NCDP,                 //����CDP������
  NSMI1,                //����������Ϣָʾ
  NNMI0,                //�رս�����Ϣָʾ
  CGATT1,               //�������總��
  CSQ,                  //��ѯ�ź�ǿ��
  CGATT,                //��ѯ���總��
  CEREG,                //��ѯ����ע��״̬
  NUESTATS,             //��ѯģ��״̬
  CCLK,                 //��ѯʵʱʱ��
  NMSTATUS,             //��ѯ��Ϣע��״̬
  NNMI1,                //���ý�����Ϣָʾ
  NMGS,                 //������Ϣ
};
  
struct BC95_Str//BC95 �ܽṹ��
{
  bool Manual;                                  //�ֶ��ϴ�
  uint8_t RxBuffer[RECV_BUFF_SIZE];        //���ջ�����
  unsigned short RxLength;         //���ճ���
  enum StartProcess_En StartProcess;  //���ӽ���
  uint8_t ReportBit;                        //����λ
  unsigned char TimeoutNum;         //��ʱ����
  unsigned char Rssi;                 //�ź�ǿ��
  bool IncidentPend;                  //�¼������־
  unsigned char ReconnectTimes;      //��������
  unsigned char ICCID[20];
  unsigned char IMEI[15];
  unsigned char ErrorStep;        //���ϲ���
  unsigned short ErrorCode;        //���ϴ���
  int16_t RSRP;                                 //�źŽ��չ���
  int16_t SNR;                                  //�����
  uint8_t ECL;                                  //��ǿ���ǵȼ�
};
 
/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/
extern struct BC95_Str BC95;//BC95 �õļĴ���
/*********************************************************************************************************
����������
*********************************************************************************************************/
void BC95_Power_On(void); 
void BC95_Power_Off(void);
void BC95_Reset(void);
void BC95_NetLightRun(void);
void BC95_Process(void);
void BC95_Data_Send(unsigned char *Data,unsigned short Len);

void BC95_Timeout_CallBack(void);
void BC95_Delay_CallBack(void);

void Recv_Data_Process(unsigned char* buff);
void Send_Data_Process(void);
void ACK(u8 messageId,u8 errcode,u8 mid[4]);
void Report_All_Parameters(void);
void Report_HC_Flow(void);
void Report_History_Data(void);
void BC95_GetSignalParas(uint8_t *buff, uint16_t length);
/********************************************************************************/
#endif

/******************************************END********************************************************/