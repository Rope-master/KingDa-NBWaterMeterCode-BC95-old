/*******************************************************************************
Copyright: 
File name: soft_timer.c
Description: ��ʱ����غ���������
Author: ��ѧ��cy
Version: 
Date: 
History:       
*******************************************************************************/
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include "soft_timer.h"  
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
NEAR static SoftTimer_TypeDef SoftTimer[SOFT_TIMER_MAX];
uint32_t DelayCounter_ms;             //��ʱ������
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
 Description:   //��ʱ������
                //���ҹ���Ķ�ʱ����ִ�лص�����
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
void SoftTimer_Process(void)
{
  unsigned char i = 0;
  for(i=0;i<SOFT_TIMER_MAX;i++)
  {
    if(SoftTimer[i].Pend != FALSE)                      
    {
      SoftTimer[i].Pend = FALSE;
      SoftTimer[i].Callback();
    }
  }
}
/*******************************************************************************
 Function:      //
 Description:   //�δ��жϷ�����
                //�ж϶�ʱ����ʱ���ѵ�������
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
void SoftTimer_SysTickHandler(void)                                                     
{
  uint8_t i = 0;
  for(i = 0;i < SOFT_TIMER_MAX;i++)
  {
    if(SoftTimer[i].State != DISABLE)   //�ж������Ķ�ʱ��
    {
      if(SoftTimer[i].Counter >= SoftTimer[i].Duration)
      {
        switch(SoftTimer[i].Mode)
        {
        case ONCE:              //����ִ��
          {
            SoftTimer[i].State = DISABLE;       //��ʱ���ر�
          }
          break;
        case CONTINUOUS:        //����ִ��
          {
            //һֱ����
          }
          break;
        default:
          break;     
        }
        SoftTimer[i].Pend = TRUE;
        SoftTimer[i].Counter = 0;
      }
      else
      {
        SoftTimer[i].Counter++;
      }
    }
  }
}
/*******************************************************************************
 Function:      //
 Description:   //������ʱ����
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
ErrorStatus SoftTimer_CreateTimer(SoftTimer_Mode_TypeDef mode,uint32_t duration,void (*Callback)(void))
{
  ErrorStatus err = SUCCESS;
  uint8_t i = 0;
  
  SoftTimer_DeleteTimer(Callback);     //����֮ǰ��ɾ����ͬ��ʱ����
  for(i = 0;i < SOFT_TIMER_MAX;i++)
  {
    if( (SoftTimer[i].State == DISABLE)&&(SoftTimer[i].Pend == FALSE) )   //�ҵ�һ�����еĶ�ʱ��
    {
      SoftTimer[i].State = ENABLE;
      SoftTimer[i].Pend = FALSE;
      SoftTimer[i].Mode = mode;
      SoftTimer[i].Duration = duration;
      SoftTimer[i].Callback = Callback;
      SoftTimer[i].Counter = 0;
      break;
    }
  }
  if(i == SOFT_TIMER_MAX)               //��ʱ�����
    err = ERROR;
  return err;
}
/*******************************************************************************
 Function:      //
 Description:   //ɾ����ʱ����
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
ErrorStatus SoftTimer_DeleteTimer(void (*Callback)(void))
{
  ErrorStatus err = ERROR;
  uint8_t i = 0;
  for(i=0;i < SOFT_TIMER_MAX;i++)
  {
    if(SoftTimer[i].Callback == Callback)
    {
      SoftTimer[i].State = DISABLE;
      SoftTimer[i].Pend = FALSE;
      err = SUCCESS;
      break;
    }
  }
  return err;
}
/*******************************************************************************
 Function:      //
 Description:   //��ȡ��ʱ������ֵ
 Input:         //Call_Back:�ص�����
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
uint32_t SoftTimer_GetCounter(void (*Callback)(void))
{
  uint32_t value = 0;
  uint8_t i = 0;
  for(i = 0;i < SOFT_TIMER_MAX;i++)
  {
    if( (SoftTimer[i].Callback == Callback)
        &&(SoftTimer[i].State != DISABLE) )
    {
      value = SoftTimer[i].Counter;
      break;
    }
  }
  return value;
}
/*******************************************************************************
 Function:      //
 Description:   //��ȡ��ʱ������ֵ
 Input:         //Call_Back:�ص�����
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
void Delay_ms(uint32_t ms)
{
  DelayCounter_ms = 1;
  while(DelayCounter_ms <= ms);
  DelayCounter_ms = 0;
}
/*************************************END**************************************/
