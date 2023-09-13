/*******************************************************************************
Copyright: 
File name: rtc.c
Description: rtc��غ���������
Author: ��ѧ��cy
Version: 
Date: 
History: 
*******************************************************************************/
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include "rtc.h"
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
RTC_TimeUpdate_TypeDef TimeUpdate;
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
 Description:   //����
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void RTC_Config(void)
{
  RTC_InitTypeDef   RTC_InitStr;        
  
  CLK_LSEConfig(CLK_LSE_ON);                                    //��LSEʱ��
  while(CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);          //�ȴ�LSEʱ�Ӿ���
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);    //LSEʱ��1��Ƶ
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
  
  RTC_WakeUpCmd(DISABLE);                                       //RTC���ѹر�
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);          //��������Ϊ16��Ƶ 
  RTC_SetWakeUpCounter(2047);                                   //���û��ѼĴ���ʱ��Ϊ 1s
  RTC_ITConfig(RTC_IT_WUT, ENABLE);                             //ʹ��RTC�����ж�ʹ��
  RTC_ClearITPendingBit(RTC_IT_WUT);                            //���RTC���ѱ�־
  RTC_WakeUpCmd(ENABLE);                                        //�Զ�����ʹ��
  
  RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;               //����24Сʱ��
  RTC_InitStr.RTC_AsynchPrediv = 127;                           //�첽��Ƶ�� 128��Ƶ
  RTC_InitStr.RTC_SynchPrediv = 255;                            //ͬ����Ƶ�� 256��Ƶ
  RTC_Init(&RTC_InitStr);
  
  ITC_SetSoftwarePriority(RTC_CSSLSE_IRQn, ITC_PriorityLevel_3);//���ȼ� 
}
/*******************************************************************************
 Function:      //
 Description:   //��������
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void RTC_AlarmConfig(void) 
{
  RTC_AlarmTypeDef  RTC_AlarmStr;       
  
  RTC_AlarmStr.RTC_AlarmTime.RTC_Hours   = 1;
  RTC_AlarmStr.RTC_AlarmTime.RTC_Minutes = 0;
  RTC_AlarmStr.RTC_AlarmTime.RTC_Seconds = 0;
  RTC_AlarmStr.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay|RTC_AlarmMask_Hours|RTC_AlarmMask_Minutes;
  RTC_AlarmCmd(DISABLE);  
  RTC_SetAlarm(RTC_Format_BIN,&RTC_AlarmStr);

  RTC_ITConfig(RTC_IT_ALRA, ENABLE);                            //���������ж�
  RTC_ClearITPendingBit(RTC_IT_ALRA);                           //��������ж�
  RTC_AlarmCmd(ENABLE);                                         //��������
  ITC_SetSoftwarePriority(RTC_CSSLSE_IRQn, ITC_PriorityLevel_2);//RTC�������ȼ�
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void Close_Alarm(void)
{
  RTC_ITConfig(RTC_IT_ALRA, DISABLE);  //�ر������ж�
  RTC_AlarmCmd(DISABLE);    //�ر�����
}
/*******************************************************************************
 Function:      //
 Description:   //�����ж�
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void RTC_AlarmInterrupt(void)                        
{
  if(RESET != RTC_GetITStatus(RTC_IT_ALRA))
  {   

      
    RTC_ClearITPendingBit(RTC_IT_ALRA);              //��������ж�
  }
}
/*******************************************************************************
 Function:      //
 Description:   //�����ж�
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void RTC_WakeInterrupt (void)                        
{ 
  RTC_DateTypeDef date;
  RTC_TimeTypeDef time; 
  
  if(RESET != RTC_GetITStatus(RTC_IT_WUT))
  { 
    RTC_Second_Event();
    
    TimeUpdate.Second_Update++;
    if(TimeUpdate.Second_Update >= 30)
    {
      TimeUpdate.Second_Update = 0;
      RTC_GetDate(RTC_Format_BIN, &date);
      RTC_GetTime(RTC_Format_BIN, &time);
      if(TimeUpdate.Minute_Update != time.RTC_Minutes)
      {
        TimeUpdate.Minute_Update = time.RTC_Minutes;
        RTC_MinuteEvent(date,time);
      }
    }
    
    RTC_ClearITPendingBit(RTC_IT_WUT);                        //���RTC���ѱ�־
  }
}
/*******************************************************************************
 Function:      //
 Description:   //RTC���¼�
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void RTC_Second_Event(void)
{
  uint32_t timeoutLimit = 0;
  
  /*���ѳ���10���ӣ���λ*/
  if(DevicePara.Mode.All != 0)
  {
    DevicePara.WeekTime++;
    if(DevicePara.WeekTime > 600)       //10m
    {
      DevicePara.Mode.WakeTimeoutRst = WAKE;
      DevicePara.Event.WakeTimeoutRst = PEND;
      DevicePara.WeekTime = 0;
    }
  }
  else
  {
    DevicePara.WeekTime = 0;
  }
  
  /*����2���ϱ�����û�ϱ��ɹ�����λ*/
  timeoutLimit = (uint32_t)DevicePara.ReportParas.Frequency*60*2;
  if(timeoutLimit < 176400)     //��С49Сʱ
  {
    timeoutLimit = 176400;
  }
  DevicePara.ReportParas.Timeout++;
  if(DevicePara.ReportParas.Timeout > timeoutLimit)
  {
    DevicePara.Mode.ReportTimeoutRst = WAKE;
    DevicePara.Event.ReportTimeoutRst = PEND; 
    DevicePara.ReportParas.Timeout = 0;
  }   
}
/*******************************************************************************
 Function:      //
 Description:   //RTC�����¼�
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void RTC_MinuteEvent(RTC_DateTypeDef date,RTC_TimeTypeDef time)
{
  /*�ϱ�*/
  RTC_CycleReport(time);
  
  if(date.RTC_Year >= 20)
  {
    /*����*/
    RTC_CycleSample(time);
    
    /*0��*/
    if(time.RTC_Minutes == 0)
    {
      /*0ʱ*/
      if(time.RTC_Hours == 0)
      {
        /*ÿ��0������澯���*/
        DevicePara.AlarmMark.All = 0;   
        
        /*���¶�ȡ����*/
        if(DevicePara.IsRereadParas != FALSE)
        {
          DevicePara.Mode.RereadParas = WAKE;
          DevicePara.Event.RereadParas = PEND;
          DevicePara.IsRereadParas = FALSE;
        }
        
        /*1��*/
        if(date.RTC_Date == 1)
        {
          DevicePara.Mode.RereadParas = WAKE;
          DevicePara.Event.RereadParas = PEND;
        } 
      }
    }
  }
}
/*******************************************************************************
 Function:      //
 Description:   //�����ϱ�
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void RTC_CycleReport(RTC_TimeTypeDef time)
{
  if(DevicePara.ReportParas.StartTimeValid != FALSE)
  {
    if( (time.RTC_Hours == DevicePara.ReportParas.StartTimeHour)
       &&(time.RTC_Minutes == DevicePara.ReportParas.StartTimeMinute) )
    {
      DevicePara.ReportParas.StartTimeValid = FALSE;
      DevicePara.ReportParas.Timer = 0;
      DevicePara.Mode.PeriodicReport = WAKE;
      DevicePara.Event.PeriodicReport = PEND;
    }
  }
  else if(DevicePara.ReportParas.Frequency > 0)
  {
    DevicePara.ReportParas.Timer++;
    if( DevicePara.ReportParas.Timer >= DevicePara.ReportParas.Frequency )
    {   
      DevicePara.ReportParas.Timer = 0;
      DevicePara.Mode.PeriodicReport = WAKE;
      DevicePara.Event.PeriodicReport = PEND;
    }  
  }
}
/*******************************************************************************
 Function:      //
 Description:   //���ڲ���
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void RTC_CycleSample(RTC_TimeTypeDef time)
{
  if(DevicePara.SampleParas.Valid == FALSE)
  {
    if( (time.RTC_Hours == 0)
        &&(time.RTC_Minutes == 0) )
    {
      DevicePara.SampleParas.Valid = TRUE;
      DevicePara.SampleParas.Timer = 0;
      DevicePara.Mode.PeriodicSample = WAKE;
      DevicePara.Event.PeriodicSample = PEND;
    }
  }
  else
  {  
    if(DevicePara.SampleParas.Frequency > 0)
    {
      DevicePara.SampleParas.Timer++;
      if(DevicePara.SampleParas.Timer >= DevicePara.SampleParas.Frequency)
      {
        DevicePara.SampleParas.Timer = 0;
        DevicePara.Mode.PeriodicSample = WAKE;
        DevicePara.Event.PeriodicSample = PEND;
      }
    }
  }
}
/*******************************************************************************
 Function:      //
 Description:   //��������ʱ��ת���ɱ���ʱ��
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
ErrorStatus RTC_GMTtoBT(unsigned char *str)
{
  unsigned char year,month,day,hour,minute,second;
  unsigned char LeapYear = 0;
  RTC_DateTypeDef Date;
  RTC_TimeTypeDef Time;
  ErrorStatus status;
  
  year   = ASCLL_to_Int(str[6])*10 + ASCLL_to_Int(str[7]);
  if(year < 20)
  {
    return ERROR;
  }
  month  = ASCLL_to_Int(str[9])*10 + ASCLL_to_Int(str[10]);
  day    = ASCLL_to_Int(str[12])*10 + ASCLL_to_Int(str[13]);
  hour   = ASCLL_to_Int(str[15])*10 + ASCLL_to_Int(str[16]);
  minute = ASCLL_to_Int(str[18])*10 + ASCLL_to_Int(str[19]);
  second = ASCLL_to_Int(str[21])*10 + ASCLL_to_Int(str[22]);
  
  //�����ж�
  if( (((year+2000)%4 == 0) && ((year+2000)%100 != 0)) || ((year+2000)%400 == 0))
  {
    LeapYear = 1;
  }
  
  hour = hour+8;
  if(hour > 23)
  {
    hour = hour%24;
    day++;
    switch(month)
    {
      case 1:
      case 3:
      case 5:
      case 7:
      case 8:
      case 10:
        {
          if(day > 31)
          {
            day = 1;
            month++;
          }
        }
        break;
      case 4:
      case 6:
      case 9:
      case 11:
        {
          if(day > 30)
          {
            day = 1;
            month++;
          }
        }
        break;
      case 2:
        {
          if( (LeapYear == 0)&&(day > 28) )
          {
            day = 1;
            month++;
          }
          else if( (LeapYear == 1)&&(day > 29) )
          {
            day = 1;
            month++;
          }
        }
        break;
      case 12:
        {
          if(day > 31)
          {
            day = 1;
            month = 1;
            year++;
          }
        }
        break;  
    }
  }
  
  Date.RTC_Year = year;
  Date.RTC_Month = (RTC_Month_TypeDef)month;
  Date.RTC_Date = day;
  Time.RTC_Hours   = hour;
  Time.RTC_Minutes = minute;
  Time.RTC_Seconds = second;
  
  status = RTC_SetDate(RTC_Format_BIN, &Date);
  status = RTC_SetTime(RTC_Format_BIN, &Time);
  
  return status;
}
/*************************************END**************************************/