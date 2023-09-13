/*******************************************************************************
Copyright: 
File name: common.h
Description: ������غ���������
Author: ��ѧ��cy
Version: 
Date: 
History: 
*******************************************************************************/
#ifndef __COMMON_H__
#define __COMMON_H__
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include "include.h"
/*******************************************************************************
�궨����
*******************************************************************************/
#define DEVICE_ID               0x01    //NB�������ر�	                  
//#define SOFTWARE_VERSION        100     //����汾

#define SampleDataEachSize      9            //��������ÿ����С
#define SampleDataMax           200          //��������������� 1Сʱһ�Σ��ɴ洢50��
/*******************************************************************************
�������Ͷ���
*******************************************************************************/

enum Device_Status_EN
{
  SLEEP,  //˯��
  WAKE     //����
};
enum Device_Event_EN
{
  FREE,  //�ͷ�
  PEND   //����
};

  
/*ѭ�����нṹ�嶨��*/
typedef struct    
{
  uint16_t Total;               //����
  uint16_t Front;               //��ַ
  uint16_t Rear;                //βַ
}CircularQueue_TypeDef;

/*ʱ��ṹ�嶨��*/
typedef struct    
{
  uint8_t Year;         
  uint8_t Month;           
  uint8_t Day;     
  uint8_t Hour;         
  uint8_t Minute;           
  uint8_t Second;  
}Time_TypeDef;

/*ˮ������������*/
typedef union                                            
{
  uint32_t flow32;
  uint8_t flow8[4];
}FlowUnion; 
/*�豸��Ԫ�����嶨��*/
typedef union 
{ 
  uint16_t All;
  struct
  {
    uint16_t PeriodicReport:1;   
    uint16_t PeriodicSample:1;
    uint16_t ManualWake:1;
    uint16_t ManualReport:1;
    uint16_t ManualSample:1;
    uint16_t ManualDebug:1; 
    uint16_t AlarmReport:1;
    uint16_t ValveCtrl:1;
    uint16_t MaintainValve:1;
    uint16_t DayFreeze:1;
    uint16_t MonthFreeze:1;
    uint16_t WakeTimeoutRst:1;
    uint16_t ReportTimeoutRst:1;
    uint16_t ManualRst:1;
    uint16_t RereadParas:1;
    uint16_t Reverse:1;
  };
}Device_UnitUnion;
/*�豸״̬�����嶨��*/
typedef union 
{ 
  uint16_t All;
  struct
  {
    uint16_t Interfere:1;       //�Ÿ��Ÿ澯��0:����,1:�澯
    uint16_t LowVoltage:1;      //��ѹ�澯��0:����,1:�澯
    uint16_t Separation:1;      //����澯��0:����,1:�澯
    uint16_t Sensor1:1;         //������1�澯��0:����,1:�澯
    uint16_t Sensor2:1;         //������2�澯��0:����,1:�澯
    uint16_t Sensor3:1;         //������3�澯��0:����,1:�澯
    uint16_t OverFlow:1;        //�����澯��0:����,1:�澯
    uint16_t HighPress:1;       //��ѹ�澯��0:����,1:�澯
    uint16_t LowPress:1;        //��ѹ�澯��0:����,1:�澯
    uint16_t EmptyPipe:1;       //�չܸ澯��0:����,1:�澯
    uint16_t Excitation:1;      //���Ÿ澯��0:����,1:�澯
    uint16_t Electrode:1;       //�缫�澯��0:����,1:�澯
    uint16_t MeterLose:1;       //����ʧ���澯��0:����,1:�澯
    uint16_t Memory:1;          //�洢���澯��0:����,1:�澯
    uint16_t Reverse:1;         //Ԥ��
  };
}Device_AlarmUnion;
/*�����ṹ�嶨��*/
typedef  struct
{
  CircularQueue_TypeDef Queue;
  uint16_t Frequency;
  uint16_t Timer;
  bool Valid;
}SampleParas_TypeDef;

/*�ϱ���ز����ṹ�嶨��*/
typedef struct 
{ 
  uint16_t Frequency;
  uint8_t StartTimeHour;       //��ʼʱ��ʱλ 
  uint8_t StartTimeMinute;     //��ʼʱ���λ
  uint16_t DRDuration;          //��ɢ�ϱ�ʱ��,m
  bool StartTimeValid;
  uint16_t Timer;               //��ʱ��,m
  uint32_t Timeout;             //��ʱ��ʱ
}ReportParas_TypeDef;

typedef enum 
{
 NORMAL,
 SENSOR1,
 SENSOR2,
 INTERFERE
}SensorError_TypeDef;

/*�豸��ز����ṹ�嶨��*/
typedef struct 
{ 
  uint16_t WeekTime;                    //����ʱ�䣬s
  Device_UnitUnion Mode;                //ģʽ
  Device_UnitUnion Event;               //�¼�
  Device_AlarmUnion AlarmMark;          //�������
  SensorError_TypeDef SensorError;      //����������
  FlowUnion ForwardFlow;                //����ˮ��
  ReportParas_TypeDef ReportParas;      //�ϱ���ز���
  SampleParas_TypeDef SampleParas;      //������ز���
  uint16_t Voltage;                     //��ѹ          
  int8_t Temp;                          //�¶�
  bool IsRereadParas;                   //���¶�ȡ����
}DevicePara_TypeDef;
/*******************************************************************************
����������
*******************************************************************************/
/*******************************************************************************
��������������
*******************************************************************************/
extern DevicePara_TypeDef DevicePara;  
/*******************************************************************************
�ⲿ����������
*******************************************************************************/
/*******************************************************************************
���ܴ��붨����
*******************************************************************************/
uint8_t Int_to_ASCLL(uint8_t data);
uint8_t ASCLL_to_Int(uint8_t data);
uint8_t BCD_to_Int(uint8_t data);
//char* strnstr(char* str1,char* str2,uint16_t len);
uint8_t Checksum8(uint8_t *buff,uint16_t length);
//uint16_t CRC16_NB(uint8_t* data,uint16_t length);
uint16_t CRC16_Modbus(uint8_t *buff, uint16_t lenth);
//uint8_t XORCheck(uint8_t *buff,uint16_t length);
//uint8_t Byte_Reverse(uint8_t data);
uint16_t GetOffsetTime(uint8_t *buff);
void GetDeviceId(uint8_t *buff);
/******************************************************************************/
#endif
/*************************************END**************************************/