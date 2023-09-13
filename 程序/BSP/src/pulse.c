/*******************************************************************************
Copyright: 
File name: pulse.c
Description: ������غ���������
Author: ��ѧ��cy
Version: 
Date: 
History: 
*******************************************************************************/
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include "pulse.h"
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
Pulse_Struct Pulse;
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
 Description:   //��������
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void Pulse_Config(void)
{
  GPIO_Init(Pulse_Cal1_Port, Pulse_Cal1_Pin, Pulse_Cal1_Mode);          //�������1���ų�ʼ�� 
  EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Falling);             //ѡ������1���������½����ж�
  EXTI_ClearITPendingBit (EXTI_IT_Pin1);                                //���жϱ�־λ
  ITC_SetSoftwarePriority(EXTI1_IRQn, ITC_PriorityLevel_2);             //�ж����ȼ��趨2
  
  GPIO_Init(Pulse_Cal2_Port, Pulse_Cal2_Pin, Pulse_Cal2_Mode);          //�������2���ų�ʼ�� 
  EXTI_SetPinSensitivity(EXTI_Pin_0, EXTI_Trigger_Falling);             //ѡ������0���������½����ж�
  EXTI_ClearITPendingBit (EXTI_IT_Pin0);                                //���жϱ�־λ
  ITC_SetSoftwarePriority(EXTI0_IRQn, ITC_PriorityLevel_2);             //�ж����ȼ��趨2

  GPIO_Init(Pulse_Weak_Port, Pulse_Weak_Pin, Pulse_Weak_Mode);          //���廽�����ų�ʼ�� 
  EXTI_SelectPort(EXTI_Port_F);                                         //ѡ��F�˿�              
  EXTI_SetHalfPortSelection(EXTI_HalfPort_F_LSB ,ENABLE);               //ѡ��PF(3:0)
  EXTI_SetPortSensitivity(EXTI_Port_F,EXTI_Trigger_Rising_Falling);     //GPIOF�ӿ� �½����ж�
  EXTI_ClearITPendingBit (EXTI_IT_PortF);                               //���жϱ�־λ
  ITC_SetSoftwarePriority(EXTIE_F_PVD_IRQn, ITC_PriorityLevel_2);       //�ж����ȼ��趨

}
/*******************************************************************************
 Function:      //
 Description:   //����1
                //��ת����4��״̬����תˮ����1
                //������10��ֻ��⵽����1���壬�����2��������
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void Pulse_Calculate1(void)
{
  uint8_t alarm = 0;
  
  if(Pulse_Cal1_Data() == RESET)        //����1�½���
  {
    Pulse.ForwardStatus = PulseStatus1; //��ת״̬1

    Pulse.Sensor1LostTimes = 0;
    if(DevicePara.SensorError == SENSOR1)
    {
      DevicePara.SensorError = NORMAL;
    }
   
    alarm += Pulse_Sensor2Check();
    alarm += Pulse_MgtIfrCheck();
    if(alarm > 0)
    {
      if(FALSE == UplinkProtocol_IsReporting())
      {
        DevicePara.Mode.AlarmReport = WAKE;
        DevicePara.Event.AlarmReport = PEND;  
      }
    }
  }
}
/*******************************************************************************
 Function:      //
 Description:   //����2
                //��ת����4��״̬������תˮ����1
                //������10��ֻ��⵽����2���壬�����1����������
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void Pulse_Calculate2(void)
{
  uint8_t alarm = 0;
  
  if(Pulse_Cal2_Data() == RESET)        //����2�½���
  {
    Pulse.ForwardStatus++;              //��ת״̬2
    if(Pulse.ForwardStatus == PulseStatus2)
    {
      DevicePara.ForwardFlow.flow32++;      //��תˮ����1
    }
    Pulse.ForwardStatus = PulseIdle;
    
    Pulse.Sensor2LostTimes = 0;
    if(DevicePara.SensorError == SENSOR2)
    {
      DevicePara.SensorError = NORMAL;
    }
   
    alarm += Pulse_Sensor1Check();
    alarm += Pulse_MgtIfrCheck();
    if(alarm > 0)
    {
      if(FALSE == UplinkProtocol_IsReporting())
      {
        DevicePara.Mode.AlarmReport = WAKE;
        DevicePara.Event.AlarmReport = PEND;  
      }
    }
  }
}
/*******************************************************************************
 Function:      //
 Description:   //������1���
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
uint8_t Pulse_Sensor1Check(void)
{
  Pulse.Sensor1LostTimes++;
  if(Pulse.Sensor1LostTimes > 10)
  {
    DevicePara.SensorError = SENSOR1;
    if(DevicePara.AlarmMark.Sensor1 == 0)  //�״ι��ϣ��ϱ�
    {
      DevicePara.AlarmMark.Sensor1 = 1;
      return 1;
    }
  }
  
  return 0;
}
/*******************************************************************************
 Function:      //
 Description:   //������2���
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
uint8_t Pulse_Sensor2Check(void)
{
  Pulse.Sensor2LostTimes++;
  if(Pulse.Sensor2LostTimes > 10)
  {
    DevicePara.SensorError = SENSOR2;
    if(DevicePara.AlarmMark.Sensor2 == 0)  //�״ι��ϣ��ϱ�
    {
      DevicePara.AlarmMark.Sensor2 = 1;
      return 1;
    }
  }
  
  return 0;
}
/*******************************************************************************
 Function:      //
 Description:   //�Ÿ��ż�� magnetic interference
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
uint8_t Pulse_MgtIfrCheck(void)
{
  uint8_t data = 0;

  data = GPIO_ReadInputData(GPIOD)&0x03;
  if(data == 0)
  {
    DevicePara.SensorError = INTERFERE;
    if(DevicePara.AlarmMark.Interfere == 0)  //�״ι��ϣ��ϱ�
    {
      DevicePara.AlarmMark.Interfere = 1;
      return 1;
    }
  }
  else
  {
    if(DevicePara.SensorError == INTERFERE)
    {
      DevicePara.SensorError = NORMAL;
    }
  }
  
  return 0;
}
/*******************************************************************************
 Function:      //
 Description:   //�ֶ�����
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void Pulse_ManualWake(void)
{
  uint32_t counter = 0;
  if(Pulse_Weak_Data() == RESET)
  {
    DevicePara.Mode.ManualWake = WAKE;
    SoftTimer_CreateTimer(ONCE,20000,
                          Pulse_ManualEnd);   
  }
  else
  {
    counter = SoftTimer_GetCounter(Pulse_ManualEnd);
    if( (counter > 0)&&(counter <= 5000) )              //0-5s�ڵ͵�ƽ�����ϱ�
    {
      Pulse_ManualReport();
    }
    else if( (counter > 5000)&&(counter <= 10000) )     //5-10s�͵�ƽ�������
    {
      Pulse_ManualDebug();
    }
    else if( (counter > 15000)&&(counter < 20000) )     //15-20s�͵�ƽ���λ
    {
      Pulse_ManualReset();
    }

    Pulse_ManualEnd();
  }
}            
/*******************************************************************************
 Function:      //
 Description:   //�ֶ��ϱ�
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void Pulse_ManualReport(void)
{
  if(FALSE == UplinkProtocol_IsReporting())
  {
    DevicePara.Mode.ManualReport = WAKE;
    DevicePara.Event.ManualReport = PEND;
  }  
}  
/*******************************************************************************
 Function:      //
 Description:   //�ֶ�����
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void Pulse_ManualDebug(void)
{
  if(DevicePara.Mode.ManualDebug == SLEEP)
  {
    DevicePara.Mode.ManualDebug = WAKE;
    DevicePara.Event.ManualDebug = PEND;
  }
} 
/*******************************************************************************
 Function:      //
 Description:   //�ֶ���λ
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void Pulse_ManualReset(void)
{
  DevicePara.Mode.ManualRst = WAKE;
  DevicePara.Event.ManualRst = PEND;
}    
/*******************************************************************************
 Function:      //
 Description:   //�ֶ���λ
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void Pulse_ManualEnd(void)
{
  SoftTimer_DeleteTimer(Pulse_ManualEnd);
  DevicePara.Mode.ManualWake = SLEEP;
}     
/*************************************END**************************************/

