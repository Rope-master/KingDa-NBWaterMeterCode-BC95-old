/*******************************************************************************
Copyright: 
File name: pulse.h
Description: ������غ���������
Author: ��ѧ��cy
Version: 
Date: 
History: 
*******************************************************************************/
#ifndef __PULSE_H__
#define __PULSE_H__
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include "include.h"
/*******************************************************************************
�궨����
*******************************************************************************/
#define Pulse_Cal1_Port         GPIOD                                 //�������1
#define Pulse_Cal1_Pin          GPIO_Pin_1
#define Pulse_Cal1_Mode         GPIO_Mode_In_FL_IT
#define Pulse_Cal1_Data()       GPIO_ReadInputDataBit(Pulse_Cal1_Port,Pulse_Cal1_Pin)

#define Pulse_Cal2_Port         GPIOD                                 //�������2
#define Pulse_Cal2_Pin          GPIO_Pin_0
#define Pulse_Cal2_Mode         GPIO_Mode_In_FL_IT
#define Pulse_Cal2_Data()       GPIO_ReadInputDataBit(Pulse_Cal2_Port,Pulse_Cal2_Pin)

#define Pulse_Weak_Port         GPIOF                                 //���廽��
#define Pulse_Weak_Pin          GPIO_Pin_0
#define Pulse_Weak_Mode         GPIO_Mode_In_PU_IT
#define Pulse_Weak_Data()       GPIO_ReadInputDataBit(Pulse_Weak_Port,Pulse_Weak_Pin)
/*******************************************************************************
�������Ͷ���
*******************************************************************************/
/*����״̬ö��*/
typedef enum 
{ 
  PulseIdle,
  PulseStatus1,
  PulseStatus2,
  PulseStatus3,
  PulseStatus4
}Pulse_StatusEnum;
/*����ṹ��*/
typedef struct
{
  Pulse_StatusEnum ForwardStatus;       //��ת״̬
  Pulse_StatusEnum ReverseStatus;       //��ת״̬
  uint8_t Sensor1LostTimes;             //����1��ʧ����
  uint8_t Sensor2LostTimes;             //����2��ʧ����
  uint8_t Sensor3LostTimes;             //����3��ʧ����
}Pulse_Struct;
/*******************************************************************************
�ⲿ����������
*******************************************************************************/
extern Pulse_Struct Pulse;
/*******************************************************************************
����������
*******************************************************************************/
void Pulse_Config(void);
void Pulse_Calculate1(void);
void Pulse_Calculate2(void);
uint8_t Pulse_Sensor1Check(void);
uint8_t Pulse_Sensor2Check(void);
uint8_t Pulse_MgtIfrCheck(void);
void Pulse_ManualWake(void);
void Pulse_ManualReport(void);
void Pulse_ManualDebug(void);
void Pulse_ManualReset(void);
void Pulse_ManualEnd(void);
/******************************************************************************/
#endif
/*************************************END**************************************/
