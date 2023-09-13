/*******************************************************************************
Copyright: 
File name: main.c
Description: ������
Author: ��ѧ��cy
Version: 
Date: 
History:       
*******************************************************************************/
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include "include.h"
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
void Event_Process(void);
void SystemResetLed_ON(void);
void SystemResetLed_OFF(void);
/*******************************************************************************
���ܴ��붨����
*******************************************************************************/
/*******************************************************************************
 Function:      //
 Description:   //������
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void main(void)
{ 
  disableInterrupts();                                      //�����ж�
  MCU_Config();
  RTC_Config();
  Pulse_Config();  
  enableInterrupts();                                       //�����ж�
/////////////////////////////////////////////////////////   
  SystemResetLed_ON();
  Delay_ms(500);
  SystemResetLed_OFF();
  DataProcess_Clear64Bytes();
  DataProcess_ReadDevicePara();                  
  DataProcess_CheckRereadParas();
  DataPocess_WriteDebugResetRecord();
/********************************DEBUG*****************************************/
/********************************DEBUG*****************************************/  
  while(1)
  { 
    IWDG_ReloadCounter();          
    if(DevicePara.Mode.All == SLEEP)      //�豸����˯��ģʽ
    {  
      Sleep();
    }  
    else
    {
      SoftTimer_Process();
      UplinkProtocol_TxProcess();
      BC95_Process();
      Event_Process();
      Debug_Process();
    }
  }
}
/*******************************************************************************
 Function:      //
 Description:   //�¼�����
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void Event_Process(void)
{
  //�����ϱ�
  if(DevicePara.Event.PeriodicReport != FREE)
  {
    DevicePara.Event.PeriodicReport = FREE;
    DataProcess_WriteFlow();                    //���浱ǰˮ��
    DataPocess_WriteDebugReportStatistics(0);   //��������+1
    DataPocess_WriteDebugReportRecord(0);       //��������ʱ��
    UplinkProtocol_Init();
  }
  
  //���ڲ���
  if(DevicePara.Event.PeriodicSample != FREE)
  {
    DevicePara.Event.PeriodicSample = FREE;
    DataPocess_WriteSampleData();       //�����������
//    DataProcess_WriteFlow();            //���浱ǰˮ��
    DevicePara.Mode.PeriodicSample = SLEEP;
  }
  
  //�ֶ��ϱ�
  if(DevicePara.Event.ManualReport != FREE)
  {
    DevicePara.Event.ManualReport = FREE;
    DataProcess_WriteFlow();                    //���浱ǰˮ��
    DataPocess_WriteDebugReportStatistics(0);   //��������+1
    DataPocess_WriteDebugReportRecord(0);       //��������ʱ��
    UplinkProtocol_Init();  
  }

  //��������
  if(DevicePara.Event.ManualDebug != FREE)
  {
    DevicePara.Event.ManualDebug = FREE;
    DataProcess_WriteFlow();                    //���浱ǰˮ��
    Debug_Init();
  }

  //�澯�ϱ�
  if(DevicePara.Event.AlarmReport != FREE)
  {
    DevicePara.Event.AlarmReport = FREE;
    DataPocess_WriteDebugReportStatistics(0);   //��������+1
    DataPocess_WriteDebugReportRecord(0);       //��������ʱ��
    UplinkProtocol_Init();  
  }
  
  //���ѳ�ʱ��λ
  if(DevicePara.Event.WakeTimeoutRst != FREE)
  {
    DevicePara.Event.WakeTimeoutRst = FREE;
    DevicePara.Mode.WakeTimeoutRst = SLEEP;
    DataProcess_WriteFlow();  //���浱ǰˮ��
    DataPocess_WriteWakeTimeoutResetRecord();
    SystemReset();            //ϵͳ��λ
  }

  //�ϱ���ʱ��λ
  if(DevicePara.Event.ReportTimeoutRst != FREE)
  {
    DevicePara.Event.ReportTimeoutRst = FREE;
    DevicePara.Mode.ReportTimeoutRst = SLEEP;
    DataProcess_WriteFlow();    //���浱ǰˮ��
    DataPocess_WriteReportTimeoutResetRecord();
    SystemReset();              //ϵͳ��λ   
  }
  
    //�ֶ���λ
  if(DevicePara.Event.ManualRst != FREE)
  {
    DevicePara.Event.ManualRst = FREE;
    DevicePara.Mode.ManualRst = SLEEP;
    DataProcess_WriteFlow();    //���浱ǰˮ��
    SystemReset();              //ϵͳ��λ   
  }
  
  //���¶�ȡ����
  if(DevicePara.Event.RereadParas != FREE)
  {
    DevicePara.Event.RereadParas = FREE;
    /*���ϱ�����*/
    DataProcess_ReadReportParas();
    if( (0 == DevicePara.ReportParas.StartTimeHour)
       &&(0 == DevicePara.ReportParas.StartTimeMinute) )
    {
      DevicePara.ReportParas.StartTimeValid = FALSE;
      DevicePara.ReportParas.Timer = 0;
      DevicePara.Mode.PeriodicReport = WAKE;
      DevicePara.Event.PeriodicReport = PEND;
    }
    /*����������*/
    DataProcess_ReadSampleParas();
    DevicePara.SampleParas.Valid = TRUE;
    
    DevicePara.Mode.RereadParas = SLEEP;
  }
}
/*******************************************************************************
 Function:      //
 Description:   //��λָʾ��
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void SystemResetLed_ON(void)
{
  GPIO_SetBits(GPIOD, GPIO_Pin_6);
  GPIO_SetBits(GPIOD, GPIO_Pin_7);
}
void SystemResetLed_OFF(void)
{
  GPIO_ResetBits(GPIOD, GPIO_Pin_6);
  GPIO_ResetBits(GPIOD, GPIO_Pin_7);
}
/*************************************END**************************************/