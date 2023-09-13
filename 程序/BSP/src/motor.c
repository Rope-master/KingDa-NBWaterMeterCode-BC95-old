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
#include "motor.h"
/*******************************************************************************
����������
*******************************************************************************/
/*******************************************************************************
��������������
*******************************************************************************/
Motor_TypeDef Motor;
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
/*******************************************************************************
���ܴ��붨����
*******************************************************************************/
/*******************************************************************************
 Function:      //
 Description:   //�����ʼ��
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void Motor_Init(void)
{  
  GPIO_Init(Motor_PWR_PORT, Motor_PWR_Pin, Motor_PWR_Mode);     // �����Դ
  GPIO_Init(Motor_IA_PORT, Motor_IA_Pin, Motor_IA_Mode);        // ������� A
  GPIO_Init(Motor_IB_PORT, Motor_IB_Pin, Motor_IB_Mode);        // ������� B
  
  TIM2_OC1Init(TIM2_OCMode_PWM1 , 
               TIM2_OutputState_Enable , 
               0 , 
               TIM2_OCPolarity_High , 
               TIM2_OCIdleState_Reset);
  TIM2_OC1PreloadConfig(ENABLE);
  TIM2_SetCompare1(0);
  
  TIM2_OC2Init(TIM2_OCMode_PWM1 , 
               TIM2_OutputState_Enable , 
               0 , 
               TIM2_OCPolarity_High , 
               TIM2_OCIdleState_Reset);
  TIM2_OC2PreloadConfig(ENABLE);
  TIM2_SetCompare2(0);
  
  TIM2_CtrlPWMOutputs(ENABLE);

  Motor_ExtiInit();
}
/*******************************************************************************
 Function:      //
 Description:   //���ȡ����ʼ��
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void Motor_DeInit(void)
{
  Motor_ExtiDeInit();
  
  Motor_PWR_Rst;        //�����Դ��
  Motor_IA_Rst;         //������� A �õ�
  Motor_IB_Rst;         //������� B �õ�
  
  TIM2_SetCompare1(0);
  TIM2_SetCompare2(0);
  TIM2_CtrlPWMOutputs(DISABLE);
  

  Motor.State = DISABLE;
  Motor.Direction = MotorIdle;
}
/*******************************************************************************
 Function:      //
 Description:   //����ⲿ�жϳ�ʼ��
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void Motor_ExtiInit(void)
{
  disableInterrupts();  //�����ж�
   
  GPIO_Init(Motor_OL_PORT,Motor_OL_Pin, Motor_OL_Mode);         // ���ſ���λ��־
  GPIO_Init(Motor_CL_PORT, Motor_CL_Pin, Motor_CL_Mode);        // ���Ź���λ��־
  EXTI_SelectPort(EXTI_Port_B);                                 //ѡ��B�˿�              
  EXTI_SetHalfPortSelection(EXTI_HalfPort_B_MSB ,ENABLE);       //ѡ��PB(7:4)
  EXTI_SetPortSensitivity(EXTI_Port_B,EXTI_Trigger_Falling);    //GPIOB�ӿ��½����ж�
  EXTI_ClearITPendingBit (EXTI_IT_PortB);                       //���жϱ�־λ
  ITC_SetSoftwarePriority(EXTIB_G_IRQn, ITC_PriorityLevel_2);   //�ж����ȼ��趨
  
  /*�Ƚ�����ʼ��*/
  COMP_DeInit();
  CLK_PeripheralClockConfig(CLK_Peripheral_COMP, ENABLE);
  SYSCFG_RIIOSwitchConfig(RI_IOSwitch_24, ENABLE);
  COMP_Init(COMP_InvertingInput_3_4VREFINT,COMP_OutputSelect_TIM1OCREFCLR,COMP_Speed_Fast);
  COMP_TriggerConfig(COMP_TriggerGroup_NonInvertingInput,COMP_TriggerPin_0,ENABLE);
  COMP_EdgeConfig(COMP_Selection_COMP2,COMP_Edge_Rising);
  COMP_ITConfig(COMP_Selection_COMP2,ENABLE);
  COMP_ClearITPendingBit(COMP_Selection_COMP2);
  COMP_ClearFlag(COMP_Selection_COMP2);
  
  enableInterrupts();   //�����ж�
}
/*******************************************************************************
 Function:      //
 Description:   //ȡ����ʼ��ΪĬ�ϸ�λ״̬
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void Motor_ExtiDeInit(void)
{
  GPIO_Init(Motor_OL_PORT, Motor_OL_Pin, GPIO_Mode_Out_PP_Low_Slow);    // ���ſ���λ��־
  GPIO_Init(Motor_CL_PORT, Motor_CL_Pin, GPIO_Mode_Out_PP_Low_Slow);    // ���Ź���λ��־
  
  COMP_ITConfig(COMP_Selection_COMP2,DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_COMP, DISABLE);              //�رձȽ���
  COMP_DeInit();
}
/*******************************************************************************
 Function:      //
 Description:   //�������
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void Motor_Run(void)
{
  unsigned short ccr2;
  
  if(Motor.State != DISABLE)    //�жϵ�ǰ�������
  {
    if(Motor.Direction == CW)   //��������  
    {
     ccr2 = TIM2_GetCapture1();
     if(ccr2<1000)
     {
       TIM2_SetCompare1(ccr2+1);
       TIM2_SetCompare2(0);
     }
    }
    else                        //�ط�����
    {
     ccr2 = TIM2_GetCapture2();
     if(ccr2<1000)
     {
       TIM2_SetCompare2(ccr2+1);
       TIM2_SetCompare1(0);
     }
    }
  }
}
//******************************************************************************
// Function:      //
// Description:   //�����λ�ж�
// Input:         //
// Output:        //
// Return:        //
// Others:        //
//*****************************************************************************/
void Motor_LimitIT(void)
{
  if(Motor.State != DISABLE)    //�������
  {
    if(Motor_OL_Data == RESET)         //����λ�͵�ƽ
    {
      if(Motor.Direction == CW)         //����ת������λ��Ч
      {
        ValveCtrl_FullOpenHandler();
      }
    }
    else if(Motor_CL_Data == RESET)     //����λ�͵�ƽ
    {
      if(Motor.Direction == CCW)        //����ת������λ��Ч
      {
        ValveCtrl_FullCloseHandler();
      }
    }
  }
}
/*******************************************************************************
 Function:      //
 Description:   //���OCP�ж�
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void Motor_OCPIT(void)
{
  if(Motor.State != DISABLE)
  {
    ValveCtrl_OCPHandler();
    SoftTimer_DeleteTimer(ValveCtrl_TimeoutCallback);
  }
}
/*************************************END**************************************/