/*******************************************************************************
Copyright: 
File name: BC95.c
Description: BC95��غ���������
Author: ��ѧ��cy
Version: 
Date: 
History: 
*******************************************************************************/
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include "bc95.h"
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
struct BC95_Str BC95;            
/*******************************************************************************
���Ա���������
*******************************************************************************/
/*******************************************************************************
�ڲ�����������
*******************************************************************************/
/*******************************************************************************
���ܴ��붨����
*******************************************************************************/
//BC95��Դ����
//void BC95_PWR_Set(void)
//{
//  GPIO_SetBits(GPIOA,GPIO_Pin_3);
//}
//void BC95_PWR_Rst(void)
//{
//  GPIO_ResetBits(GPIOA,GPIO_Pin_3);
//}
////BC95��λ����
//void BC95_RST_Set(void)        
//{
//  GPIO_ResetBits(GPIOE,GPIO_Pin_1);
//}
//void BC95_RST_Rst(void)        
//{
//  GPIO_SetBits(GPIOE,GPIO_Pin_1);
//}
//BC95״ָ̬ʾ��
//void BC95_LED_OFF(void)       
//{
//  GPIO_ResetBits(GPIOD, GPIO_Pin_5);
//}
void BC95_LedRun(void)        
{
  GPIO_ToggleBits(GPIOD, GPIO_Pin_6);
}
/*******************************************************************************
 Function:      //
 Description:   //��ʼ��
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void BC95_Init(void)        
{
  if(DevicePara.Mode.ManualReport == WAKE)
  {
    SoftTimer_CreateTimer(CONTINUOUS, 1000, BC95_LedRun); 
  }  
  
  USART2_Config(9600);  
  BC95_PWR_Set;
  
  SoftTimer_CreateTimer(ONCE,10,BC95_Reset); 
}
/*******************************************************************************
 Function:      //
 Description:   //����
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void BC95_Reset(void)
{
  BC95_RST_Rst;     //��λ������
  
  SoftTimer_CreateTimer(ONCE,200,BC95_Start); 
}
/*******************************************************************************
 Function:      //
 Description:   //����
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void BC95_Start(void)
{
  BC95_RST_Set;     //��λ������
  
  BC95.Process = NRB;
  SoftTimer_CreateTimer(ONCE,10000,
                        BC95_Timeout); 
}
 /*******************************************************************************
 Function:      //
 Description:   //ȡ����ʼ��ΪĬ�ϸ�λ״̬
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void BC95_Reconnect(void)
{
  SoftTimer_DeleteTimer(BC95_InquireMsgRecv);
  SoftTimer_DeleteTimer(BC95_Timeout);
  SoftTimer_DeleteTimer(BC95_LedRun); 
  SoftTimer_DeleteTimer(BC95_DeInit);
  
  USART2_Free();
  BC95.ReconnectTimes++;
  if(BC95.ReconnectTimes < 3)    //����δ������������
  {
    if(DevicePara.Mode.ManualReport == WAKE)
    {
      SoftTimer_CreateTimer(CONTINUOUS, 1000, BC95_LedRun); 
    }
    BC95.Process = NRB;
    BC95.IncidentPend = TRUE; 
    BC95.TimeoutNum = 0;  
  }
  else                            //����ػ�
  {
    DataPocess_WriteDebugReportStatistics(2);      //ʧ�ܴ���+1
    DataPocess_WriteDebugReportRecord(2);           //����ʧ�ܼ�¼
    UplinkProtocol_DeInit();
  }
}
/*******************************************************************************
 Function:      //
 Description:   //�ػ�
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void BC95_ShutDown(void)
{
  BC95.Process = BC95_SHUTDOWN; 
  BC95.IncidentPend = TRUE;    
}
/*******************************************************************************
 Function:      //
 Description:   //ȡ����ʼ��ΪĬ�ϸ�λ״̬
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void BC95_DeInit(void)        
{
  SoftTimer_DeleteTimer(BC95_InquireMsgRecv);
  SoftTimer_DeleteTimer(BC95_Timeout);
  SoftTimer_DeleteTimer(BC95_LedRun); 
  SoftTimer_DeleteTimer(BC95_DeInit);

  BC95_LED_OFF;
  BC95_PWR_Rst;
  BC95.Process = BC95_IDLE;
  BC95.IncidentPend = FALSE; 
  BC95.TimeoutNum = 0;
  BC95.ReconnectTimes = 0;
  memset(BC95.RxBuffer,0,RECV_BUFF_SIZE);
  BC95.RxLength = 0;
  BC95.ErrorCode = 0;
  BC95.ErrorStep = 0;
  
  USART2_Free();
  USART_DeInit(USART2);
  CLK_PeripheralClockConfig(CLK_Peripheral_USART2,DISABLE);
  GPIO_Init(GPIOE, GPIO_Pin_4 , GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOE, GPIO_Pin_3 , GPIO_Mode_Out_PP_Low_Slow);
 
  DevicePara.Mode.PeriodicReport = SLEEP;
  DevicePara.Mode.ManualReport = SLEEP;
  DevicePara.Mode.AlarmReport = SLEEP; 
}
/*******************************************************************************
 Function:      //
 Description:   //����
 Input:         //
 Output:        //
 Return:      	//
 Others:        //
*******************************************************************************/
void BC95_Process(void)                         
{
  char *str = NULL;
  
  if(BC95.IncidentPend != FALSE) //����Ƿ����¼�����
  {
    BC95.IncidentPend = FALSE;  //����¼�����
    switch(BC95.Process)
    {
    case NRB:                   //����
      {
        BC95_TxStr("AT+NRB\r\n",8);
        SoftTimer_CreateTimer(ONCE,10000,
                              BC95_Timeout); 
      }
      break;
    case AT:                    //ͬ��������
      {
        BC95_TxStr("AT\r\n",4);         
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      } 
      break;
    case CMEE:                  //�����ƶ��ն˴���
      {
        BC95_TxStr("AT+CMEE=1\r\n",11);         
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      } 
      break;
    case CFUN0:                 //������С����
      {
        BC95_TxStr("AT+CFUN=0\r\n",11);
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      } 
      break;
    case NCSEARFCN:             //�������Ƶ��
      { 
        BC95_TxStr("AT+NCSEARFCN\r\n",14);
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      } 
      break;
    case CFUN1:                 //����ȫ����
      {      
        BC95_TxStr("AT+CFUN=1\r\n",11);
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      } 
      break;
    case CGSN:                  //��ѯIMEI      
      {
        BC95_TxStr("AT+CGSN=1\r\n",11);
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      }
      break;   
    case NCCID:                //��ѯCCID
      {
        BC95_TxStr("AT+NCCID\r\n",10); 
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      }
      break; 
    case CIMI:                  //��ѯIMSI
      {
        BC95_TxStr("AT+CIMI\r\n",9); 
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      }
      break; 
    case NCDP:                  //����CDP������ 
      {
 //       BC95_TxStr("AT+NCDP=180.101.147.115,5683\r\n",30);    //����ƽ̨
        BC95_TxStr("AT+NCDP=117.60.157.137,5683\r\n",29);      //��ʽƽ̨
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      }
      break; 
    case NSMI:                 //����������Ϣָʾ
      {
        BC95_TxStr("AT+NSMI=1\r\n",11);
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout);
      }
      break;
    case NNMI:                 //�ر�����Ϣָʾ 
      {
        BC95_TxStr("AT+NNMI=0\r\n",11);
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout);
      }
      break;
    case CGATT1:                //�������總��  
      {
        BC95_TxStr("AT+CGATT=1\r\n",12);
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      }
      break;
    case CGATT:                 //��ѯ���總�� 
      {
        BC95_TxStr("AT+CGATT?\r\n",11);
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      }
      break;
    case CEREG:                 //��ѯ����ע��   
      {
        BC95_TxStr("AT+CEREG?\r\n",11);
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      }
      break; 
    case CSQ:                   //��ѯ�ź�ǿ��
      {
        BC95_TxStr("AT+CSQ\r\n",8);
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      } 
      break; 
    case NUESTATS:              //��ѯģ��״̬
      {
        BC95_TxStr("AT+NUESTATS\r\n",13);
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      } 
      break;
    case CCLK:                  //��ѯʵʱʱ�� 
      {
        BC95_TxStr("AT+CCLK?\r\n",10);
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      }
      break;
    case NMSTATUS:              //��ѯ��Ϣע��״̬
      {
        BC95_TxStr("AT+NMSTATUS?\r\n",14);
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      }
      break;      
    case QLWULDATAEX:          //����CON/NON��Ϣ   
      {   
//        BC95_TxData(); 
      }
      break;
//    case NMGS:                  //������Ϣ     
//      {   
////        BC95_TxData(); 
//      }
//      break;
    case NQMGR:                 //��ѯ��Ϣ����״̬  
      {     
        BC95_TxStr("AT+NQMGR\r\n",10);
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      }
      break;
    case NMGR:                  //��ȡ��Ϣ   
      {     
        BC95_TxStr("AT+NMGR\r\n",9);  
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      }
      break;
    case BC95_SHUTDOWN:       //�ػ�
      {
        BC95_TxStr("AT+CFUN=0\r\n",11);    
        SoftTimer_CreateTimer(ONCE,BC95_TIMEROUT_TIME,
                              BC95_Timeout); 
      }
      break;
    default:
      break;
    }
  }
  if(Usart2.RxPend != FALSE)//�ж�������
  {
    memset(BC95.RxBuffer,'\0',RECV_BUFF_SIZE);                  //�建��
    BC95.RxLength = USART2_RX((unsigned char *)BC95.RxBuffer);//��������
  
    BC95_GetErrorInfo(BC95.RxBuffer, BC95.RxLength);            //��ȡ������
    BC95.ErrorStep = BC95.Process;
    switch(BC95.Process)
    {
    case NRB:                  //����
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {
          BC95.TimeoutNum = 0;
          BC95.Process = AT;
          BC95.IncidentPend = TRUE;
          SoftTimer_DeleteTimer(BC95_Timeout);
        }
      }
      break;
    case AT:               //ͬ��������
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {   
          BC95.TimeoutNum = 0;
          BC95.Process = CMEE;
          BC95.IncidentPend = TRUE;
          SoftTimer_DeleteTimer(BC95_Timeout);
        }
      } 
      break;
    case CMEE:                 //�����ƶ��ն˴���
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {    
          BC95.TimeoutNum = 0;
          if(BC95.ReconnectTimes == 0)
          {
            BC95.Process = CFUN1;
          }
          else
          {
            BC95.Process = CFUN0;
          }
          BC95.IncidentPend = TRUE;
          SoftTimer_DeleteTimer(BC95_Timeout);
        }
      } 
      break;
    case CFUN0:               //������С����
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {   
          BC95.TimeoutNum = 0;
          BC95.Process = NCSEARFCN;
          BC95.IncidentPend = TRUE;
          SoftTimer_DeleteTimer(BC95_Timeout);
        }  
      } 
      break;
    case NCSEARFCN:               //�������Ƶ��
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {   
          BC95.TimeoutNum = 0;
          BC95.Process = CFUN1;
          BC95.IncidentPend = TRUE;
          SoftTimer_DeleteTimer(BC95_Timeout);
        } 
      } 
      break;
    case CFUN1:               //����ȫ����
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {   
          BC95.TimeoutNum = 0;
          BC95.Process = CGSN;
          BC95.IncidentPend = TRUE;
          SoftTimer_DeleteTimer(BC95_Timeout);
        } 
      } 
      break;
    case CGSN:                //��ѯIMEI
      {
        str = strstr((const char *)BC95.RxBuffer,"+CGSN");
        if( str != NULL)
        {
          memcpy(BC95.IMEI,&str[6],15);
          DataProcess_WriteIMEI();
          BC95.TimeoutNum = 0;
          BC95.Process = NCCID;
          BC95.IncidentPend = TRUE;
          SoftTimer_DeleteTimer(BC95_Timeout);
        }
      }
      break;
    case NCCID:                //��ѯCCID
      {
        str = strstr((const char *)BC95.RxBuffer,"+NCCID:");
        if( str != NULL)
        {
          memcpy(BC95.CCID,&str[7],20);
          BC95.TimeoutNum = 0;
          BC95.Process = CIMI;
          BC95.IncidentPend = TRUE;
          SoftTimer_DeleteTimer(BC95_Timeout);
        }
      }
      break; 
    case CIMI:                //��ѯIMSI
      {
        str = strstr((const char *)BC95.RxBuffer,"460");
        if( str != NULL)
        {
          memcpy(BC95.IMSI,str,15);
          BC95.TimeoutNum = 0;
          BC95.Process = NCDP;
          BC95.IncidentPend = TRUE;
          SoftTimer_DeleteTimer(BC95_Timeout);
        }
      }
      break;  
    case NCDP:                 //����CDP������ 
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {     
          BC95.TimeoutNum = 0;
          BC95.Process = NSMI;
          BC95.IncidentPend = TRUE;
          SoftTimer_DeleteTimer(BC95_Timeout);
        }
      }
      break;  
    case NSMI:                 //����������Ϣָʾ
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {   
          BC95.TimeoutNum = 0;
          BC95.Process = NNMI;
          BC95.IncidentPend = TRUE;
          SoftTimer_DeleteTimer(BC95_Timeout);
        }  
      }
      break;
    case NNMI:                 //�ر�����Ϣָʾ 
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {   
          BC95.TimeoutNum = 0;
          BC95.Process = CGATT1;
          BC95.IncidentPend = TRUE;
          SoftTimer_DeleteTimer(BC95_Timeout);
        }  
      }
      break;
    case CGATT1:               //�������總��  
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {   
          BC95.TimeoutNum = 0;
          BC95.Process = CGATT;
          BC95.IncidentPend = TRUE;
          SoftTimer_DeleteTimer(BC95_Timeout);
        } 
      }
      break;
    case CGATT:               //��ѯ���總�� 
      {
        if( strstr((const char *)BC95.RxBuffer,"+CGATT:1") != NULL)
        {  
          BC95.TimeoutNum = 0;
          BC95.Process = CEREG;
          BC95.IncidentPend = TRUE;
          SoftTimer_DeleteTimer(BC95_Timeout);
        }
      }
      break;
    case CEREG:                //��ѯ����ע��   
      {
        str = strstr((const char *)BC95.RxBuffer,"+CEREG:");
        if( (str[9] == '1')||(str[9] == '5') )//��ע��
        {   
          BC95.TimeoutNum = 0;
          BC95.Process = CSQ;
          BC95.IncidentPend = TRUE;
          SoftTimer_DeleteTimer(BC95_Timeout);
        }
      }
      break;  
    case CSQ:                 //��ѯ�ź�ǿ��
      {
        str = strstr((const char *)BC95.RxBuffer,"+CSQ:");
        if( str != NULL)
        {
          BC95.CSQ =0;//�����ź�ǿ��
          if( (str[5] >= '0') && (str[5] <= '9') )
          {
            BC95.CSQ += str[5]-0x30;
          }
          if( (str[6] >= '0') && (str[6] <= '9') )
          {
            BC95.CSQ *=10;
            BC95.CSQ += str[6]-0x30;
          }
    
          if( (BC95.CSQ > 0)&&(BC95.CSQ <= 31) )
          {
            BC95.TimeoutNum = 0;
            BC95.Process = NUESTATS;
            BC95.IncidentPend = TRUE;
            SoftTimer_DeleteTimer(BC95_Timeout);
          }
        } 
      } 
      break; 
    case NUESTATS:                 //��ѯģ��״̬
      {
        if(strstr((const char *)BC95.RxBuffer,"Signal power") != NULL)
        {   
          BC95_GetSignalParas(BC95.RxBuffer,BC95.RxLength);
          BC95.TimeoutNum = 0;
          BC95.Process = CCLK;
          BC95.IncidentPend = TRUE;
          SoftTimer_DeleteTimer(BC95_Timeout);
        }  
      } 
      break;
    case CCLK:                //��ѯʵʱʱ�� 
      {
        str = strstr((const char *)BC95.RxBuffer,"+CCLK:");
        if( str != NULL)
        {  
          if( ERROR != RTC_GMTtoBT((unsigned char*)str) )
          {
            BC95.TimeoutNum = 0;
            BC95.Process = NMSTATUS;
            BC95.IncidentPend = TRUE;
            SoftTimer_DeleteTimer(BC95_Timeout);
          }
        }  
      }
      break;
    case NMSTATUS:              //��ѯ��Ϣע��״̬
      {
        if( (strstr((const char *)BC95.RxBuffer,"MO_DATA_ENABLED") != NULL)
          ||(strstr((const char *)BC95.RxBuffer,"INIITIALISED") != NULL) )  //���ݾɰ�ģ��
        {   
          SoftTimer_DeleteTimer(BC95_Timeout);
          BC95.TimeoutNum = 0;
          BC95.Process = NMGS;
          
          if(DevicePara.Mode.ManualReport == WAKE)
          {
            SoftTimer_CreateTimer(CONTINUOUS, 500, BC95_LedRun); 
          }
          UplinkProtocol_StartReport();
        }  
      }
      break;
//    case QLWULDATAEX:          //����CON/NON��Ϣ   
//      {  
//        if(strstr((const char *)BC95.RxBuffer,"+QLWULDATASTATUS:4") != NULL)  //���ͳɹ�
//        {
////          UplinkProtocol_TxSuccess();
//          BC95.Process = NQMGR;
//          BC95.IncidentPend = TRUE;
//        }
//      }
//      break;
    case NMGS:          //������Ϣ   
      {  
        if(strstr((const char *)BC95.RxBuffer,"+NSMI:SENT") != NULL)  //���ͳɹ�
        {
          BC95.Process = NQMGR;
          BC95.IncidentPend = TRUE;
        }
      }
      break;
    case NQMGR:                 //��ѯ����Ϣ  
      {     
        if(strstr((const char *)BC95.RxBuffer,"BUFFERED=") != NULL)
        {  
          SoftTimer_DeleteTimer(BC95_Timeout); 
          if(strstr((const char *)BC95.RxBuffer,"BUFFERED=0") == NULL)  //����Ϣ����
          {
            BC95.Process = NMGR;
            BC95.IncidentPend = TRUE;
          }
          else                                                          //δ����Ϣ
          {
            SoftTimer_CreateTimer(ONCE, 1000, BC95_InquireMsgRecv); 
          }
        }  
      }
      break;
    case NMGR:                 //��ȡ����Ϣ   
      {     
        if(strstr((const char *)BC95.RxBuffer,",") != NULL)
        {  
          SoftTimer_DeleteTimer(BC95_Timeout); 
          if(strstr((const char *)BC95.RxBuffer,"4,AAAA") != NULL)
          {   
            UplinkProtocol_TxSuccess();
          }
          else
          {
            BC95_RxData(BC95.RxBuffer,BC95.RxLength);   
          }        
        }   
      }
      break;
    case BC95_SHUTDOWN:       //�ϵ�
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {   
          SoftTimer_DeleteTimer(BC95_Timeout);
          BC95_DeInit();   
        } 
      }
      break;
    default:
      break;
    }
  }
  
}
/******************************************************************************
 Function:      //
 Description:   //��������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*******************************************************************************/
void BC95_RxData(uint8_t *buff,uint16_t length)
{
  uint8_t temp[128] = {0};
  uint8_t i = 0,dataLen = 0;
  uint8_t *str = NULL;
  
  while(buff[i] != ',')
  {
    if( (buff[i] >= '0')&&(buff[i] <= '9') )
    {
      dataLen = dataLen*10+ASCLL_to_Int(buff[i]);
    }
    i++;
    if(i >= length)
    {
      return;
    }
  }
  
  if(dataLen > 128)
  {
    dataLen = 128;
  }
  
  str = (uint8_t *)&buff[i+1];
  for(i = 0;i < dataLen;i++)
  {
    temp[i] = ASCLL_to_Int(str[i*2])*0x10+ASCLL_to_Int(str[i*2+1]);
  }

  UplinkProtocol_RxProcess(temp,dataLen);    
}
/*******************************************************************************
 Function:      //
 Description:   //�����ַ���
 Input:         //
 Output:        //
 Return:        //
 Others:        //
*******************************************************************************/
void BC95_TxStr(uint8_t *buff,uint16_t length)
{  
  USART_ITConfig(USART2,USART_IT_TC,DISABLE);                  //�رշ����ж�  
  memset(Usart2.TxBuffer,'\0',Usart2.TxLength);//�建��
  Usart2.TxLength = length;
  memcpy(Usart2.TxBuffer,buff,length);
  USART2_TX();
}
/*******************************************************************************
 Function:      //
 Description:   //���ͱ���
 Input:         //
 Output:        //
 Return:        //
 Others:        //
*******************************************************************************/
void BC95_TxMessage_NMGS(uint8_t *buff,uint16_t length)
{  
  BC95.Process = NMGS;
  
  USART_ITConfig(USART2,USART_IT_TC,DISABLE);                  //�رշ����ж�  
  memset(Usart2.TxBuffer,'\0',Usart2.TxLength);//�建��
  Usart2.TxLength = length;
  memcpy(Usart2.TxBuffer,buff,length);
  USART2_TX();
}
/*******************************************************************************
 Function:      //
 Description:   //��ȡ������Ϣ
 Input:         //
 Output:        //
 Return:        //
 Others:        //
*******************************************************************************/
void BC95_GetErrorInfo(uint8_t *buff,uint16_t length)
{
  char *str = NULL;
   
  str = strstr((const char *)buff,"ERROR");
  if(str != NULL)
  {
    BC95.ErrorCode = 0;
    if( (str[6] >= '0') && (str[6] <= '9') )
    {
      BC95.ErrorCode += str[6]-0x30;
    }
    if( (str[7] >= '0') && (str[7] <= '9') )
    {
      BC95.ErrorCode *=10;
      BC95.ErrorCode += str[7]-0x30;
    }
    if( (str[8] >= '0') && (str[8] <= '9') )
    {
      BC95.ErrorCode *=10;
      BC95.ErrorCode += str[8]-0x30;
    }
  }
  else if(strstr((const char *)buff,"OK") != NULL)
  {   
    BC95.ErrorCode = 0;
  }
}
/*******************************************************************************
 Function:      //
 Description:   //��ʱ�ص�
 Input:         //
 Output:        //
 Return:        //
 Others:        //
*******************************************************************************/
void BC95_Timeout(void)
{
  unsigned char timeoutMax = 0;
  
  BC95.TimeoutNum++;
  
  switch(BC95.Process)
  {
  case NRB:                     //����
    {
      timeoutMax = 3;
      break;
    }
  case CGATT:                   //��ѯ���總��  
    {
      timeoutMax = 20*(BC95.ReconnectTimes+1);
      break;
    }
  case BC95_SHUTDOWN:           //�ϵ�
    {
      BC95_DeInit();    
      return;
    }
    break;
  default:
    {
      timeoutMax = 10;
      break;
    }
  }
  
  if(BC95.TimeoutNum < timeoutMax)//�жϳ�ʱ�Ƿ񳬴���
  {
    BC95.IncidentPend = TRUE;  
  }
  else
  {
    BC95.TimeoutNum = 0; 
    if(BC95.Process == CSQ)
    {
      BC95.Process = NUESTATS;
      BC95.IncidentPend = TRUE;  
    }
    else
    {
      BC95.ErrorStep = BC95.Process;
      BC95_Reconnect();
    }
  }
}
/*******************************************************************************
 Function:      //
 Description:   //��ʱ�ص�
 Input:         //
 Output:        //
 Return:        //
 Others:        //
*******************************************************************************/
void BC95_Delay_CallBack(void)
{
  BC95.IncidentPend = TRUE;//��ǹ���
}
/*******************************************************************************
 Function:      //
 Description:   //��ѯ��Ϣ����
 Input:         //
 Output:        //
 Return:        //
 Others:        //
*******************************************************************************/
void BC95_InquireMsgRecv(void)
{
   BC95.Process = NQMGR;
   BC95.IncidentPend = TRUE;
}
/*******************************************************************************
 Function:      //
 Description:   //��ȡ�źŲ���
 Input:         //
 Output:        //
 Return:        //
 Others:        //
*******************************************************************************/
void BC95_GetSignalParas(uint8_t *buff, uint16_t length)
{
  char *str = NULL;
  uint8_t i = 0;

  BC95.RSRP = 0;
  str = strstr((const char *)buff,"Signal power:");
  if( str != NULL)
  {
    for(i = 13;i < 19;i++)
    {
      if((str[i] >= '0') && (str[i] <= '9'))
      {
        BC95.RSRP = BC95.RSRP*10 + (str[i]-0x30);
      }
    }
    BC95.RSRP = BC95.RSRP/10;
    if(str[13] == '-')
    {
      BC95.RSRP = 0 - BC95.RSRP;
    }
  }

  BC95.SNR = 0;
  str = strstr((const char *)buff,"SNR:");
  if( str != NULL)
  {
    for(i = 4;i < 10;i++)
    {
      if((str[i] >= '0') && (str[i] <= '9'))
      {
        BC95.SNR = BC95.SNR*10 + (str[i]-0x30);
      }
    }
    BC95.SNR = BC95.SNR/10;
    if(str[4] == '-')
    {
      BC95.SNR = 0 - BC95.SNR;
    }
  }
  
  BC95.ECL = 0;
  str = strstr((const char *)buff,"ECL:");
  if( str != NULL)
  {
    for(i = 4;i < 7;i++)
    {
      if((str[i] >= '0') && (str[i] <= '9'))
      {
        BC95.ECL = BC95.ECL*10 + (str[i]-0x30);
      }
    }
  }
  
  BC95.CELLID = 0;
  str = strstr((const char *)buff,"Cell ID:");
  if( str != NULL)
  {
    for(i = 8;i < 18;i++)
    {
      if((str[i] >= '0') && (str[i] <= '9'))
      {
        BC95.CELLID = BC95.CELLID*10 + (str[i]-0x30);
      }
      else
      {
        break;
      }
    }
  }
  
  BC95.EARFCN = 0;
  str = strstr((const char *)buff,"EARFCN:");
  if( str != NULL)
  {
    for(i = 7;i < 15;i++)
    {
      if((str[i] >= '0') && (str[i] <= '9'))
      {
        BC95.EARFCN = BC95.EARFCN*10 + (str[i]-0x30);
      }
      else
      {
        break;
      }
    }
  }
  
  BC95.PCI = 0;
  str = strstr((const char *)buff,"PCI:");
  if( str != NULL)
  {
    for(i = 4;i < 9;i++)
    {
      if((str[i] >= '0') && (str[i] <= '9'))
      {
        BC95.PCI = BC95.PCI*10 + (str[i]-0x30);
      }
      else
      {
        break;
      }
    }
  }
}
/*********************************************************************************
 Function:      //
 Description:   //������Ӧ
 Input:         //messageId:��ַ��
                //errcode:00�ɹ���01ʧ��
 Output:        //mid:��Ӧ��ʶ
 Return:        //
 Others:        //
*********************************************************************************/
void BC95_ACK(uint8_t mid[4])
{
//  uint8_t data[32] = "AT+NMGS=5,0000000000\r\n";
//  
//  data[10] = 0x30;
//  data[11] = 0x33;
//  data[12] = mid[0];
//  data[13] = mid[1];
//  data[14] = mid[2];
//  data[15] = mid[3];
//  data[16] = 0x30;
//  data[17] = 0x30;
//  data[18] = 0x30;
//  data[19] = 0x30;
//  
//  BC95_TxStr(data,22);
}
/*************************************END**************************************/