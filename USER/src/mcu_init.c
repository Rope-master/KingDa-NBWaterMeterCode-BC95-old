/*********************************************************************************
//������
//���ߣ����ƮԾ     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "include.h"
/*********************************************************************************
����������
*********************************************************************************/

/*********************************************************************************
��������������
*********************************************************************************/
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
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void MCU_Config(void)
{
  RCC_Configuration();
  GPIO_Configuration();
  TIM4_Config();
  IWDG_Config();
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void RCC_Configuration(void)                                                   //ʱ�ӳ�ʼ��
{
  CLK_HSICmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);                                         //16M�ٶ�
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_Init(GPIOA, GPIO_Pin_All,GPIO_Mode_Out_PP_Low_Slow); //GPIO�˿����������
  GPIO_Init(GPIOB, GPIO_Pin_All,GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOC, GPIO_Pin_All,GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOD, GPIO_Pin_All,GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOE, GPIO_Pin_All,GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOF, GPIO_Pin_All,GPIO_Mode_Out_PP_Low_Slow); 
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void TIM4_Config(void)
{ 
  TIM4_DeInit();
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4 , ENABLE);              //ʹ�ܶ�ʱ��4ʱ��
  TIM4_TimeBaseInit(TIM4_Prescaler_128 , 125);    //���ö�ʱ��4Ϊ128��Ƶ�����ϼ���������ֵΪ125��Ϊ1����ļ���ֵ
  TIM4_ITConfig(TIM4_IT_Update , ENABLE);         //ʹ�����ϼ�������ж�
  TIM4_ARRPreloadConfig(ENABLE);                  //ʹ�ܶ�ʱ��4�Զ����ع���    
  TIM4_Cmd(ENABLE);                               //������ʱ��4��ʼ����
  ITC_SetSoftwarePriority(TIM4_UPD_OVF_TRG_IRQn, ITC_PriorityLevel_2);  //��ʱ��4�ж����ȼ�
}
/*******************************************************************************
 Function:      //
 Description:   //���Ź�����
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void IWDG_Config(void)  
{ 
  IWDG_Enable();                                //�������Ź�
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //����дԤ��Ƶ�������ؼĴ���
  IWDG_SetPrescaler(IWDG_Prescaler_256);        //����IWDGԤ��Ƶֵ
  IWDG_SetReload(0xFF);                         //��������ֵ1.7s��(255+1)*256/38K = 1.72s
  IWDG_ReloadCounter();                         //���ؼ�����
}
/*******************************************************************************
 Function:      //
 Description:   //����
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void Sleep(void)
{  
  PWR_FastWakeUpCmd(ENABLE);                    //������Դ������Ŀ��ٻ���  
  PWR_UltraLowPowerCmd(ENABLE);                 //ʹ�ܵ�Դ�ĵ͹���ģʽ          
  CLK_HSICmd(DISABLE);                          //�ر��ڲ�����ʱ��

  halt();
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void USART2_Configuration ( void )
{  
  CLK_PeripheralClockConfig(CLK_Peripheral_USART2, ENABLE);
  USART_DeInit(USART2);
  GPIO_Init(GPIOE, GPIO_Pin_3 , GPIO_Mode_In_PU_No_IT);
  GPIO_Init(GPIOE, GPIO_Pin_4 , GPIO_Mode_Out_PP_High_Fast);
  USART_Init(USART2,
             9600,
             USART_WordLength_8b,
             USART_StopBits_1,
             USART_Parity_No,
             USART_Mode_TxRx);    
  USART_ITConfig(USART2,USART_IT_RXNE, ENABLE);//�����������ж�
  USART_Cmd(USART2,ENABLE);
  
  ITC_SetSoftwarePriority(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQn, ITC_PriorityLevel_2);  //����2���ȼ�
  ITC_SetSoftwarePriority(TIM2_CC_USART2_RX_IRQn, ITC_PriorityLevel_2);              //����2���ȼ�
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void USART3_Configuration ( void )
{
  CLK_PeripheralClockConfig(CLK_Peripheral_USART3, ENABLE);
  USART_DeInit(USART3);
  GPIO_Init(GPIOE, GPIO_Pin_7 , GPIO_Mode_In_PU_No_IT);
  GPIO_Init(GPIOE, GPIO_Pin_6 , GPIO_Mode_Out_PP_High_Fast);
  USART_Init(USART3,
             9600,
             USART_WordLength_8b,
             USART_StopBits_1,
             USART_Parity_No,
             USART_Mode_TxRx);    
  USART_ITConfig(USART3,USART_IT_RXNE, ENABLE);//�����������ж�
  USART_Cmd(USART3,ENABLE);
  
  ITC_SetSoftwarePriority(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQn, ITC_PriorityLevel_2);  //����3���ȼ�
  ITC_SetSoftwarePriority(TIM3_CC_USART3_RX_IRQn, ITC_PriorityLevel_3);              //����3���ȼ�
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	    	//
 Others:        //
*********************************************************************************/
//void ITC_Config(void)                                                          //�жϿ���  0��� 3���  
//{
//    ITC_SetSoftwarePriority(TIM4_UPD_OVF_TRG_IRQn, ITC_PriorityLevel_3);        //��ʱ��4������ȼ�
//
//    ITC_SetSoftwarePriority(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQn, ITC_PriorityLevel_2);  //����2���ȼ�
//    ITC_SetSoftwarePriority(TIM2_CC_USART2_RX_IRQn, ITC_PriorityLevel_2);              //����2���ȼ�
//    ITC_SetSoftwarePriority(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQn, ITC_PriorityLevel_1);  //����3���ȼ�
//    ITC_SetSoftwarePriority(TIM3_CC_USART3_RX_IRQn, ITC_PriorityLevel_1);              //����3���ȼ�
//}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void Read_Voltage(void)
{
  unsigned short value = 0;
  float vdd = 0;
  
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);   //����ADCʱ��
  ADC_Init (ADC1,ADC_ConversionMode_Continuous, 
                                          ADC_Resolution_12Bit, ADC_Prescaler_1 );         //����ģʽ 12λ 1��Ƶ
  
  ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_384Cycles);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  ADC_VrefintCmd(ENABLE);                                 //�����ڲ��ο���ѹ
  ADC_ChannelCmd(ADC1,ADC_Channel_Vrefint,ENABLE );         //ͨ��ѡ���ڲ���ѹ
  

  ADC_SoftwareStartConv (ADC1);                                 //ADC��ʼת��
  while(RESET == ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));        //���ADC�Ƿ�ת������
  ADC_ClearFlag (ADC1,ADC_FLAG_EOC );    
  
  value=ADC_GetConversionValue(ADC1);                      //��ȡת��ֵ
  vdd=1.224*4096/value;                                    //���VDD��ѹ����λV
  ADC_Cmd(ADC1, DISABLE);                                       //�ر�ADC��Χ
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,DISABLE);       //�ر�ADCʱ��
  
  MeterParameter.Voltage = (unsigned short)(vdd*100);
}
/*********************************************************************************
 Function:      //
 Description:   //������ѹ
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
