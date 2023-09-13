/*******************************************************************************
Copyright: 
File name: valveCtrl.c
Description: ���ſ�����غ���������
Author: ��ѧ��cy
Version: 
Date: 
History: 
*******************************************************************************/
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include "valve_ctrl.h"
/*******************************************************************************
����������
*******************************************************************************/
/*******************************************************************************
��������������
*******************************************************************************/
ValveCtrl_TypeDef ValveCtrl;
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
�ڲ���������
*******************************************************************************/
/*******************************************************************************
���ܴ��붨����
*******************************************************************************/
/*******************************************************************************
 Function:      //
 Description:   //��ȡ����״̬
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
uint8_t ValveCtrl_GetStatus(void)
{ 
  if(ValveCtrl.Status == OCPStatus)     //����
  {

  }
  else if( Motor_OL_Data == RESET )     //ȫ��
  {
    ValveCtrl.Status = FullOpenStatus;
  }
  else if( Motor_CL_Data == RESET )     //ȫ��
  {
    ValveCtrl.Status = FullCloseStatus;
  }
  else                                  //�뿪
  {
    ValveCtrl.Status = HalfOpenStatus;
  }
  return (uint8_t)ValveCtrl.Status;
}
/*******************************************************************************
 Function:      //
 Description:   //ȡ����ʼ��ΪĬ�ϸ�λ״̬
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void ValveCtrl_Init(void)
{
  DevicePara.Mode.ValveCtrl = WAKE;
  SoftTimer_CreateTimer(CONTINUOUS, 20000, ValveCtrl_DeInit); 
  Motor_Init();
}
/*******************************************************************************
 Function:      //
 Description:   //ȡ����ʼ��ΪĬ�ϸ�λ״̬
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void ValveCtrl_DeInit(void)
{
  SoftTimer_DeleteTimer(ValveCtrl_DeInit);
  SoftTimer_DeleteTimer(ValveCtrl_TimeoutCallback);
  ValveCtrl.LastCmd = IdleCmd;
  ValveCtrl.NextCmd = IdleCmd;
  ValveCtrl_GetStatus();
  Motor_DeInit();
  
  DevicePara.Mode.ValveCtrl = SLEEP;
  DevicePara.Mode.MaintainValve = SLEEP;
}
/*******************************************************************************
 Function:      //
 Description:   //���ſ��������
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void ValveCtrl_Process(uint8_t cmd)
{
  ValveCtrl_Init();
  ValveCtrl_GetStatus();
  switch(cmd)
  {
  case FullOpenCmd:
    {
      ValveCtrl_FullOpen();
    }
    break;
  case HalfOpenCmd:
    {
      ValveCtrl_HalfOpen();
    }
    break;
  case FullCloseCmd:
    {
      ValveCtrl_FullClose();
    }
    break;
  case MaintainCmd:
    {
      ValveCtrl_Maintain();
    }
    break;
  default:
    {
      ValveCtrl_DeInit();
    }
    break;
  }
}
/*******************************************************************************
 Function:      //
 Description:   //����ȫ��
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void ValveCtrl_FullOpen(void)
{
  if( ValveCtrl.Status == FullOpenStatus )      //���������ȫ�������سɹ�
  {
    ValveCtrl_DeInit();
  }
  else                                          //ȫ��
  {
    Motor.State = ENABLE;
    Motor.Direction = CW;
    SoftTimer_CreateTimer(ONCE, FULL_CTRL_TIME,
                          ValveCtrl_TimeoutCallback);
    
    ValveCtrl.LastCmd = FullOpenCmd;
    ValveCtrl.NextCmd = IdleCmd;       //�޺�������
  }
}
/*******************************************************************************
 Function:      //
 Description:   //���Ű뿪
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void ValveCtrl_HalfOpen(void)
{ 
  if( ValveCtrl.Status == HalfOpenStatus )      //��������Ѱ뿪�����سɹ�
  {
    ValveCtrl_DeInit();
  }
  if( ValveCtrl.Status == FullOpenStatus )      //���������ȫ�����뿪
  {
    Motor.State = ENABLE;
    Motor.Direction = CCW;
    SoftTimer_CreateTimer(ONCE, CCWHALF_CTRL_TIME,
                          ValveCtrl_TimeoutCallback);
    
    ValveCtrl.LastCmd = HalfOpenCmd;
    ValveCtrl.NextCmd = IdleCmd;       //�޺�������
  }
  else if( ValveCtrl.Status == FullCloseStatus )//���������ȫ�أ��뿪
  { 
    Motor.State = ENABLE;
    Motor.Direction = CW;
    SoftTimer_CreateTimer(ONCE, CWHALF_CTRL_TIME,
                          ValveCtrl_TimeoutCallback);
    
    ValveCtrl.LastCmd = HalfOpenCmd;
    ValveCtrl.NextCmd = IdleCmd;       //�޺�������
  }
}
/*******************************************************************************
 Function:      //
 Description:   //����ȫ��
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void ValveCtrl_FullClose(void)
{
  if( ValveCtrl.Status == FullCloseStatus )      //���������ȫ�أ����سɹ�
  {
    ValveCtrl_DeInit();
  }
  else                                          //�ط�
  {
    Motor.State = ENABLE;
    Motor.Direction = CCW;
    SoftTimer_CreateTimer(ONCE, FULL_CTRL_TIME,
                          ValveCtrl_TimeoutCallback); 
    
    ValveCtrl.LastCmd = FullCloseCmd;
    ValveCtrl.NextCmd = IdleCmd;       //�޺�������
  }
}
/*******************************************************************************
 Function:      //
 Description:   //ϴ��
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void ValveCtrl_Maintain(void)
{
  if( ValveCtrl.Status == FullOpenStatus )      //���������ȫ�����Ȱ뿪����ȫ��
  {  
    Motor.State = ENABLE;
    Motor.Direction = CCW;
    SoftTimer_CreateTimer(ONCE, CCWHALF_CTRL_TIME,
                          ValveCtrl_TimeoutCallback);
    
    ValveCtrl.LastCmd = HalfOpenCmd;
    ValveCtrl.NextCmd = FullOpenCmd;    //��һ��ȫ��
  }
  else if(ValveCtrl.Status == FullCloseStatus)  //���������ȫ�أ��Ȱ뿪����ȫ��                    
  {
    Motor.State = ENABLE;
    Motor.Direction = CW;
    SoftTimer_CreateTimer(ONCE, CWHALF_CTRL_TIME,
                          ValveCtrl_TimeoutCallback);
    
    ValveCtrl.LastCmd = HalfOpenCmd;
    ValveCtrl.NextCmd = FullCloseCmd;   //��һ��ȫ��
  }
  else                                          //��������Ѱ뿪����ȫ�أ��ٰ뿪
  {
    Motor.State = ENABLE;
    Motor.Direction = CCW;
    SoftTimer_CreateTimer(ONCE, FULL_CTRL_TIME,
                          ValveCtrl_TimeoutCallback);
    
    ValveCtrl.LastCmd = FullCloseCmd;
    ValveCtrl.NextCmd = HalfOpenCmd;   //��һ���뿪
  }
}
/*******************************************************************************
 Function:      //
 Description:   //���س�ʱ�ص�����
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void ValveCtrl_TimeoutCallback(void)
{
  switch(ValveCtrl.NextCmd)
    {
    case  IdleCmd:              //��������
      {
        ValveCtrl_DeInit();
      }
      break;   
    case FullOpenCmd:           //ȫ��������
      {
        Motor.State = ENABLE;
        Motor.Direction = CW;
        SoftTimer_CreateTimer(ONCE, FULL_CTRL_TIME,
                              ValveCtrl_TimeoutCallback); 
        
        ValveCtrl.LastCmd = FullOpenCmd;
        ValveCtrl.NextCmd = IdleCmd;       //�޺�������
      }
      break;
    case HalfOpenCmd:           //�뿪������
      {
        Motor.State = ENABLE;
        Motor.Direction = CW;
        SoftTimer_CreateTimer(ONCE, CWHALF_CTRL_TIME,
                              ValveCtrl_TimeoutCallback);
        
        ValveCtrl.LastCmd = HalfOpenCmd;
        ValveCtrl.NextCmd = IdleCmd;       //�޺�������
      }
      break;
    case FullCloseCmd:          //ȫ�ط�����
      {
        Motor.State = ENABLE;
        Motor.Direction = CCW;
        SoftTimer_CreateTimer(ONCE, FULL_CTRL_TIME,
                              ValveCtrl_TimeoutCallback); 
        
        ValveCtrl.LastCmd = FullCloseCmd;
        ValveCtrl.NextCmd = IdleCmd;       //�޺�������
      }
      break;
    default:
      break;
    }
}
/*******************************************************************************
 Function:      //
 Description:   //����ȫ��������
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void ValveCtrl_FullOpenHandler(void)
{
  switch(ValveCtrl.NextCmd)
  {
  case  IdleCmd:      //��������
    {
      if(ValveCtrl.LastCmd == FullOpenCmd)
      {
        ValveCtrl_DeInit();
      }
    }
    break;   
  default:
    break;
  }
}
/*******************************************************************************
 Function:      //
 Description:   //����ȫ�ش�����
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void ValveCtrl_FullCloseHandler(void)
{
  switch(ValveCtrl.NextCmd)
  {
  case  IdleCmd:        //��������
    {
      if(ValveCtrl.LastCmd == FullCloseCmd)
      {
        ValveCtrl_DeInit();
      }
    }
    break;   
  case HalfOpenCmd:     //�뿪������
    {
      Motor.State = ENABLE;
      Motor.Direction = CW;
      SoftTimer_CreateTimer(ONCE, CWHALF_CTRL_TIME,
                            ValveCtrl_TimeoutCallback);
      
      ValveCtrl.LastCmd = HalfOpenCmd;
      ValveCtrl.NextCmd = IdleCmd;       //�޺�������
    }
    break;
  default:
    break;
  }
}
/*******************************************************************************
 Function:      //
 Description:   //����ȫ�ش�����
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void ValveCtrl_OCPHandler(void)
{
  ValveCtrl.Status = OCPStatus;
  ValveCtrl_DeInit();
}
/*************************************END**************************************/