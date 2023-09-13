/*******************************************************************************
Copyright: 
File name: valveCtrl.h
Description: ���ſ�����غ���������
Author: ��ѧ��cy
Version: 
Date: 
History: 
*******************************************************************************/
#ifndef __VALVE_CTRL_H__
#define __VALVE_CTRL_H__
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include "include.h"
/*******************************************************************************
�궨����
*******************************************************************************/
#define FULL_CTRL_TIME  10000           //ȫ����ʱʱ��
#define CWHALF_CTRL_TIME  4000          //��ת�뿪��ʱʱ��
#define CCWHALF_CTRL_TIME  3000         //��ת�뿪��ʱʱ��
/*******************************************************************************
�������Ͷ���
*******************************************************************************/
typedef enum{
  IdleCmd,
  FullOpenCmd,
  HalfOpenCmd,
  FullCloseCmd,
  MaintainCmd
}ValveCtrlCmd;
  
typedef enum{
  UnknownStatus,
  FullOpenStatus,
  HalfOpenStatus,
  FullCloseStatus,
  OCPStatus
}ValveCtrlStatus;

typedef struct 
{
  ValveCtrlStatus Status;
  ValveCtrlCmd LastCmd;
  ValveCtrlCmd NextCmd;
}ValveCtrl_TypeDef;  
/*******************************************************************************
�ⲿ����������
*******************************************************************************/
extern ValveCtrl_TypeDef ValveCtrl;
/*******************************************************************************
����������
*******************************************************************************/
uint8_t ValveCtrl_GetStatus(void);
void ValveCtrl_Init(void);
void ValveCtrl_DeInit(void);
void ValveCtrl_Process(uint8_t cmd);
void ValveCtrl_FullOpen(void);
void ValveCtrl_HalfOpen(void);
void ValveCtrl_FullClose(void);
void ValveCtrl_Maintain(void);
void ValveCtrl_TimeoutCallback(void);
void ValveCtrl_FullOpenHandler(void);
void ValveCtrl_FullCloseHandler(void);
void ValveCtrl_OCPHandler(void);
/******************************************************************************/
#endif
/*************************************END**************************************/