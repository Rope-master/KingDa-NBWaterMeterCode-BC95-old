/*********************************************************************************************************
//������
//���ߣ����ƮԾ    ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************************************/
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
 Description:   //��ROMд����
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void WriteRom (unsigned short addr, void *pbuff, unsigned char length)
{
 unsigned char itemp;
 unsigned char cnt;
 unsigned char *p; 
 unsigned short itemp16;
 do
 {FLASH_Unlock(FLASH_MemType_Data); }//����EEPROM���
 while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET); 
 itemp = 0;
 p = pbuff;
 cnt =0 ;
 while (itemp < length)
 {
   FLASH_ProgramByte(addr ,p[itemp]);
   itemp16 = 0xffff;
   while(FLASH_GetFlagStatus(FLASH_FLAG_EOP) == RESET)
   {
    itemp16 --;
    if(itemp16 <= 1)
    {break;}
   }
   itemp16 = FLASH_ReadByte(addr);
   if(p[itemp] != itemp16)
   {
    if(++cnt > 50)
    {
     FLASH_Lock(FLASH_MemType_Data);
     break;
    }
    continue ; 
   }
   else
   {cnt = 0;itemp ++;addr ++;}
 }
 FLASH_Lock(FLASH_MemType_Data);
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡˮ�����
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void Read_Meter_Parameter(void)
{
  Read_ACUM_Flow(ADD_FLOW_ADD,&Cal.Water_Data);         //��ȡ��ǰ�ۻ�����
  //��ȡ���
  MeterParameter.MeterNumber[0] = *((const unsigned char *)(CJT_188_ADD));
  MeterParameter.MeterNumber[1] = *((const unsigned char *)(CJT_188_ADD+1));
  MeterParameter.MeterNumber[2] = *((const unsigned char *)(CJT_188_ADD+2));
  MeterParameter.MeterNumber[3] = *((const unsigned char *)(CJT_188_ADD+3));
  MeterParameter.MeterNumber[4] = *((const unsigned char *)(CJT_188_ADD+4));
  MeterParameter.MeterNumber[5] = *((const unsigned char *)(CJT_188_ADD+5));
  MeterParameter.MeterNumber[6] = *((const unsigned char *)(CJT_188_ADD+6));
  //��ȡ�澯��ѹ
//  MeterParameter.AlarmVoltage = *((const unsigned short *)(BAT_ALARM_ADD));
//  if(MeterParameter.AlarmVoltage == 0)
  {
    MeterParameter.AlarmVoltage = 330;
  }

  //��ȡ������
  MeterParameter.SettleDate = *((const unsigned char *)(SETTLE_DATE_ADD));
  if((MeterParameter.SettleDate == 0)||(MeterParameter.SettleDate > 31))//Ĭ�Ͻ�������1��
  {
    MeterParameter.SettleDate = 1;
  }
  //��ȡ�ϱ�Ƶ��
  MeterParameter.ReportFrequency = *((const unsigned short *)(REPORT_FREQUENCY_ADDR));
  if(MeterParameter.ReportFrequency == 0)
  {
    MeterParameter.ReportFrequency = 1440;
  }

  //��ȡ����Ƶ��
  MeterParameter.SampleFrequency = *((const unsigned short *)(SAMPLE_FREQUENCY_ADDR));
  
  //��ȡ�׷�ʱ��
  MeterParameter.DRStartTimeHour = *((const unsigned char *)(FIRST_REPORT_ADDR));
  MeterParameter.DRStartTimeMinute = *((const unsigned char *)(FIRST_REPORT_ADDR+1));
  MeterParameter.DRDuration = *((const unsigned short *)(FIRST_REPORT_ADDR+2));
  if(MeterParameter.DRDuration > 0)
  {
    MeterParameter.DRValid = TRUE;
    MeterParameter.ReportRandTime = 0;
    MeterParameter.ReportRandTiming = 0;
  }
  
  //��ȡ��ʷ�����ס�β������
  HistoryData.Front = *((const unsigned char *)(HISTORY_DATA_FRONT_ADDR));
  HistoryData.Rear = *((const unsigned char *)(HISTORY_DATA_REAR_ADDR));
  HistoryData.Total = *((const unsigned char *)(HISTORY_DATA_TOTAL_ADDR));
}
/*********************************************************************************
 Function:      //
 Description:   //�洢ˮ�����
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void Save_Meter_Parameter(void)
{
  //�洢���
  WriteRom(CJT_188_ADD,MeterParameter.MeterNumber,7);
//  //�洢��ѹ�澯ֵ
//  WriteRom(BAT_ALARM_ADD,&MeterParameter.AlarmVoltage,2);
  //�洢��������
  WriteRom (SETTLE_DATE_ADD,&MeterParameter.SettleDate,1);
  //�洢�ϱ�Ƶ��
  WriteRom(REPORT_FREQUENCY_ADDR,&MeterParameter.ReportFrequency,2);
  //�洢����Ƶ��
  WriteRom(SAMPLE_FREQUENCY_ADDR,&MeterParameter.SampleFrequency,2);
  //�洢�׷�ʱ��
  WriteRom(FIRST_REPORT_ADDR,&MeterParameter.DRStartTimeHour,1);
  WriteRom((FIRST_REPORT_ADDR+1),&MeterParameter.DRStartTimeMinute,1);
  WriteRom((FIRST_REPORT_ADDR+2),&MeterParameter.DRDuration,2);
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ�ۻ�ˮ��
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void Read_ACUM_Flow(unsigned short addr,union flow_union *Flow)       
{
  union flow_union *flow;
  flow = (union flow_union *)(addr);
  Flow->flow32 = flow->flow32;
}
/*********************************************************************************
 Function:      //
 Description:   //�洢�ۻ�ˮ��
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void Save_Add_Flow(u16 addr,union flow_union *Flow)       
{
    WriteRom (addr,Flow->flow8,4);      
}
/*********************************************************************************
 Function:      //
 Description:   //�洢�������ۻ�ˮ��
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void Save_SDCF_Flow(union flow_union *Flow)    
{
  union flow_union *Water;
  
  Water = (union flow_union *)(SDCF12_ADDR); 
  WriteRom (SDCF13_ADDR,Water->flow8,4);      //д�ۻ�����
  
  Water = (union flow_union *)(SDCF11_ADDR); 
  WriteRom (SDCF12_ADDR,Water->flow8,4);      //д�ۻ�����
  
  Water = (union flow_union *)(SDCF10_ADDR); 
  WriteRom (SDCF11_ADDR,Water->flow8,4);      //д�ۻ�����
  
  Water = (union flow_union *)(SDCF9_ADDR); 
  WriteRom (SDCF10_ADDR,Water->flow8,4);      //д�ۻ�����
  
  Water = (union flow_union *)(SDCF8_ADDR); 
  WriteRom (SDCF9_ADDR,Water->flow8,4);      //д�ۻ�����
  
  Water = (union flow_union *)(SDCF7_ADDR); 
  WriteRom (SDCF8_ADDR,Water->flow8,4);      //д�ۻ�����
  
  Water = (union flow_union *)(SDCF6_ADDR); 
  WriteRom (SDCF7_ADDR,Water->flow8,4);      //д�ۻ�����
  
  Water = (union flow_union *)(SDCF5_ADDR); 
  WriteRom (SDCF6_ADDR,Water->flow8,4);      //д�ۻ�����
  
  Water = (union flow_union *)(SDCF4_ADDR); 
  WriteRom (SDCF5_ADDR,Water->flow8,4);      //д�ۻ�����
  
  Water = (union flow_union *)(SDCF3_ADDR); 
  WriteRom (SDCF4_ADDR,Water->flow8,4);      //д�ۻ�����
  
  Water = (union flow_union *)(SDCF2_ADDR); 
  WriteRom (SDCF3_ADDR,Water->flow8,4);      //д�ۻ�����
  
  Water = (union flow_union *)(SDCF1_ADDR); 
  WriteRom (SDCF2_ADDR,Water->flow8,4);      //д�ۻ�����
  
  WriteRom (SDCF1_ADDR,Flow->flow8,4);      //д�ۻ�����
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ��ʷ����
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void Read_History_Data(unsigned char* buff)
{
  unsigned char j = 0;
  uint16_t addr = 0;
  
  addr = HISTORY_DATA_START_ADDR + HistoryData.Front*HistoryDataSize;
  for(j = 0;j < 9;j++)
  {    
    buff[j] = *((const unsigned char *)(addr+j));
  }
}
/*********************************************************************************
 Function:      //
 Description:   //������ʷ����
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void Save_History_Data(void)
{
  uint32_t addr = 0;
  unsigned char buff[9] = {0};
 
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
  
  buff[0] = Cal.Water_Data.flow8[0];
  buff[1] = Cal.Water_Data.flow8[1]; 
  buff[2] = Cal.Water_Data.flow8[2];
  buff[3] = Cal.Water_Data.flow8[3];
  buff[4] = RTC_DateStr.RTC_Year;
  buff[5] = RTC_DateStr.RTC_Month;
  buff[6] = RTC_DateStr.RTC_Date;
  buff[7] = RTC_TimeStr.RTC_Hours;
  buff[8] = RTC_TimeStr.RTC_Minutes; 
  
  addr = HISTORY_DATA_START_ADDR + HistoryData.Rear*HistoryDataSize;
  WriteRom(addr,buff,9);

  if(HistoryData.Total < HistoryDataMaxNum)
  {
    HistoryData.Total++;
  }
  else
  {
    HistoryData.Front = (HistoryData.Rear+1)%HistoryDataMaxNum;
  }
  HistoryData.Rear = (HistoryData.Rear+1)%HistoryDataMaxNum;
  
  WriteRom(HISTORY_DATA_FRONT_ADDR,&HistoryData.Front,3);
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡBC95���ϼ�¼
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void Read_BC95_Error_Record(void)
{
  BC95.ErrorStep = *((const unsigned char *)(BC95_ERROR_RECORD_ADD));
  BC95.ErrorCode = *((const unsigned short *)(BC95_ERROR_RECORD_ADD+1));
}
/*********************************************************************************
 Function:      //
 Description:   //����BC95���ϼ�¼
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void Save_BC95_Error_Record(void)
{
  WriteRom(BC95_ERROR_RECORD_ADD,&BC95.ErrorStep,1);
  WriteRom(BC95_ERROR_RECORD_ADD+1,&BC95.ErrorCode,2);
}

/*****************************************************************************************************/
/******************************************DEBUG��ض�д**********************************************/
/*****************************************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //������Ը�λ��¼
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void Save_DebugResetRecord(void)
{
  uint8_t buff[10] = {0};
  uint16_t num = 0;
  RTC_DateTypeDef date;    
  RTC_TimeTypeDef time;

  //��ȡʱ��   
  RTC_GetDate(RTC_Format_BIN, &date);
  RTC_GetTime(RTC_Format_BIN, &time);
  
  num = *((const unsigned short *)(DEBUG_RESET_RECORD_ADDR));
  num++;
  
  buff[0] = num/0x100;
  buff[1] = num%0x100; 
  buff[2] = date.RTC_Year;
  buff[3] = date.RTC_Month;
  buff[4] = date.RTC_Date;
  buff[5] = time.RTC_Hours;
  buff[6] = time.RTC_Minutes; 
  buff[7] = time.RTC_Seconds; 
  
  WriteRom(DEBUG_RESET_RECORD_ADDR,buff,8);
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ���Ը�λ��¼
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void Read_DebugResetRecord(uint8_t *buff)
{
  uint8_t i = 0;

  for(i = 0;i < 8;i++)
  {
    buff[i] = *((const unsigned char *)(DEBUG_RESET_RECORD_ADDR+i));
  }
}  
/*********************************************************************************
 Function:      //
 Description:   //�����ϴ�ͳ��
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void Save_DebugReportStatistics(uint8_t flag)
{
  uint16_t num = 0;
  uint16_t addr = 0;
  
  addr = DEBUG_REPORT_STATISTICS_ADDR + flag*2;
  
  num = *((const unsigned short *)(addr));
  num++;
  WriteRom(addr,&num,2);
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ�ϴ�ͳ��
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void Read_DebugReportStatistics(uint8_t *buff)
{
  uint8_t i = 0;

  for(i = 0;i < 6;i++)
  {
    buff[i] = *((const unsigned char *)(DEBUG_REPORT_STATISTICS_ADDR+i));
  }
}
/*********************************************************************************
 Function:      //
 Description:   //�����ϴ���¼
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void Save_DebugReportRecord(uint8_t flag)
{
  uint8_t buff[10] = {0};
  uint16_t addr = 0;
  RTC_DateTypeDef date;    
  RTC_TimeTypeDef time;
 
  if(flag == 0)         //��������ʱ��
  { 
    addr = DEBUG_REPORT_RECORD_ADDR;
    RTC_GetDate(RTC_Format_BIN, &date);
    RTC_GetTime(RTC_Format_BIN, &time);
    buff[0] = date.RTC_Year;
    buff[1] = date.RTC_Month;
    buff[2] = date.RTC_Date;
    buff[3] = time.RTC_Hours;
    buff[4] = time.RTC_Minutes; 
    buff[5] = time.RTC_Seconds; 
    WriteRom(addr,buff,10); 
  }
  else if(flag == 1)    //�������ɹ�
  {
    addr = DEBUG_REPORT_RECORD_ADDR+6;
    buff[0] = 1;
    WriteRom(addr,buff,1); 
  }
  else if(flag == 2)    //������ʧ��
  {
    addr = DEBUG_REPORT_RECORD_ADDR+6;
    buff[0] = 2;
    buff[1] = BC95.ErrorStep;
    buff[2] = BC95.ErrorCode/0x100;
    buff[3] = BC95.ErrorCode%0x100;
    WriteRom(addr,buff,4); 
  }
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ�ϴ���¼
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void Read_DebugReportRecord(uint8_t *buff)
{
  uint8_t i = 0;

  for(i = 0;i < 10;i++)
  {
    buff[i] = *((const unsigned char *)(DEBUG_REPORT_RECORD_ADDR+i));
  }
}
/******************************************END********************************************************/
