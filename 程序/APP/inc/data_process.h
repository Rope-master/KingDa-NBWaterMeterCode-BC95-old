/*******************************************************************************
Copyright: 
File name: uplink_protocol.c
Description: ����ͨ��Э����غ���������
Author: ��ѧ��cy
Version: 
Date: 
History:       
*******************************************************************************/
#ifndef  __DATA_PROCESS_H
#define  __DATA_PROCESS_H
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include "include.h"
/*******************************************************************************
�궨����
*******************************************************************************/
/*MRS256��ַ���� 0x0000--0x7FFF*/
#define  INTERNAL_EEPROM_START_ADDR             0x00001000              //EEPROM ��ʼ��ַ
#define  INTERNAL_EEPROM_END_ADDR               0x000017FF              //EEPROM ������ַ

#define  METER_NUMBER_ADDR                      (INTERNAL_EEPROM_START_ADDR+2)      //��ŵ�ַ��7�ֽ�
#define  FORWARD_FLOW_ADDR                      (INTERNAL_EEPROM_START_ADDR+9)      //�ۻ�������ַ��4�ֽ�
#define  SETTLE_DATE_ADD                        (INTERNAL_EEPROM_START_ADDR+13)     //�������ڵ�ַ��1�ֽ�
#define  SAMPLE_FREQUENCY_ADDR                  (INTERNAL_EEPROM_START_ADDR+14)     //����Ƶ�ʵ�ַ��2�ֽ�
#define  SAMPLE_DATA_TOTAL_ADDR                 (INTERNAL_EEPROM_START_ADDR+16)     //��������������2�ֽ�
#define  SAMPLE_DATA_FRONT_ADDR                 (INTERNAL_EEPROM_START_ADDR+18)     //��������ͷ��2�ֽ�
#define  SAMPLE_DATA_REAR_ADDR                  (INTERNAL_EEPROM_START_ADDR+20)     //��������β��2�ֽ�
#define  REPORT_FREQUENCY_ADDR                  (INTERNAL_EEPROM_START_ADDR+22)     //�ϱ�Ƶ�ʵ�ַ��2�ֽ�
#define  FIRST_REPORT_ADDR                      (INTERNAL_EEPROM_START_ADDR+24)     //�׷�ʱ�䣬4�ֽ�
#define  BC95_ERROR_RECORD_ADD                  (INTERNAL_EEPROM_START_ADDR+28)     //BC95���������¼��ַ��1�ֽ�

#define  IMEI_ADDR                              (INTERNAL_EEPROM_START_ADDR+30)     //IMEI��ַ��15�ֽ�   

#define  DEBUG_RESET_RECORD_ADDR                (INTERNAL_EEPROM_START_ADDR+80)     //��λ��¼��ַ��8�ֽ�
#define  DEBUG_REPORT_STATISTICS_ADDR           (INTERNAL_EEPROM_START_ADDR+88)     //�ϴ�ͳ�Ƶ�ַ��6�ֽ�
#define  DEBUG_REPORT_RECORD_ADDR               (INTERNAL_EEPROM_START_ADDR+94)     //�ϴ���¼��ַ��10�ֽ�
#define  DEBUG_RESET_WAKE_TIMEOUT_ADDR          (INTERNAL_EEPROM_START_ADDR+104)     //���ѳ�ʱ��λ��¼��ַ��6�ֽ�
#define  DEBUG_RESET_REPORT_TIMEOUT_ADDR        (INTERNAL_EEPROM_START_ADDR+110)     //�ϱ���ʱ��λ��¼��ַ��6�ֽ�

#define  SAMPLE_DATA_START_ADDR                (INTERNAL_EEPROM_START_ADDR+120)      //��ʷ������ʼ��ַ

/*******************************************************************************
�ܽŶ���
*******************************************************************************/
/*******************************************************************************
�������Ͷ���
*******************************************************************************/
/*******************************************************************************
�ⲿ����������
*******************************************************************************/
/*******************************************************************************
����������
*******************************************************************************/
void DataProcess_ReadData(uint32_t addr, uint8_t *buff, uint16_t length);
void DataProcess_WriteData(uint32_t addr, uint8_t *buff, uint16_t length);
void DataProcess_Clear64Bytes(void);
void DataProcess_WriteFlow(void);
void DataProcess_WriteIMEI(void);
void DataProcess_CheckRereadParas(void);
void DataProcess_ReadDevicePara(void);
void DataProcess_ReadReportParas(void);
void DataProcess_ReadSampleParas(void);
void DataPocess_WriteSampleData(void);
void DataPocess_ReadSampleData(uint8_t *buff,uint8_t number);
void DataPocess_ClearSampleData(uint8_t number);
/*************************************DEBUG************************************/
void DataPocess_WriteDebugResetRecord(void);
void DataPocess_ReadDebugResetRecord(uint8_t *buff);
void DataPocess_WriteDebugReportStatistics(uint8_t flag);
void DataPocess_ReadDebugReportStatistics(uint8_t *buff);
void DataPocess_WriteDebugReportRecord(uint8_t flag);
void DataPocess_ReadDebugReportRecord(uint8_t *buff);
void DataPocess_WriteWakeTimeoutResetRecord(void);
void DataPocess_WriteReportTimeoutResetRecord(void);
/******************************************************************************/
#endif
/*************************************END**************************************/