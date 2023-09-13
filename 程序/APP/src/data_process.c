/*******************************************************************************
Copyright: 
File name: uplink_protocol.h
Description: ����ͨ��Э����غ���������
Author: ��ѧ��cy
Version: 
Date: 
History:       
*******************************************************************************/
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include "data_process.h"
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
 Description:   //������
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
void DataProcess_ReadData(uint32_t addr, uint8_t *buff, uint16_t length)
{
  Flash_Read(addr, buff, length);
}
/*******************************************************************************
 Function:      //
 Description:   //д����
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
void DataProcess_WriteData(uint32_t addr, uint8_t *buff, uint16_t length)
{
  Flash_Write(addr, buff, length);
}
/*******************************************************************************
 Function:      //
 Description:   //���EEPROMǰ64�ֽ�
                //ǰ2�ֽڲ�ʹ�ã���ǰ2�ֽڲ�ȫΪ0�������EEPROMǰ64�ֽ�
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void DataProcess_Clear64Bytes(void)
{
  uint8_t temp[64] = {0};

  DataProcess_ReadData(INTERNAL_EEPROM_START_ADDR,temp,2);
  if( (temp[0] != 0xAA)||(temp[1] != 0xBB) )
  {
    temp[0] = 0xAA;
    temp[1] = 0xBB;
    DataProcess_WriteData(INTERNAL_EEPROM_START_ADDR,temp,64);
  }
}
/*******************************************************************************
 Function:      //
 Description:   //дˮ��
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void DataProcess_WriteFlow(void)
{
  DataProcess_WriteData(FORWARD_FLOW_ADDR, DevicePara.ForwardFlow.flow8, 4);  
}
/*******************************************************************************
 Function:      //
 Description:   //дIMEI
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void DataProcess_WriteIMEI(void)
{
  uint8_t imei[15] = {0};
  
  DataProcess_ReadData(IMEI_ADDR,imei,15);  
  if( 0 != memcmp(BC95.IMEI, imei, 15) )
  {
    DataProcess_WriteData(IMEI_ADDR, BC95.IMEI, 15); 
  } 
}
/*********************************************************************************
 Function:      //
 Description:   //�ж��Ƿ����¶�����
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
void DataProcess_CheckRereadParas(void)
{
  RTC_DateTypeDef date;
  
  RTC_GetDate(RTC_Format_BIN, &date);
  if(date.RTC_Year < 20)
  {
    DevicePara.IsRereadParas = TRUE;
  }
}
/*********************************************************************************
 Function:      //
 Description:   //���豸����
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
void DataProcess_ReadDevicePara(void)
{
  /*��ˮ��*/
  DataProcess_ReadData(FORWARD_FLOW_ADDR,DevicePara.ForwardFlow.flow8,4);  
  
  /*���ϱ�����*/
  DataProcess_ReadReportParas();
  
  /*����������*/
  DataProcess_ReadSampleParas();
}
/*********************************************************************************
 Function:      //
 Description:   //���ϱ�����
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
void DataProcess_ReadReportParas(void)
{
  uint8_t imei[15] = {0};
  uint8_t data[32] = {0};
  uint16_t offsetTime = 0;
  uint8_t offsetHour = 0;
  uint8_t offsetMinute = 0;
  uint16_t temp = 0;
  
  DataProcess_ReadData(IMEI_ADDR, imei, 15);
  offsetTime = GetOffsetTime(&imei[11]);
  
  DataProcess_ReadData(REPORT_FREQUENCY_ADDR,data,6);  

  temp = data[0]*0x100 + data[1];
  if( (temp > 0)&&(temp < 0xFFFF) )
  {
    DevicePara.ReportParas.Frequency = temp;
  }
  else
  {
    DevicePara.ReportParas.Frequency = 1440;
  }
  
  temp = data[4]*0x100 + data[5];
  if( (data[2] < 24)&&(data[3] < 60)
    &&(temp > 0)&&(temp <= 1440) )
  {
    DevicePara.ReportParas.StartTimeHour = data[2];
    DevicePara.ReportParas.StartTimeMinute = data[3];
    DevicePara.ReportParas.DRDuration = temp;
  }
  else
  {
    DevicePara.ReportParas.StartTimeHour = 0;
    DevicePara.ReportParas.StartTimeMinute = 0;
    DevicePara.ReportParas.DRDuration = 480;
  }
  
  offsetHour = offsetTime%DevicePara.ReportParas.DRDuration/60;
  offsetMinute = offsetTime%DevicePara.ReportParas.DRDuration%60;
  
  DevicePara.ReportParas.StartTimeHour = (DevicePara.ReportParas.StartTimeHour+offsetHour)%24;
  DevicePara.ReportParas.StartTimeMinute = (DevicePara.ReportParas.StartTimeMinute+offsetMinute)%60;
  DevicePara.ReportParas.StartTimeValid = TRUE;
  DevicePara.ReportParas.Timer = 0;
}
/*********************************************************************************
 Function:      //
 Description:   //����������
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
void DataProcess_ReadSampleParas(void)
{
  uint8_t data[16] = {0};
  CircularQueue_TypeDef queue;
  
  DataProcess_ReadData(SAMPLE_FREQUENCY_ADDR,data,8); 
  memcpy(&queue.Total,&data[2],6);
  if( ((queue.Front+queue.Total)%SampleDataMax) ==  queue.Rear)
  {
    memcpy(&DevicePara.SampleParas.Queue.Total,&queue.Total,6);
  }
  else
  {
    memset(&DevicePara.SampleParas.Queue.Total,0,6);
  }
  DevicePara.SampleParas.Frequency = data[0]*0x100 + data[1];
  DevicePara.SampleParas.Timer = 0;
  DevicePara.SampleParas.Valid = FALSE;
}
/*******************************************************************************
 Function:      //
 Description:   //д��������
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
void DataPocess_WriteSampleData(void)
{
  uint8_t data[16] = {0};
  uint32_t addr = 0;
  RTC_DateTypeDef Date;
  RTC_TimeTypeDef Time;
  
  addr = SAMPLE_DATA_START_ADDR + DevicePara.SampleParas.Queue.Rear*SampleDataEachSize;      //�������ݴ洢��ַ

  RTC_GetDate(RTC_Format_BIN, &Date);
  RTC_GetTime(RTC_Format_BIN, &Time);

  memcpy(data,DevicePara.ForwardFlow.flow8,4);
  data[4] = Date.RTC_Year;
  data[5] = Date.RTC_Month;
  data[6] = Date.RTC_Date;
  data[7] = Time.RTC_Hours;
  data[8] = Time.RTC_Minutes;

  DataProcess_WriteData(addr,data,9);
 
  DevicePara.SampleParas.Queue.Rear = (DevicePara.SampleParas.Queue.Rear+1)%SampleDataMax;
  if(DevicePara.SampleParas.Queue.Total < SampleDataMax)
  {
    DevicePara.SampleParas.Queue.Total++;
  }
  else
  {
    DevicePara.SampleParas.Queue.Front = DevicePara.SampleParas.Queue.Rear;
  }
  DataProcess_WriteData(SAMPLE_DATA_TOTAL_ADDR, (uint8_t *)&DevicePara.SampleParas.Queue.Total, 6);
}
/*******************************************************************************
 Function:      //
 Description:   //����������
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
void DataPocess_ReadSampleData(uint8_t *buff,uint8_t number)
{
  uint32_t addr = 0;

  addr = SAMPLE_DATA_START_ADDR + DevicePara.SampleParas.Queue.Front*SampleDataEachSize;      //�������ݶ���ַ
  DataProcess_ReadData(addr, buff, number*SampleDataEachSize);
}
/*******************************************************************************
 Function:      //
 Description:   //����������ݰ�
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
void DataPocess_ClearSampleData(uint8_t number)
{
  DevicePara.SampleParas.Queue.Total = DevicePara.SampleParas.Queue.Total - number;
  if(DevicePara.SampleParas.Queue.Total > 0)
  {
    DevicePara.SampleParas.Queue.Front = (DevicePara.SampleParas.Queue.Front+number)%SampleDataMax;
  }
  else
  {
    DevicePara.SampleParas.Queue.Front = DevicePara.SampleParas.Queue.Rear;
  }
  
  DataProcess_WriteData(SAMPLE_DATA_TOTAL_ADDR,(uint8_t *)&DevicePara.SampleParas.Queue.Total,6);
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
void DataPocess_WriteDebugResetRecord(void)
{
  uint8_t buff[10] = {0};
  uint16_t num = 0;
  RTC_DateTypeDef date;    
  RTC_TimeTypeDef time;

  //��ȡʱ��   
  RTC_GetDate(RTC_Format_BIN, &date);
  RTC_GetTime(RTC_Format_BIN, &time);
  
  DataProcess_ReadData(DEBUG_RESET_RECORD_ADDR,buff,2);
  num = buff[0]*0x100 + buff[1];
  if(num < 0xFFFF)
  {
    num++;
  }
  buff[0] = num/0x100;
  buff[1] = num%0x100; 
  buff[2] = date.RTC_Year;
  buff[3] = date.RTC_Month;
  buff[4] = date.RTC_Date;
  buff[5] = time.RTC_Hours;
  buff[6] = time.RTC_Minutes; 
  buff[7] = time.RTC_Seconds; 
  
  DataProcess_WriteData(DEBUG_RESET_RECORD_ADDR,buff,8);
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ���Ը�λ��¼
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void DataPocess_ReadDebugResetRecord(uint8_t *buff)
{
  DataProcess_ReadData(DEBUG_RESET_RECORD_ADDR,buff,8);
  DataProcess_ReadData(DEBUG_RESET_WAKE_TIMEOUT_ADDR,&buff[8],12);
}  
/*********************************************************************************
 Function:      //
 Description:   //�����ϴ�ͳ��
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void DataPocess_WriteDebugReportStatistics(uint8_t flag)
{
  uint8_t buff[2] = {0};
  uint16_t num = 0;
  uint16_t addr = 0;
  
  addr = DEBUG_REPORT_STATISTICS_ADDR + flag*2;
  
  DataProcess_ReadData(addr,buff,6);
  num = buff[0]*0x100 + buff[1];
  num++;
  buff[0] = num/0x100;
  buff[1] = num%0x100;
  DataProcess_WriteData(addr,buff,2);
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ�ϴ�ͳ��
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void DataPocess_ReadDebugReportStatistics(uint8_t *buff)
{
  DataProcess_ReadData(DEBUG_REPORT_STATISTICS_ADDR,buff,6);
}
/*********************************************************************************
 Function:      //
 Description:   //�����ϴ���¼
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void DataPocess_WriteDebugReportRecord(uint8_t flag)
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
    DataProcess_WriteData(addr,buff,10); 
  }
  else if(flag == 1)    //�������ɹ�
  {
    addr = DEBUG_REPORT_RECORD_ADDR+6;
    buff[0] = 1;
    DataProcess_WriteData(addr,buff,1); 
  }
  else if(flag == 2)    //������ʧ��
  {
    addr = DEBUG_REPORT_RECORD_ADDR+6;
    buff[0] = 2;
    buff[1] = BC95.ErrorStep;
    buff[2] = BC95.ErrorCode/0x100;
    buff[3] = BC95.ErrorCode%0x100;
    DataProcess_WriteData(addr,buff,4); 
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
void DataPocess_ReadDebugReportRecord(uint8_t *buff)
{
  DataProcess_ReadData(DEBUG_REPORT_RECORD_ADDR,buff,10);
}
/*********************************************************************************
 Function:      //
 Description:   //���滽�ѳ�ʱ��λ��¼
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void DataPocess_WriteWakeTimeoutResetRecord(void)
{
  uint8_t buff[10] = {0};
  RTC_DateTypeDef date;    
  RTC_TimeTypeDef time;

  //��ȡʱ��   
  RTC_GetDate(RTC_Format_BIN, &date);
  RTC_GetTime(RTC_Format_BIN, &time);
  
  buff[0] = date.RTC_Year;
  buff[1] = date.RTC_Month;
  buff[2] = date.RTC_Date;
  buff[3] = time.RTC_Hours;
  buff[4] = time.RTC_Minutes; 
  buff[5] = time.RTC_Seconds; 
  
  DataProcess_WriteData(DEBUG_RESET_WAKE_TIMEOUT_ADDR,buff,6);
}
/*********************************************************************************
 Function:      //
 Description:   //�����ϱ���ʱ��λ��¼
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void DataPocess_WriteReportTimeoutResetRecord(void)
{
  uint8_t buff[10] = {0};
  RTC_DateTypeDef date;    
  RTC_TimeTypeDef time;

  //��ȡʱ��   
  RTC_GetDate(RTC_Format_BIN, &date);
  RTC_GetTime(RTC_Format_BIN, &time);
  
  buff[0] = date.RTC_Year;
  buff[1] = date.RTC_Month;
  buff[2] = date.RTC_Date;
  buff[3] = time.RTC_Hours;
  buff[4] = time.RTC_Minutes; 
  buff[5] = time.RTC_Seconds; 
  
  DataProcess_WriteData(DEBUG_RESET_REPORT_TIMEOUT_ADDR,buff,6);
}
/*************************************END**************************************/
