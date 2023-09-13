#ifndef __GLOBALDATA_H__
#define __GLOBALDATA_H__
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "include.h"
/*********************************************************************************
�궨����
*********************************************************************************/
#define HistoryDataMaxNum       200
#define HistoryDataSize         9
/*********************************************************************************
�������Ͷ���
*********************************************************************************/
enum Device_Status_EN
{
 SLEEP,  //˯��
 WAKE    //����
};
enum Device_Event_EN
{
  FREE,  //�ͷ�
  PEND   //����
};
/*�豸��Ԫ�����嶨��*/
typedef union 
{ 
  uint8_t All;
  struct
  {
    uint8_t bc95:1;   
    uint8_t eeprom:1;
    uint8_t debug:1;
    uint8_t reverse:5;
  };
}Device_UnitUnion;

struct Meter_Parameter_EN
{ 
  Device_UnitUnion Mode;                //ģʽ
  Device_UnitUnion Event;               //�¼�
  u8 MeterNumber[7];                    //���
  u16 AlarmVoltage;                     //��ظ澯��ѹ
  u8 SettleDate;                        //��������
  u16 ReportFrequency;                  //�ϱ�Ƶ�ʣ���
  u16 SampleFrequency;                  //����Ƶ��, ��
  u8 DRStartTimeHour;                   //��ɢ�ϱ���ʼʱ��ʱλ Discrete report
  u8 DRStartTimeMinute;                 //��ɢ�ϱ���ʼʱ���λ Discrete report
  u16 DRDuration;                       //��ɢ�ϱ�ʱ�� Discrete report
  bool DRValid;                        //��ɢ�ϱ���Ч
  u32 ReportRandTime;                   //�ϱ����ʱ�䣬��
  u32 ReportRandTiming;                 //�ϱ������ʱ����
  u32 ReportTiming;                     //�ϱ���ʱ����
  u32 SampleTiming;                     //������ʱ����
  u32 SaveFlowTiming;                   //����ˮ����ʱ����
  signed char Temp;                   //�¶�
  u16 Voltage;                          //��ѹ 
};

typedef struct 
{ 
  u8 Front;     //ͷ
  u8 Rear;      //β
  u8 Total;     //����
}History_Data_TypeDef;
/*********************************************************************************
����������
*********************************************************************************/
/*********************************************************************************
��������������
*********************************************************************************/
extern RTC_TimeTypeDef   RTC_TimeStr;        //RTCʱ��ṹ��
extern RTC_DateTypeDef   RTC_DateStr;        //RTC���ڽṹ��
extern struct Meter_Parameter_EN MeterParameter;
extern History_Data_TypeDef HistoryData;
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
unsigned short CRC16(unsigned char* data,unsigned short length);
unsigned char Sum_Check(unsigned char *sdata,unsigned short NUM);                //�����У��
unsigned char Int_to_ASCLL(unsigned char data);
unsigned char ASCLL_to_Int(unsigned char data);
unsigned char BCD_to_Int(unsigned char data);
char* strnstr(char* str1,char* str2,unsigned char len);
unsigned char Check_Sum8(unsigned char *data,unsigned short length);
/*********************************************************************************/
#endif
/******************************************END********************************************************/