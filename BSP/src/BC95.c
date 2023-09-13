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
struct BC95_Str BC95;            //BC95 �õļĴ���
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
 Function:      //void BC95_Power_On(void)
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void BC95_Power_On(void)        
{
  USART2_Configuration();               //��ʼ������2
   
  GPIO_SetBits(GPIOE,GPIO_Pin_2);       //VBAT����        3.1-4.2V������ֵ3.6V
  GPIO_SetBits(GPIOE,GPIO_Pin_1);       //��λ������

  Create_Timer(ONCE,200,
               BC95_Reset,0,PROCESS); 
  
  if(BC95.Manual != FALSE)
  {
    Create_Timer(CONTINUOUSS,1000,
                 BC95_NetLightRun,0,PROCESS); 
  }
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
void BC95_Power_Off(void)        //BC95�ϵ�
{
  Delete_Timer(BC95_NetLightRun);
  GPIO_ResetBits(GPIOD,GPIO_Pin_6);
  GPIO_ResetBits(GPIOE,GPIO_Pin_2); 
    
  BC95.ReconnectTimes = 0; 
  BC95.ReportBit = 0;
  BC95.StartProcess = IDLE;
  BC95.IncidentPend = FALSE; //����¼�����
  BC95.TimeoutNum = 0;
  BC95.Manual = FALSE;
  memset(BC95.RxBuffer,0,RECV_BUFF_SIZE);
  BC95.RxLength = 0;

  Free_Uart2();
  USART_DeInit(USART2);
  CLK_PeripheralClockConfig(CLK_Peripheral_USART2,DISABLE);
  GPIO_Init(GPIOE, GPIO_Pin_4 , GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOE, GPIO_Pin_3 , GPIO_Mode_Out_PP_Low_Slow);
  
  MeterParameter.Mode.bc95 = SLEEP;
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
void BC95_Reset(void)
{
  GPIO_ResetBits(GPIOE,GPIO_Pin_1);     //��λ������
   
  BC95.StartProcess = AT; //ָ����һ������
  BC95.IncidentPend = TRUE;
  BC95.TimeoutNum = 0;   
  
  BC95.ReportBit = 1;
}
/*********************************************************************************
 Function:      //
 Description:   //����ָʾ��
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void BC95_NetLightRun(void)
{
  GPIO_ToggleBits(GPIOD,GPIO_Pin_6);     
}
/*********************************************************************************
 Function:      //
 Description:   ////BC95������
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void BC95_Process(void)                         
{
  char *str = NULL;
  
  if(BC95.IncidentPend != FALSE) //����Ƿ����¼�����
  {
    BC95.IncidentPend = FALSE; //����¼�����
    switch(BC95.StartProcess)
    {
    case NRB:                   //����
      {
        BC95_Data_Send("AT+NRB\r\n",8);
        Create_Timer(ONCE,10000,
                     BC95_Timeout_CallBack,0,PROCESS); 
      }
      break;
    case AT:                    //ͬ��������
      {
        BC95_Data_Send("AT\r\n",4);         
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS); 
      } 
      break;
    case CMEE:                  //�����ƶ��ն˴���
      {
        BC95_Data_Send("AT+CMEE=1\r\n",11);         
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS); 
      } 
      break;
    case CFUN0:                 //������С����
      {
        BC95_Data_Send("AT+CFUN=0\r\n",11);
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS); 
      } 
      break;
    case NCSEARFCN:             //�������Ƶ��
      {
        BC95_Data_Send("AT+NCSEARFCN\r\n",14);
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS); 
      } 
      break;
    case CFUN1:                 //����ȫ����
      {
        BC95_Data_Send("AT+CFUN=1\r\n",11);
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS); 
      } 
      break;
    case CGSN:                  //��ѯIMEI      
      {
        BC95_Data_Send("AT+CGSN=1\r\n",11);
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS); 
      }
      break;  
    case CCID:                  //��ѯCCID
      {
        BC95_Data_Send("AT+NCCID\r\n",10); 
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS); 
      }
      break; 
    case NCDP:                  //����CDP������ 
      {
//        BC95_Data_Send("AT+NCDP=180.101.147.115,5683\r\n",30);    //����ƽ̨
        BC95_Data_Send("AT+NCDP=117.60.157.137,5683\r\n",29);      //��ʽƽ̨
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS); 
      }
      break; 
    case NSMI1:                 //����������Ϣָʾ
      {
        BC95_Data_Send("AT+NSMI=1\r\n",11);
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS);
      }
      break;
    case NNMI0:                 //�ر�����Ϣָʾ 
      {
        BC95_Data_Send("AT+NNMI=0\r\n",11);
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS);
      }
      break;
    case CGATT1:                //�������總��  
      {
        BC95_Data_Send("AT+CGATT=1\r\n",12);
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS); 
      }
      break;
    case CSQ:                   //��ѯ�ź�ǿ��
      {
        BC95_Data_Send("AT+CSQ\r\n",8);
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS); 
      } 
      break; 
    case CGATT:                 //��ѯ���總�� 
      {
        BC95_Data_Send("AT+CGATT?\r\n",11);
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS); 
      }
      break;
    case CEREG:                 //��ѯ����ע��״̬     
      {
        BC95_Data_Send("AT+CEREG?\r\n",11);
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS); 
      }
      break;
    case NUESTATS:              //��ѯģ��״̬
      {
        BC95_Data_Send("AT+NUESTATS\r\n",13);
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS); 
      } 
      break;
    case CCLK:                  //��ѯʵʱʱ�� 
      {
        BC95_Data_Send("AT+CCLK?\r\n",10);
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS); 
      }
      break;
    case NMSTATUS:              //��ѯ��Ϣע��״̬
      {
        BC95_Data_Send("AT+NMSTATUS?\r\n",14);
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS); 
      }
      break;
    case NNMI1:                 //���ý�����Ϣָʾ
      {
        BC95_Data_Send("AT+NNMI=1\r\n",11);
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS); 
      }
      break;
    case NMGS:                 //������Ϣ     
      {     
        Send_Data_Process();   
      }
      break;
    case BC95_RECONNECT:      //����ʧ��
      {
        BC95.ReconnectTimes++;
        if(BC95.ReconnectTimes >= 3)  //������������˯��
        {  
          Save_DebugReportStatistics(2);      //ʧ�ܴ���+1
          Save_DebugReportRecord(2);           //����ʧ�ܼ�¼
          Save_History_Data();    //���汾������
          Save_BC95_Error_Record();     //������ϼ�¼
          BC95.StartProcess = BC95_POWER_DOWN; //ָ����һ������
          BC95.IncidentPend = TRUE; //�¼���־����
        }
        else                            //��������
        {
          if(BC95.Manual != FALSE)
          {
            Create_Timer(CONTINUOUSS,1000,
                         BC95_NetLightRun,0,PROCESS); 
          }
  
          BC95.StartProcess = NRB; //ָ����һ������
          BC95.IncidentPend = TRUE; //�¼���־����
          BC95.TimeoutNum = 0;  
          Free_Uart2();
        }
      }
      break;
    case BC95_POWER_DOWN:       //�ϵ�
      {
        BC95_Data_Send("AT+CFUN=0\r\n",11);
        Create_Timer(ONCE,BC95_TIMEROUT_TIME,
                     BC95_Timeout_CallBack,0,PROCESS);       
      }
      break;
    default:
      break;
    }
  }
  if(Uart2.Receive_Pend != FALSE)//�ж�������
  {
    memset(BC95.RxBuffer,'\0',RECV_BUFF_SIZE);//�建��
    BC95.RxLength = Uart2_Receive((unsigned char *)BC95.RxBuffer);//��������
    /*��ȡ������*/
    str = strstr((const char *)BC95.RxBuffer,"ERROR:");
    if(str != NULL)
    {
      BC95.ErrorStep = BC95.StartProcess;
      BC95.ErrorCode = 0;
      if( (str[7] >= '0') && (str[7] <= '9') )
      {
        BC95.ErrorCode += str[7]-0x30;
      }
      if( (str[8] >= '0') && (str[8] <= '9') )
      {
        BC95.ErrorCode *=10;
        BC95.ErrorCode += str[8]-0x30;
      }
      if( (str[9] >= '0') && (str[9] <= '9') )
      {
        BC95.ErrorCode *=10;
        BC95.ErrorCode += str[9]-0x30;
      }
    }
    else if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
    {
      BC95.ErrorStep = 0;
      BC95.ErrorCode = 0;
    }
    
    switch(BC95.StartProcess)
    {
    case NRB:                  //����
      {
  //      if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {
          BC95.StartProcess = AT;
          BC95.IncidentPend = TRUE;//��ǹ���
          BC95.TimeoutNum = 0;
          Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص� 
        }
      }
      break;
    case AT:               //ͬ��������
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {   
          BC95.TimeoutNum = 0;
          BC95.StartProcess = CMEE;
          BC95.IncidentPend = TRUE;
          Delete_Timer(BC95_Timeout_CallBack);
        }
      } 
      break;
    case CMEE:                 //�����ƶ��ն˴���
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {   
          if(BC95.ReconnectTimes == 0)  //��������
          {
            BC95.StartProcess = CFUN1;
          }
          else                          //��������
          {
            BC95.StartProcess = CFUN0;
          }
          BC95.IncidentPend = TRUE;//��ǹ���
          BC95.TimeoutNum = 0;
          Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص�
        }
      } 
      break;
    case CFUN0:               //������С����
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {   
          BC95.TimeoutNum = 0;
          BC95.StartProcess = NCSEARFCN;
          BC95.IncidentPend = TRUE;//��ǹ���
          Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص�
        }  
      } 
      break;
    case NCSEARFCN:               //�������Ƶ��
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {   
          BC95.TimeoutNum = 0;
          BC95.StartProcess = CFUN1;
          BC95.IncidentPend = TRUE;//��ǹ���
          Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص�
        } 
      } 
      break;
    case CFUN1:               //����ȫ����
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {   
          BC95.TimeoutNum = 0;
          BC95.StartProcess = CGSN;
          BC95.IncidentPend = TRUE;//��ǹ���
          Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص�
        } 
      } 
      break;
    case CGSN:                //��ѯIMEI
      {
        str = strstr((const char *)BC95.RxBuffer,"+CGSN");
        if( str != NULL)
        {
          memcpy(BC95.IMEI,&str[6],15);
          BC95.TimeoutNum = 0;
          BC95.StartProcess = CCID;
          BC95.IncidentPend = TRUE;//��ǹ���
          Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص�
        }
      }
      break;
    case CCID:                //��ѯCCID
      {
        str = strstr((const char *)BC95.RxBuffer,"+NCCID");
        if( str != NULL)
        {
          memcpy(BC95.ICCID,&str[7],20);
          BC95.TimeoutNum = 0;
          BC95.StartProcess = NCDP;
          BC95.IncidentPend = TRUE;//��ǹ���
          Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص�
        }
      }
      break;
    case NCDP:                 //����CDP������ 
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {     
          BC95.TimeoutNum = 0;
          BC95.StartProcess = NSMI1;
          BC95.IncidentPend = TRUE;//��ǹ���
          Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص�
        }
      }
      break;  
    case NSMI1:                 //���÷�����Ϣָʾ
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {   
          BC95.TimeoutNum = 0;
          BC95.StartProcess = NNMI0;
          BC95.IncidentPend = TRUE;//��ǹ���
          Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص�
        }  
      }
      break;
    case NNMI0:                 //��ֹ����Ϣָʾ 
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {   
          BC95.TimeoutNum = 0;
          BC95.StartProcess = CGATT1;
          BC95.IncidentPend = TRUE;//��ǹ���
          Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص�
        }  
      }
      break;
    case CGATT1:               //�������總��  
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {   
          BC95.TimeoutNum = 0;
          BC95.StartProcess = CSQ;
          BC95.IncidentPend = TRUE;//��ǹ���
          Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص�
        } 
      }
      break;
    case CSQ:                 //��ѯ�ź�ǿ��
      {
        str = strstr((const char *)BC95.RxBuffer,"+CSQ");
        if( str != NULL)
        {
          BC95.Rssi =0;//�����ź�ǿ��
          if( (str[5] >= '0') && (str[5] <= '9') )
          {
            BC95.Rssi += str[5]-0x30;
          }
          if( (str[6] >= '0') && (str[6] <= '9') )
          {
            BC95.Rssi *=10;
            BC95.Rssi += str[6]-0x30;
          }
    
          if( (BC95.Rssi > 0 )&&(BC95.Rssi <= 31) )
          {
            BC95.TimeoutNum = 0;
            BC95.StartProcess = CGATT;
            BC95.IncidentPend = TRUE;//��ǹ���
            Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص�
          }
        } 
      } 
      break; 
    case CGATT:               //��ѯ���總�� 
      {
        if( strstr((const char *)BC95.RxBuffer,"+CGATT:1") != NULL)
        {  
          BC95.TimeoutNum = 0;
          BC95.StartProcess = CEREG;
          BC95.IncidentPend = TRUE;//��ǹ���
          Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص�
        }
      }
      break;
    case CEREG:                //��ѯ����ע��״̬     
      {
        if( strstr((const char *)BC95.RxBuffer,"+CEREG:0,1") != NULL )//��ע��
        {   
          BC95.TimeoutNum = 0;
          BC95.StartProcess = NUESTATS;
          BC95.IncidentPend = TRUE;//��ǹ���
          Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص�
        }
      }
      break;  
    case NUESTATS:                 //��ѯģ��״̬
      {
        if(strstr((const char *)BC95.RxBuffer,"Signal power") != NULL)
        {   
          BC95_GetSignalParas(BC95.RxBuffer,BC95.RxLength);
          if(BC95.ECL <= 2)
          {
            BC95.TimeoutNum = 0;
            BC95.StartProcess = CCLK;
            BC95.IncidentPend = TRUE;
            Delete_Timer(BC95_Timeout_CallBack);
          }
        }  
      } 
      break;
    case CCLK:                //��ѯʵʱʱ�� 
      {
        str = strstr((const char *)BC95.RxBuffer,"+CCLK:");
        if( str != NULL)
        {  
          GMT_to_BT((unsigned char*)str);

          BC95.TimeoutNum = 0;
          BC95.StartProcess = NMSTATUS;
          BC95.IncidentPend = TRUE;//��ǹ���                //������
          Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص�
        }  
      }
      break;
    case NMSTATUS:              //��ѯ��Ϣע��״̬
      {
        if(strstr((const char *)BC95.RxBuffer,"MO_DATA_ENABLED") != NULL)
        {   
          BC95.TimeoutNum = 0;
          BC95.StartProcess = NNMI1;
          BC95.IncidentPend = TRUE;//��ǹ���
          Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص� 
        }  
      }
      break;
    case NNMI1:                 //���ý�����Ϣָʾ 
      {
        if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        { 
          BC95.TimeoutNum = 0;
          BC95.StartProcess = NMGS;
          BC95.IncidentPend = TRUE;//��ǹ���
          Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص�
          
          if(BC95.Manual != FALSE)
          {
            Create_Timer(CONTINUOUSS,500,
                         BC95_NetLightRun,0,PROCESS); 
          } 
        }  
      }
      break;
    case NMGS:        //������Ϣ
      {  
        if(strstr((const char *)BC95.RxBuffer,"+NNMI:") != NULL)
        {
          if(strstr((const char *)BC95.RxBuffer,"+NNMI:4,AAAA0000") != NULL)
          {
            BC95.ReportBit = 2;
            BC95.IncidentPend = TRUE;//��ǹ���
          }
          else if(strstr((const char *)BC95.RxBuffer,"+NNMI:4,AAAA0001") != NULL)
          {
            BC95.ReportBit = 3;
            BC95.IncidentPend = TRUE;//��ǹ���
          }
          else if(strstr((const char *)BC95.RxBuffer,"+NNMI:4,AAAA0002") != NULL)
          {
            BC95.ReportBit = 3;
            BC95.IncidentPend = TRUE;//��ǹ���
            if(HistoryData.Total > 0)
            {
              HistoryData.Total--;
              HistoryData.Front = (HistoryData.Front+1)%HistoryDataMaxNum;
              WriteRom(HISTORY_DATA_FRONT_ADDR,&HistoryData.Front,3);
            }  
          }
          else
          {
            Recv_Data_Process((unsigned char*)BC95.RxBuffer);
          }
;
          BC95.TimeoutNum = 0;
        }
      }
      break; 
    case BC95_RECONNECT:      //����
      {
      }
      break;
    case BC95_POWER_DOWN:       //�ϵ�
      {
       if(strstr((const char *)BC95.RxBuffer,"OK") != NULL)
        {   
          BC95_Power_Off();
          Delete_Timer(BC95_Timeout_CallBack);//ɾ����ʱ�ص�
        }        
      }
      break;
    default:
      break;
    }
  }
  
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void BC95_Data_Send(unsigned char *Data,unsigned short Len)
{    
  memset(Uart2.R_Buffer,'\0',RECV_BUFF_SIZE);//����ջ�����
  Uart2_Send((unsigned char*)Data,Len);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void BC95_Timeout_CallBack(void)//������ʱ�ط�
{
  unsigned char timeoutMax = 0;
  
  BC95.TimeoutNum++;
  
  switch(BC95.StartProcess)
  {
  case NRB:                   //����
    {
      timeoutMax = 3;
    }
    break;
  case CGATT:                 //��ѯ���總�� 
    {
      timeoutMax = 30;
    }
    break;
  case NMGS:                 //������Ϣ     
    {     
      timeoutMax = 4;
    }
    break;
  case BC95_POWER_DOWN:       //�ϵ�
    {  
      BC95_Power_Off();
      return;
    }
  default:
    {
      timeoutMax = 10;
    }
    break;
  }
  
  if(BC95.TimeoutNum < timeoutMax)//�жϳ�ʱ�Ƿ񳬴���
  {
    BC95.IncidentPend = TRUE;  
  }
  else
  { 
    BC95.StartProcess = BC95_RECONNECT;//��������
    BC95.IncidentPend = TRUE;
    BC95.TimeoutNum = 0;
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void BC95_Delay_CallBack(void)
{
  BC95.IncidentPend = TRUE;//��ǹ���
}
/*********************************************************************************
 Function:      //
 Description:   //������Ϣ����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void Recv_Data_Process(unsigned char* buff)
{
  u16 i = 0;
  u8 month = 0;
  u8 MessageID = 0;
  u16 DataLength = 0;
//  u8 mid[4] = {0};           //�������к�
  u8 valueH = 0,valueL = 0;
  RTC_TimeTypeDef  RTC_Time;
  RTC_DateTypeDef  RTC_Date;
  union flow_union Flow;
  unsigned char* str = NULL;
  
  i = 6;
  while(buff[i] != ',')
  {
    if( (buff[i] >= '0')&&(buff[i] <= '9') )
    {
      DataLength = DataLength*10+ASCLL_to_Int(buff[i]);
    }
    i++;
  }
  
  str = (unsigned char*)&buff[i];
  for(i = 0;i < (DataLength*2);i++)
  {
    str[i+1] = ASCLL_to_Int(str[i+1]);
  }
  
  
  MessageID = str[1]*16+str[2];
//  mid[0] = str[3];
//  mid[1] = str[4];
//  mid[2] = str[5];
//  mid[3] = str[6];
  
  switch(MessageID)
  {
    case 0x02:            //���ò���
      {//24,020004 3030303031383035303030303031 01 02D0 0140 001E
        //���
        valueH = str[7]*0x10+str[8];
        valueL = str[9]*0x10+str[10];
        MeterParameter.MeterNumber[0] = ASCLL_to_Int(valueH)*0x10+ASCLL_to_Int(valueL);
        valueH = str[11]*0x10+str[12];
        valueL = str[13]*0x10+str[14];
        MeterParameter.MeterNumber[1] = ASCLL_to_Int(valueH)*0x10+ASCLL_to_Int(valueL);
        valueH = str[15]*0x10+str[16];
        valueL = str[17]*0x10+str[18];
        MeterParameter.MeterNumber[2] = ASCLL_to_Int(valueH)*0x10+ASCLL_to_Int(valueL);
        valueH = str[19]*0x10+str[20];
        valueL = str[21]*0x10+str[22];
        MeterParameter.MeterNumber[3] = ASCLL_to_Int(valueH)*0x10+ASCLL_to_Int(valueL);
        valueH = str[23]*0x10+str[24];
        valueL = str[25]*0x10+str[26];
        MeterParameter.MeterNumber[4] = ASCLL_to_Int(valueH)*0x10+ASCLL_to_Int(valueL);
        valueH = str[27]*0x10+str[28];
        valueL = str[29]*0x10+str[30];
        MeterParameter.MeterNumber[5] = ASCLL_to_Int(valueH)*0x10+ASCLL_to_Int(valueL);
        valueH = str[31]*0x10+str[32];
        valueL = str[33]*0x10+str[34];
        MeterParameter.MeterNumber[6] = ASCLL_to_Int(valueH)*0x10+ASCLL_to_Int(valueL);
        //��������
        MeterParameter.SettleDate = str[35]*0x10+str[36];   //���ý�������    

        //�ϱ�Ƶ��
        MeterParameter.ReportFrequency = str[37]*0x1000+str[38]*0x100+str[39]*0x10+str[40]; //�����ϱ�����

        //�澯��ѹ
        MeterParameter.AlarmVoltage = str[41]*0x1000+str[42]*0x100+str[43]*0x10+str[44];
 
        //����Ƶ��
        MeterParameter.SampleFrequency = str[45]*0x1000+str[46]*0x100+str[47]*0x10+str[48];
        
        Save_Meter_Parameter(); ///�洢ˮ�����
        
        MeterParameter.SampleTiming = 0;
        MeterParameter.ReportTiming = 0;
        
        //0x03��Ӧ
//        ACK(0x03,0x00,mid);
      }
      break;
    case 0x04:           //У׼ˮ��
      {//8,040003010000007B 
        month = str[7]*0x10+str[8];
        if(month == 0)  //У׼��ǰ�ۻ�ˮ��
        {
          Cal.Water_Data.flow8[0] = str[9]*0x10+str[10];
          Cal.Water_Data.flow8[1] = str[11]*0x10+str[12];
          Cal.Water_Data.flow8[2] = str[13]*0x10+str[14];
          Cal.Water_Data.flow8[3] = str[15]*0x10+str[16];
          Save_Add_Flow(ADD_FLOW_ADD,&Cal.Water_Data);
        }
        else if(month <= 13)  //У׼�������ۻ�ˮ��
        {
          Flow.flow8[0] = str[9]*0x10+str[10];
          Flow.flow8[1] = str[11]*0x10+str[12];
          Flow.flow8[2] = str[13]*0x10+str[14];
          Flow.flow8[3] = str[15]*0x10+str[16];
          Save_Add_Flow(SDCF1_ADDR+4*(month-1),&Flow);
        }
        //0x05��Ӧ
//        ACK(0x05,0x00,mid);
      } 
      break;
    case 0x06:           //У׼ʱ��
      {//17,060001 3230313830313031303030303030
        //ʱ��       
        RTC_Date.RTC_Year = str[12]*10+str[14];
        RTC_Date.RTC_Month = (RTC_Month_TypeDef)(str[16]*10+str[18]);
        RTC_Date.RTC_Date = str[20]*10+str[22];
        RTC_Date.RTC_WeekDay = RTC_Weekday_Monday;
        
        RTC_Time.RTC_Hours   = str[24]*10+str[26];
        RTC_Time.RTC_Minutes = str[28]*10+str[30];
        RTC_Time.RTC_Seconds = str[32]*10+str[34];
        
        RTC_SetDate(RTC_Format_BIN, &RTC_Date);
        RTC_SetTime(RTC_Format_BIN, &RTC_Time);
        //0x05��Ӧ
//        ACK(0x07,0x00,mid);
      } 
      break;
    case 0x09:           //������ɢ�ϱ���ʼʱ�䡢��ɢ�ϱ�ʱ��
      {
        MeterParameter.DRStartTimeHour = str[7]*0x10+str[8];
        MeterParameter.DRStartTimeMinute = str[9]*0x10+str[10];
        MeterParameter.DRDuration = str[11]*0x1000+str[12]*0x100+str[13]*0x10+str[14];
        Save_Meter_Parameter(); ///�洢ˮ�����
        if(MeterParameter.DRDuration > 0)
        {
          MeterParameter.DRValid = TRUE;
          MeterParameter.ReportRandTime = 0;
          MeterParameter.ReportRandTiming = 0;
        }
        
        //0x0A��Ӧ
//        ACK(0x0A,0x00,mid);
      } 
      break;
    default:
      break;
  }
}
/*********************************************************************************
 Function:      //
 Description:   //������Ϣ����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void Send_Data_Process(void)
{  
  switch(BC95.ReportBit)
  {
    case 1:            //����ȫ������
      {
        Report_All_Parameters();
        Create_Timer(ONCE,10000,
                     BC95_Timeout_CallBack,0,PROCESS);
      }
      break;
    case 2:            //������ʷ�ۻ�����
      {
        Report_HC_Flow();
        Create_Timer(ONCE,10000,
                     BC95_Timeout_CallBack,0,PROCESS);
      }
      break;
    case 3:            //������ʷ����
      {       
        if(HistoryData.Total > 0)
        {
          Report_History_Data();
          Create_Timer(ONCE,10000,
                       BC95_Timeout_CallBack,0,PROCESS);
        }
        else
        {
          HistoryData.Rear = HistoryData.Front;
          WriteRom(HISTORY_DATA_FRONT_ADDR,&HistoryData.Front,3);
              
          BC95.ReportBit++;
          BC95.IncidentPend = TRUE;
        }
      }
      break;
    default:
      {
        BC95.ReportBit = 0;
        BC95.StartProcess = BC95_POWER_DOWN;
        Create_Timer(ONCE,5000,                            //�ȴ�5s�ػ�
                     BC95_Delay_CallBack,0,PROCESS);
        Save_DebugReportStatistics(1);      //�ɹ�����+1
        Save_DebugReportRecord(1);           //����ɹ���¼
      }
      break;
  }
}
/*********************************************************************************
 Function:      //
 Description:   //������Ӧ
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void ACK(u8 messageId,u8 errcode,u8 mid[4])
{
  uint8_t data[64] = "AT+NMGS=4,00000000\r\n";
  
  data[10] = Int_to_ASCLL(messageId/0x10);
  data[11] = Int_to_ASCLL(messageId%0x10);
  data[12] = Int_to_ASCLL(errcode/0x10);
  data[13] = Int_to_ASCLL(errcode%0x10);
  data[14] = Int_to_ASCLL(mid[0]);
  data[15] = Int_to_ASCLL(mid[1]);
  data[16] = Int_to_ASCLL(mid[2]);
  data[17] = Int_to_ASCLL(mid[3]);
  
  BC95_Data_Send(data,20);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void Report_All_Parameters(void)
{
  uint8_t data[180] = "AT+NMGS=79,00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\r\n";
  union flow_union LastMonthFlow;     //��һ�½������ۻ�����
  uint8_t month,date,hour,minute,second;
  unsigned short year = 0;
  uint8_t valueH = 0,valueL = 0;
   
  //��ȡʱ��   
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
  year = RTC_DateStr.RTC_Year+2000;
  month = RTC_DateStr.RTC_Month;
  date = RTC_DateStr.RTC_Date;
  hour = RTC_TimeStr.RTC_Hours;
  minute = RTC_TimeStr.RTC_Minutes;
  second = RTC_TimeStr.RTC_Seconds;
  //��ȡ��һ���ۼ�����
  Read_ACUM_Flow(SDCF1_ADDR,&LastMonthFlow);
  
  //�����¶�
  Read_Temp();
  
  //������ѹ
  Read_Voltage();
  
  //��ǰ�ۻ�����
  data[13] = Int_to_ASCLL(Cal.Water_Data.flow8[0]/0x10);
  data[14] = Int_to_ASCLL(Cal.Water_Data.flow8[0]%0x10);
  data[15] = Int_to_ASCLL(Cal.Water_Data.flow8[1]/0x10);
  data[16] = Int_to_ASCLL(Cal.Water_Data.flow8[1]%0x10);
  data[17] = Int_to_ASCLL(Cal.Water_Data.flow8[2]/0x10);
  data[18] = Int_to_ASCLL(Cal.Water_Data.flow8[2]%0x10);
  data[19] = Int_to_ASCLL(Cal.Water_Data.flow8[3]/0x10);
  data[20] = Int_to_ASCLL(Cal.Water_Data.flow8[3]%0x10);

  //�������ۻ�����
  data[21] = Int_to_ASCLL(LastMonthFlow.flow8[0]/0x10);
  data[22] = Int_to_ASCLL(LastMonthFlow.flow8[0]%0x10);
  data[23] = Int_to_ASCLL(LastMonthFlow.flow8[1]/0x10);
  data[24] = Int_to_ASCLL(LastMonthFlow.flow8[1]%0x10);
  data[25] = Int_to_ASCLL(LastMonthFlow.flow8[2]/0x10);
  data[26] = Int_to_ASCLL(LastMonthFlow.flow8[2]%0x10);
  data[27] = Int_to_ASCLL(LastMonthFlow.flow8[3]/0x10);
  data[28] = Int_to_ASCLL(LastMonthFlow.flow8[3]%0x10);
  
  //��ص�ѹ
  data[29] = Int_to_ASCLL(MeterParameter.Voltage/0x1000);
  data[30] = Int_to_ASCLL(MeterParameter.Voltage%0x1000/0x100);
  data[31] = Int_to_ASCLL(MeterParameter.Voltage%0x100/0x10);
  data[32] = Int_to_ASCLL(MeterParameter.Voltage%0x10);
  
  //�ź�ǿ��
  data[33] = Int_to_ASCLL(BC95.Rssi/0x10);
  data[34] = Int_to_ASCLL(BC95.Rssi%0x10);
  //�¶� 
  data[35] = Int_to_ASCLL((u8)MeterParameter.Temp/0x10);
  data[36] = Int_to_ASCLL((u8)MeterParameter.Temp%0x10);
  
  //��
  data[37] = Int_to_ASCLL(year/0x1000);
  data[38] = Int_to_ASCLL(year%0x1000/0x100);
  data[39] = Int_to_ASCLL(year%0x100/0x10);
  data[40] = Int_to_ASCLL(year%0x10); 
  //��
  data[41] = Int_to_ASCLL(month/0x10);
  data[42] = Int_to_ASCLL(month%0x10);
  //��
  data[43] = Int_to_ASCLL(date/0x10);
  data[44] = Int_to_ASCLL(date%0x10);
  //ʱ
  data[45] = Int_to_ASCLL(hour/0x10);
  data[46] = Int_to_ASCLL(hour%0x10);
  //��
  data[47] = Int_to_ASCLL(minute/0x10);
  data[48] = Int_to_ASCLL(minute%0x10);
  //��
  data[49] = Int_to_ASCLL(second/0x10);
  data[50] = Int_to_ASCLL(second%0x10);
  //���
  valueH = Int_to_ASCLL(MeterParameter.MeterNumber[0]/0x10);
  valueL = Int_to_ASCLL(MeterParameter.MeterNumber[0]%0x10);
  data[51] = Int_to_ASCLL(valueH/0x10);
  data[52] = Int_to_ASCLL(valueH%0x10);
  data[53] = Int_to_ASCLL(valueL/0x10);
  data[54] = Int_to_ASCLL(valueL%0x10);
  valueH = Int_to_ASCLL(MeterParameter.MeterNumber[1]/0x10);
  valueL = Int_to_ASCLL(MeterParameter.MeterNumber[1]%0x10);
  data[55] = Int_to_ASCLL(valueH/0x10);
  data[56] = Int_to_ASCLL(valueH%0x10);
  data[57] = Int_to_ASCLL(valueL/0x10);
  data[58] = Int_to_ASCLL(valueL%0x10);
  valueH = Int_to_ASCLL(MeterParameter.MeterNumber[2]/0x10);
  valueL = Int_to_ASCLL(MeterParameter.MeterNumber[2]%0x10);
  data[59] = Int_to_ASCLL(valueH/0x10);
  data[60] = Int_to_ASCLL(valueH%0x10);
  data[61] = Int_to_ASCLL(valueL/0x10);
  data[62] = Int_to_ASCLL(valueL%0x10);
  valueH = Int_to_ASCLL(MeterParameter.MeterNumber[3]/0x10);
  valueL = Int_to_ASCLL(MeterParameter.MeterNumber[3]%0x10);
  data[63] = Int_to_ASCLL(valueH/0x10);
  data[64] = Int_to_ASCLL(valueH%0x10);
  data[65] = Int_to_ASCLL(valueL/0x10);
  data[66] = Int_to_ASCLL(valueL%0x10);
  valueH = Int_to_ASCLL(MeterParameter.MeterNumber[4]/0x10);
  valueL = Int_to_ASCLL(MeterParameter.MeterNumber[4]%0x10);
  data[67] = Int_to_ASCLL(valueH/0x10);
  data[68] = Int_to_ASCLL(valueH%0x10);
  data[69] = Int_to_ASCLL(valueL/0x10);
  data[70] = Int_to_ASCLL(valueL%0x10);
  valueH = Int_to_ASCLL(MeterParameter.MeterNumber[5]/0x10);
  valueL = Int_to_ASCLL(MeterParameter.MeterNumber[5]%0x10);
  data[71] = Int_to_ASCLL(valueH/0x10);
  data[72] = Int_to_ASCLL(valueH%0x10);
  data[73] = Int_to_ASCLL(valueL/0x10);
  data[74] = Int_to_ASCLL(valueL%0x10);
  valueH = Int_to_ASCLL(MeterParameter.MeterNumber[6]/0x10);
  valueL = Int_to_ASCLL(MeterParameter.MeterNumber[6]%0x10);
  data[75] = Int_to_ASCLL(valueH/0x10);
  data[76] = Int_to_ASCLL(valueH%0x10);
  data[77] = Int_to_ASCLL(valueL/0x10);
  data[78] = Int_to_ASCLL(valueL%0x10);
   //��������
  data[79] = Int_to_ASCLL(MeterParameter.SettleDate/0x10);
  data[80] = Int_to_ASCLL(MeterParameter.SettleDate%0x10);
  //�ϱ�Ƶ��
  data[81] = Int_to_ASCLL(MeterParameter.ReportFrequency/0x1000);
  data[82] = Int_to_ASCLL(MeterParameter.ReportFrequency%0x1000/0x100);
  data[83] = Int_to_ASCLL(MeterParameter.ReportFrequency%0x100/0x10);
  data[84] = Int_to_ASCLL(MeterParameter.ReportFrequency%0x10);
  //�澯��ѹ
  data[85] = Int_to_ASCLL(MeterParameter.AlarmVoltage/0x1000);
  data[86] = Int_to_ASCLL(MeterParameter.AlarmVoltage%0x1000/0x100);
  data[87] = Int_to_ASCLL(MeterParameter.AlarmVoltage%0x100/0x10);
  data[88] = Int_to_ASCLL(MeterParameter.AlarmVoltage%0x10);
  //����״̬
  data[89] = 0x30;
  data[90] = 0x30;
  //����״̬
  data[91] = Int_to_ASCLL(Cal.Error/0x10);
  data[92] = Int_to_ASCLL(Cal.Error%0x10);
  //IMEI
  data[93] = Int_to_ASCLL(BC95.IMEI[0]/0x10);
  data[94] = Int_to_ASCLL(BC95.IMEI[0]%0x10);
  data[95] = Int_to_ASCLL(BC95.IMEI[1]/0x10);
  data[96] = Int_to_ASCLL(BC95.IMEI[1]%0x10);
  data[97] = Int_to_ASCLL(BC95.IMEI[2]/0x10);
  data[98] = Int_to_ASCLL(BC95.IMEI[2]%0x10);
  data[99] = Int_to_ASCLL(BC95.IMEI[3]/0x10);
  data[100] = Int_to_ASCLL(BC95.IMEI[3]%0x10);
  data[101] = Int_to_ASCLL(BC95.IMEI[4]/0x10);
  data[102] = Int_to_ASCLL(BC95.IMEI[4]%0x10);
  data[103] = Int_to_ASCLL(BC95.IMEI[5]/0x10);
  data[104] = Int_to_ASCLL(BC95.IMEI[5]%0x10);
  data[105] = Int_to_ASCLL(BC95.IMEI[6]/0x10);
  data[106] = Int_to_ASCLL(BC95.IMEI[6]%0x10);
  data[107] = Int_to_ASCLL(BC95.IMEI[7]/0x10);
  data[108] = Int_to_ASCLL(BC95.IMEI[7]%0x10);
  data[109] = Int_to_ASCLL(BC95.IMEI[8]/0x10);
  data[110] = Int_to_ASCLL(BC95.IMEI[8]%0x10);
  data[111] = Int_to_ASCLL(BC95.IMEI[9]/0x10);
  data[112] = Int_to_ASCLL(BC95.IMEI[9]%0x10);
  data[113] = Int_to_ASCLL(BC95.IMEI[10]/0x10);
  data[114] = Int_to_ASCLL(BC95.IMEI[10]%0x10);
  data[115] = Int_to_ASCLL(BC95.IMEI[11]/0x10);
  data[116] = Int_to_ASCLL(BC95.IMEI[11]%0x10);
  data[117] = Int_to_ASCLL(BC95.IMEI[12]/0x10);
  data[118] = Int_to_ASCLL(BC95.IMEI[12]%0x10);
  data[119] = Int_to_ASCLL(BC95.IMEI[13]/0x10);
  data[120] = Int_to_ASCLL(BC95.IMEI[13]%0x10);
  data[121] = Int_to_ASCLL(BC95.IMEI[14]/0x10);
  data[122] = Int_to_ASCLL(BC95.IMEI[14]%0x10);
  //ICCID
  data[123] = Int_to_ASCLL(BC95.ICCID[0]/0x10);
  data[124] = Int_to_ASCLL(BC95.ICCID[0]%0x10);
  data[125] = Int_to_ASCLL(BC95.ICCID[1]/0x10);
  data[126] = Int_to_ASCLL(BC95.ICCID[1]%0x10);
  data[127] = Int_to_ASCLL(BC95.ICCID[2]/0x10);
  data[128] = Int_to_ASCLL(BC95.ICCID[2]%0x10);
  data[129] = Int_to_ASCLL(BC95.ICCID[3]/0x10);
  data[130] = Int_to_ASCLL(BC95.ICCID[3]%0x10);
  data[131] = Int_to_ASCLL(BC95.ICCID[4]/0x10);
  data[132] = Int_to_ASCLL(BC95.ICCID[4]%0x10);
  data[133] = Int_to_ASCLL(BC95.ICCID[5]/0x10);
  data[134] = Int_to_ASCLL(BC95.ICCID[5]%0x10);
  data[135] = Int_to_ASCLL(BC95.ICCID[6]/0x10);
  data[136] = Int_to_ASCLL(BC95.ICCID[6]%0x10);
  data[137] = Int_to_ASCLL(BC95.ICCID[7]/0x10);
  data[138] = Int_to_ASCLL(BC95.ICCID[7]%0x10);
  data[139] = Int_to_ASCLL(BC95.ICCID[8]/0x10);
  data[140] = Int_to_ASCLL(BC95.ICCID[8]%0x10);
  data[141] = Int_to_ASCLL(BC95.ICCID[9]/0x10);
  data[142] = Int_to_ASCLL(BC95.ICCID[9]%0x10);
  data[143] = Int_to_ASCLL(BC95.ICCID[10]/0x10);
  data[144] = Int_to_ASCLL(BC95.ICCID[10]%0x10);
  data[145] = Int_to_ASCLL(BC95.ICCID[11]/0x10);
  data[146] = Int_to_ASCLL(BC95.ICCID[11]%0x10);
  data[147] = Int_to_ASCLL(BC95.ICCID[12]/0x10);
  data[148] = Int_to_ASCLL(BC95.ICCID[12]%0x10);
  data[149] = Int_to_ASCLL(BC95.ICCID[13]/0x10);
  data[150] = Int_to_ASCLL(BC95.ICCID[13]%0x10);
  data[151] = Int_to_ASCLL(BC95.ICCID[14]/0x10);
  data[152] = Int_to_ASCLL(BC95.ICCID[14]%0x10);
  data[153] = Int_to_ASCLL(BC95.ICCID[15]/0x10);
  data[154] = Int_to_ASCLL(BC95.ICCID[15]%0x10);
  data[155] = Int_to_ASCLL(BC95.ICCID[16]/0x10);
  data[156] = Int_to_ASCLL(BC95.ICCID[16]%0x10);
  data[157] = Int_to_ASCLL(BC95.ICCID[17]/0x10);
  data[158] = Int_to_ASCLL(BC95.ICCID[17]%0x10);
  data[159] = Int_to_ASCLL(BC95.ICCID[18]/0x10);
  data[160] = Int_to_ASCLL(BC95.ICCID[18]%0x10);
  data[161] = Int_to_ASCLL(BC95.ICCID[19]/0x10);
  data[162] = Int_to_ASCLL(BC95.ICCID[19]%0x10);
  
  //�ϴ�����������Ϣ
  valueH = *((const unsigned char *)(BC95_ERROR_RECORD_ADD));
  data[163] = Int_to_ASCLL(valueH/0x10);
  data[164] = Int_to_ASCLL(valueH%0x10);
  //����Ƶ��
  data[165] = Int_to_ASCLL(MeterParameter.SampleFrequency/0x1000);
  data[166] = Int_to_ASCLL(MeterParameter.SampleFrequency%0x1000/0x100);
  data[167] = Int_to_ASCLL(MeterParameter.SampleFrequency%0x100/0x10);
  data[168] = Int_to_ASCLL(MeterParameter.SampleFrequency%0x10);
  
  BC95_Data_Send(data,171);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void Report_HC_Flow(void)
{
  uint8_t data[128] = "AT+NMGS=53,0100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\r\n";
  union flow_union flow;
  
  //��1���ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF1_ADDR,&flow);
  data[13] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[14] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[15] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[16] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[17] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[18] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[19] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[20] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��2�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF2_ADDR,&flow);
  data[21] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[22] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[23] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[24] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[25] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[26] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[27] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[28] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��3�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF3_ADDR,&flow);
  data[29] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[30] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[31] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[32] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[33] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[34] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[35] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[36] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��4�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF4_ADDR,&flow);
  data[37] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[38] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[39] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[40] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[41] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[42] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[43] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[44] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��5�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF5_ADDR,&flow);
  data[45] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[46] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[47] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[48] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[49] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[50] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[51] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[52] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��6�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF6_ADDR,&flow);
  data[53] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[54] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[55] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[56] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[57] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[58] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[59] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[60] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��7�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF7_ADDR,&flow);
  data[61] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[62] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[63] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[64] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[65] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[66] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[67] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[68] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��8�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF8_ADDR,&flow);
  data[69] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[70] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[71] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[72] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[73] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[74] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[75] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[76] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��9�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF9_ADDR,&flow);
  data[77] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[78] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[79] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[80] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[81] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[82] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[83] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[84] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��10�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF10_ADDR,&flow);
  data[85] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[86] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[87] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[88] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[89] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[90] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[91] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[92] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��11�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF11_ADDR,&flow);
  data[93] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[94] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[95] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[96] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[97] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[98] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[99] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[100] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��12�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF12_ADDR,&flow); 
  data[101] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[102] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[103] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[104] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[105] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[106] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[107] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[108] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  //��13�½������ۻ�����
  flow.flow32 = 0;
  Read_ACUM_Flow(SDCF13_ADDR,&flow);
  data[109] = Int_to_ASCLL(flow.flow8[0]/0x10);
  data[110] = Int_to_ASCLL(flow.flow8[0]%0x10);
  data[111] = Int_to_ASCLL(flow.flow8[1]/0x10);
  data[112] = Int_to_ASCLL(flow.flow8[1]%0x10);
  data[113] = Int_to_ASCLL(flow.flow8[2]/0x10);
  data[114] = Int_to_ASCLL(flow.flow8[2]%0x10);
  data[115] = Int_to_ASCLL(flow.flow8[3]/0x10);
  data[116] = Int_to_ASCLL(flow.flow8[3]%0x10);
  
  BC95_Data_Send(data,119);
}
/*********************************************************************************
 Function:      //
 Description:   //�ϱ���ʷ����
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void Report_History_Data(void)
{
  uint8_t data[40] = "AT+NMGS=12,080000000000000000000000\r\n";
  unsigned char buff[9] = {0};
    
  Read_History_Data(buff);

  //�ۻ�����
  data[13] = Int_to_ASCLL(buff[0]/0x10);
  data[14] = Int_to_ASCLL(buff[0]%0x10);
  data[15] = Int_to_ASCLL(buff[1]/0x10);
  data[16] = Int_to_ASCLL(buff[1]%0x10);
  data[17] = Int_to_ASCLL(buff[2]/0x10);
  data[18] = Int_to_ASCLL(buff[2]%0x10);
  data[19] = Int_to_ASCLL(buff[3]/0x10);
  data[20] = Int_to_ASCLL(buff[3]%0x10);
  //��
  data[21] = Int_to_ASCLL((buff[4]+2000)/0x1000);
  data[22] = Int_to_ASCLL((buff[4]+2000)%0x1000/0x100);
  data[23] = Int_to_ASCLL((buff[4]+2000)%0x100/0x10);
  data[24] = Int_to_ASCLL((buff[4]+2000)%0x10); 
  //��
  data[25] = Int_to_ASCLL(buff[5]/0x10);
  data[26] = Int_to_ASCLL(buff[5]%0x10);
  //��
  data[27] = Int_to_ASCLL(buff[6]/0x10);
  data[28] = Int_to_ASCLL(buff[6]%0x10);
  //ʱ
  data[29] = Int_to_ASCLL(buff[7]/0x10);
  data[30] = Int_to_ASCLL(buff[7]%0x10);
  //��
  data[31] = Int_to_ASCLL(buff[8]/0x10);
  data[32] = Int_to_ASCLL(buff[8]%0x10);
  //��
  data[33] = 0x30;
  data[34] = 0x30;
  
  BC95_Data_Send(data,37);
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
}
/******************************************END********************************************************/