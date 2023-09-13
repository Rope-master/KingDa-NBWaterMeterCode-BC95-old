/*********************************************************************************
Copyright: 
File name: usart.c
Description: ������غ���������
Author: ��ѧ��cy
Version: 
Date: 
History:       
*******************************************************************************/
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include "usart.h"
/*******************************************************************************
����������
*******************************************************************************/
/*******************************************************************************
��������������
*******************************************************************************/
USART1_TypeDef  Usart1;
USART2_TypeDef  Usart2;
USART3_TypeDef  Usart3;
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
 Description:   //����1���ú���
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void USART1_Config(uint32_t BaudRate)
{
  GPIO_Init(GPIOC, GPIO_Pin_3 , GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOC, GPIO_Pin_2 , GPIO_Mode_In_PU_No_IT);
  
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
  USART_DeInit(USART1); 
  USART_Init(USART1, 
             BaudRate, 
             USART_WordLength_8b, 
             USART_StopBits_1,
             USART_Parity_No, 
             USART_Mode_TxRx);
  
  USART_IrDAConfig(USART1, USART_IrDAMode_Normal);
  USART_IrDACmd(USART1, ENABLE);
  
  USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);
  USART_Cmd(USART1, ENABLE);

  
  
  ITC_SetSoftwarePriority(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQn, ITC_PriorityLevel_2);    //����1�������ȼ�
  ITC_SetSoftwarePriority(USART1_RX_TIM5_CC_IRQn, ITC_PriorityLevel_2);                 //����1�������ȼ�
}
/*******************************************************************************
 Function:      //
 Description:   //����2���ú���
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void USART2_Config(uint32_t BaudRate)
{
  GPIO_Init(GPIOE, GPIO_Pin_4 , GPIO_Mode_Out_PP_High_Slow);
  GPIO_Init(GPIOE, GPIO_Pin_3 , GPIO_Mode_In_PU_No_IT);
  
  CLK_PeripheralClockConfig(CLK_Peripheral_USART2, ENABLE);
  USART_DeInit(USART2); 
  USART_Init(USART2, 
             BaudRate, 
             USART_WordLength_8b, 
             USART_StopBits_1,
             USART_Parity_No, 
             USART_Mode_TxRx);
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
  USART_Cmd(USART2, ENABLE);
  
  ITC_SetSoftwarePriority(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQn, ITC_PriorityLevel_2);    //����2�������ȼ�
  ITC_SetSoftwarePriority(TIM2_CC_USART2_RX_IRQn, ITC_PriorityLevel_2);                 //����2�������ȼ�
}
/*******************************************************************************
 Function:      //
 Description:   //����3���ú���
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void USART3_Config(uint32_t BaudRate)
{
  GPIO_Init(GPIOE, GPIO_Pin_6 , GPIO_Mode_Out_PP_High_Slow);
  GPIO_Init(GPIOE, GPIO_Pin_7 , GPIO_Mode_In_PU_No_IT);
  
  CLK_PeripheralClockConfig(CLK_Peripheral_USART3, ENABLE);
  USART_DeInit(USART3);
  USART_Init(USART3,
             BaudRate,
             USART_WordLength_8b,
             USART_StopBits_1,
             USART_Parity_No,
             USART_Mode_TxRx);    
  USART_ITConfig(USART3,USART_IT_RXNE, ENABLE);
  USART_Cmd(USART3,ENABLE);
  
  ITC_SetSoftwarePriority(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQn, ITC_PriorityLevel_2);    //����3�������ȼ�
  ITC_SetSoftwarePriority(TIM3_CC_USART3_RX_IRQn, ITC_PriorityLevel_2);                 //����3�������ȼ�
}
/*******************************************************************************
 Function:      //
 Description:   //����1�����жϷ�����
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void USART1_RX_IRQHandler(void)                                                   
{
  if(USART_GetITStatus(USART1,USART_IT_RXNE)!= RESET)//USART_IT_RXNE�������ж�  
  {
    if(Usart1.RxPend == FALSE)//�жϵ�ǰ�����ѱ�ȡ��
    {
      if(Usart1.RxBusy == FALSE)//�ж�Ϊ�״νӰ�
      {
        Usart1.RxBusy = TRUE;//��־����æ
        Usart1.RxLength = 0;//��������
        USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);            //�������߿����ж�
      }
      Usart1.RxBuffer[Usart1.RxLength] = USART_ReceiveData8(USART1);//��������
      Usart1.RxLength++;
      if(Usart1.RxLength == USART1_RX_MAX)//�������ݳ����������򴮿���ǰ����
      {
        Usart1.RxBusy = FALSE;//��ս���æ
        Usart1.RxPend = TRUE;//��־���ڹ���
      }
    }
    else
    {
      USART_ReceiveData8(USART1);//��������,��ֹ�������һֱ���ж�
    }
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }
  else if(USART_GetITStatus(USART1,USART_IT_IDLE)!= RESET)//������߿���
  {
    if(Usart1.RxPend == FALSE)//�жϵ�ǰ�����ѱ�ȡ��
    {
      Usart1.RxBusy = FALSE;//��ս���æ
      Usart1.RxPend = TRUE;//��־���ڹ���
      USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);            //��ֹ���߿����ж�
    }
    USART_ClearITPendingBit (USART1, USART_IT_IDLE);
  }
}
/*******************************************************************************
 Function:      //
 Description:   ////����1�����жϷ�����
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void USART1_TX_IRQHandler(void)                                                   
{
  if(USART_GetITStatus(USART1,USART_IT_TC)!= RESET)//�жϷ������
  {
    if(Usart1.TxBusy != FALSE)//�ж��Ƿ�Ϊ����æ״̬����ֹ�������ж�
    {
      if(Usart1.TxLength != Usart1.TcLength)//�ж� δ�������
      {
        USART_SendData8(USART1,Usart1.TxBuffer[Usart1.TcLength]);//�����ѷ����ֽڷ�����һ���ֽ�
        Usart1.TcLength++;     
      }
      else //�ѷ������
      {
        USART_ITConfig(USART1,USART_IT_TC,DISABLE); //�رշ����ж�
        Usart1.TxBusy = FALSE;//���æ��־
        #ifdef USART1_HALF_DUPLEX                                      //����ǰ�˫����������
        USART1_RX_ENABLE;
        #endif
      }
    }
    USART_ClearITPendingBit (USART1, USART_IT_TC);//�����ر�־
  }
  USART_ClearITPendingBit (USART1, USART_IT_TXE);//�����ر�־
}
/*******************************************************************************
 Function:      //
 Description:   //����1���ͺ���
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
ErrorStatus USART1_TX(unsigned char *buff,unsigned short length)
{
  ErrorStatus err;  //���巵��ֵ
  u16 i = 0;

  if(Usart1.TxBusy == FALSE)                                  //����Ƿ�æ
  {
  #ifdef USART1_HALF_DUPLEX                                      //����ǰ�˫����رս���
    USART1_RX_DISABLE;
  #endif
    USART_ITConfig(USART1,USART_IT_TC,DISABLE);                  //�رշ����ж�
    
    if(length > USART1_TX_MAX)//���������������ֻ�ܰ�����󻺳巢��
      Usart1.TxLength = USART1_TX_MAX;
    else
      Usart1.TxLength = length;//���淢�ͳ���
    for(i=0;i<Usart1.TxLength;i++)
      Usart1.TxBuffer[i] = buff[i];
    Usart1.TcLength = 0;          //�ѷ��͵ĳ�������
    Usart1.TxBusy = TRUE;      //��־æ
    
    USART_ITConfig(USART1,USART_IT_TC,ENABLE);                  //�򿪷����ж�
    USART_SendData8 (USART1,Usart1.TxBuffer[0]);
    Usart1.TcLength++;
  }
  else
    err = ERROR;                                                //��ʾ���ʹ���
  return err; 
}
//void USART1_TX(void)
//{
//  Usart1.TcLength = 0;                                 //�ѷ��͵ĳ�������
//  Usart1.TxBusy = TRUE;                                //��־æ
//  USART_ITConfig(USART1,USART_IT_TC,ENABLE);            //�򿪷����ж�
//  USART_SendData8(USART1,Usart1.TxBuffer[0]);          //�������ֽ�
//  Usart1.TcLength++;
//}
/*******************************************************************************
 Function:      //
 Description:   //����1���պ���
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
unsigned short USART1_RX(unsigned char *buff)
{
  unsigned short R_Len,i;
  if(Usart1.RxPend != FALSE)
  {
    for(i=0;i<Usart1.RxLength;i++)
      buff[i] = Usart1.RxBuffer[i];
    Usart1.RxPend = FALSE;
    R_Len = Usart1.RxLength;
    
    memset(Usart1.RxBuffer,'\0',Usart1.RxLength);//����ջ�����
  }
  else
    R_Len = 0;
  return R_Len;
}
/*******************************************************************************
 Function:      //
 Description:   //����1��պ���
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
void USART1_Free(void)
{
  Usart1.TcLength = 0;                                //��մ���1���ͳ���
  Usart1.RxLength = 0;                             //��մ���1���ճ���
  Usart1.TxBusy = FALSE;                              //��մ���1����æ��־  
  Usart1.RxBusy = FALSE;                           //��մ���1����æ
  Usart1.RxPend = FALSE;                           //��մ���1����
}
/*******************************************************************************
 Function:      //
 Description:   //����2�����жϷ�����
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void USART2_RX_IRQHandler(void)                                                   
{
  if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET)//USART_IT_RXNE�������ж�  
  {
    if(Usart2.RxPend == FALSE)//�жϵ�ǰ�����ѱ�ȡ��
    {
      if(Usart2.RxBusy == FALSE)//�ж�Ϊ�״νӰ�
      {
        Usart2.RxBusy = TRUE;//��־����æ
        Usart2.RxLength = 0;//��������
        USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);            //�������߿����ж�
      }
      Usart2.RxBuffer[Usart2.RxLength] = USART_ReceiveData8 (USART2);//��������
      Usart2.RxLength++;
      if(Usart2.RxLength == USART2_RX_MAX)//�������ݳ����������򴮿���ǰ����
      {
        Usart2.RxBusy = FALSE;//��ս���æ
        Usart2.RxPend = TRUE;//��־���ڹ���
      }
    }
    else
    {
      USART_ReceiveData8(USART2);//��������,��ֹ�������һֱ���ж�
    }
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }
  else if(USART_GetITStatus(USART2,USART_IT_IDLE)!= RESET)//������߿���
  {
    if(Usart2.RxPend == FALSE)//�жϵ�ǰ�����ѱ�ȡ��
    {
      Usart2.RxBusy = FALSE;//��ս���æ
      Usart2.RxPend = TRUE;//��־���ڹ���
      USART_ITConfig(USART2, USART_IT_IDLE, DISABLE);            //��ֹ���߿����ж�
    }
    USART_ClearITPendingBit (USART2, USART_IT_IDLE);
  }
  else if(USART_GetITStatus(USART2,USART_IT_OR)!= RESET)//USART_IT_OR������ж�  
  {
    USART_ReceiveData8 (USART2);//��������
    USART_ClearITPendingBit(USART2, USART_IT_OR);
  }
  
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void USART2_TX_IRQHandler(void)                                                   //����2�����жϷ�����
{
  if(USART_GetITStatus(USART2,USART_IT_TC)!= RESET)//�жϷ������
  {
    if(Usart2.TxBusy != FALSE)//�ж��Ƿ�Ϊ����æ״̬����ֹ�������ж�
    {
      if(Usart2.TxLength != Usart2.TcLength)//�ж� δ�������
      {
        USART_SendData8 (USART2,Usart2.TxBuffer[Usart2.TcLength]);//�����ѷ����ֽڷ�����һ���ֽ�
        Usart2.TcLength++;     
      }
      else //�ѷ������
      {
        USART_ITConfig(USART2,USART_IT_TC,DISABLE); //�رշ����ж�
        Usart2.TxBusy = FALSE;//���æ��־
        #ifdef Usart2_Half_Duplex                                      //����ǰ�˫����������
        Usart2_RX_ENABLE;
        #endif
      }
    }
    USART_ClearITPendingBit (USART2, USART_IT_TC);//�����ر�־
  }

  USART_ClearITPendingBit (USART2, USART_IT_TXE);//�����ر�־
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
//ErrorStatus USART2_TX(unsigned char *buff,unsigned short length)
//{
//  ErrorStatus err;  //���巵��ֵ
//  u16 i = 0;
//  
//  Usart2.TxBusy = FALSE;
//
//  if(Usart2.TxBusy == FALSE)                                  //����Ƿ�æ
//  {
//  #ifdef Usart2_Half_Duplex                                      //����ǰ�˫����رս���
//    Usart2_RX_DISABLE;
//  #endif
//    USART_ITConfig(USART2,USART_IT_TC,ENABLE);                  //�򿪷����ж�
//    if(length > USART2_TX_MAX)//���������������ֻ�ܰ�����󻺳巢��
//      Usart2.TxLength = USART2_TX_MAX;
//    else
//      Usart2.TxLength = length;//���淢�ͳ���
//    for(i=0;i<Usart2.TxLength;i++)
//      Usart2.TxBuffer[i] = buff[i];
//    Usart2.TcLength = 0;          //�ѷ��͵ĳ�������
//    Usart2.TxBusy = TRUE;      //��־æ
//    USART_SendData8(USART2,Usart2.TxBuffer[0]);
//    Usart2.TcLength++;
//  }
//  else
//    err = ERROR;                                                //��ʾ���ʹ���
//  return err; 
//}
void USART2_TX(void)
{
  Usart2.TcLength = 0;                                 //�ѷ��͵ĳ�������
  Usart2.TxBusy = TRUE;                                //��־æ
  USART_ITConfig(USART2,USART_IT_TC,ENABLE);            //�򿪷����ж�
  USART_SendData8(USART2,Usart2.TxBuffer[0]);          //�������ֽ�
  Usart2.TcLength++;
}
/*******************************************************************************
 Function:      //
 Description:   //��ѯ����
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
//void USART2_TX_IQ(unsigned char *buff,unsigned short length)
//{
//  unsigned char i = 0;
//  unsigned short timeout = 0xFFFF;
//  
//  while(i < length)
//  {
//    USART_SendData8(USART2,*buff++);
//    while((USART_GetFlagStatus(USART2,USART_FLAG_TC))==RESET && (timeout != 0))
//    {timeout--;}
//    i++;
//  }
//}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
uint16_t USART2_RX(uint8_t *buff)
{
  uint16_t R_Len = 0;
  if(Usart2.RxPend != FALSE)
  {
    memcpy(buff,Usart2.RxBuffer,Usart2.RxLength);
    Usart2.RxPend = FALSE;
    R_Len = Usart2.RxLength;
  }
  else
    R_Len = 0;
  return R_Len;
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
void USART2_Free(void)
{
  USART_ITConfig(USART2,USART_IT_TC,DISABLE); //�رշ����ж�
  memset(Usart2.TxBuffer,0,USART2_TX_MAX);
  Usart2.TxLength = 0;                                //��մ���2���ͳ���
  Usart2.TcLength = 0;                                //��մ���2���ͳ���
  Usart2.TxBusy = FALSE;                              //��մ���2����æ��־ 
  memset(Usart2.RxBuffer,0,USART2_RX_MAX);
  Usart2.RxLength = 0;                             //��մ���2���ճ��� 
  Usart2.RxBusy = FALSE;                           //��մ���2����æ
  Usart2.RxPend = FALSE;                           //��մ���2����  
}
/*******************************************************************************
 Function:      //
 Description:   //����3�жϷ�����
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void USART3_RX_IRQHandler(void)                                                   
{    
  if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)//USART_IT_RXNE�������ж�  
  {   
    if(Usart3.RxPend == FALSE)//�жϵ�ǰ�����ѱ�ȡ��
    {
      if(Usart3.RxBusy == FALSE)//�ж�Ϊ�״νӰ�
      {
        Usart3.RxBusy = TRUE;//��־����æ
        Usart3.RxLength = 0;//��������
        USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);            //�������߿����ж�
      }
      Usart3.RxBuffer[Usart3.RxLength] = USART_ReceiveData8(USART3);//��������
      Usart3.RxLength++;
      if(Usart3.RxLength == USART3_RX_MAX)//�������ݳ����������򴮿���ǰ����
      {
        Usart3.RxBusy = FALSE;//��ս���æ
        Usart3.RxPend = TRUE;//��־���ڹ���
      }
    }
    USART_ClearITPendingBit (USART3, USART_IT_RXNE);
  }
  else if(USART_GetITStatus(USART3,USART_IT_IDLE) != RESET)//������߿���
  {
    if(Usart3.RxPend == FALSE)//�жϵ�ǰ�����ѱ�ȡ��
    {
      Usart3.RxBusy = FALSE;//��ս���æ
      Usart3.RxPend = TRUE;//��־���ڹ���
      USART_ITConfig(USART3, USART_IT_IDLE, DISABLE);            //��ֹ���߿����ж�
    }
    USART_ClearITPendingBit (USART3, USART_IT_IDLE);
  }
}
/*******************************************************************************
 Function:      //
 Description:   //����3�����жϷ�����
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void USART3_TX_IRQHandler(void)                                                   
{
  if(USART_GetITStatus(USART3,USART_IT_TC)!= RESET)//�жϷ������
  {
    if(Usart3.TxBusy != FALSE)//�ж��Ƿ�Ϊ����æ״̬����ֹ�������ж�
    {
      if(Usart3.TxLength != Usart3.TcLength)//�ж� δ�������
      {
        USART_SendData8 (USART3,Usart3.TxBuffer[Usart3.TcLength]);//�����ѷ����ֽڷ�����һ���ֽ�
        Usart3.TcLength++;     
      }
      else //�ѷ������
      {
        USART_ITConfig(USART3,USART_IT_TC,DISABLE); //�رշ����ж�
        Usart3.TxBusy = FALSE;//���æ��־
        #ifdef Usart3_Half_Duplex                                      //����ǰ�˫����������
        Usart3_RX_ENABLE;
        #endif
      }
    }
    USART_ClearITPendingBit (USART3, USART_IT_TC);//�����ر�־
  }
  USART_ClearITPendingBit (USART3, USART_IT_TXE);//�����ر�־
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
ErrorStatus USART3_TX(uint8_t *buff,uint16_t length)
{
  ErrorStatus err;  //���巵��ֵ
 
  Usart3.TxBusy = FALSE;
  
  if(Usart3.TxBusy == FALSE)                                  //����Ƿ�æ
  {
  #ifdef Usart3_Half_Duplex                                      //����ǰ�˫����رս���
    Usart3_RX_DISABLE;
  #endif
    USART_ITConfig(USART3,USART_IT_TC,ENABLE);                //������3�����жϡ�  
    if(length > USART3_TX_MAX)//���������������ֻ�ܰ�����󻺳巢��
      Usart3.TxLength = USART3_TX_MAX;
    else
      Usart3.TxLength = length;//���淢�ͳ���
    memcpy(Usart3.TxBuffer,buff,Usart3.TxLength);
    Usart3.TcLength = 0;          //�ѷ��͵ĳ�������
    Usart3.TxBusy = TRUE;      //��־æ
    USART_SendData8(USART3,Usart3.TxBuffer[0]);
    Usart3.TcLength++;
  }
  else
    err = ERROR;                                                //��ʾ���ʹ���
  return err;
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
uint16_t USART3_RX(uint8_t *buff)
{
  uint16_t R_Len = 0;
  if(Usart3.RxPend != FALSE)
  {
    memcpy(buff,Usart3.RxBuffer,Usart3.RxLength);
    Usart3.RxPend = FALSE;
    R_Len = Usart3.RxLength;
  }
  else
    R_Len = 0;
  return R_Len;
}
/*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void USART3_Free(void)
{
  USART_ITConfig(USART3,USART_IT_TC,DISABLE); //�رշ����ж�
  memset(Usart3.TxBuffer,0,USART3_TX_MAX);
  Usart3.TxLength = 0;                                //��մ���2���ͳ���
  Usart3.TcLength = 0;                                //��մ���2���ͳ���
  Usart3.TxBusy = FALSE;                              //��մ���2����æ��־ 
  memset(Usart3.RxBuffer,0,USART3_RX_MAX);
  Usart3.RxLength = 0;                             //��մ���2���ճ��� 
  Usart3.RxBusy = FALSE;                           //��մ���2����æ
  Usart3.RxPend = FALSE;                           //��մ���2����
}
/*************************************END**************************************/

