/*******************************************************************************
Copyright: 
File name: flash.c  
Description: flash����
Author: ��ѧ��cy
Version: 
Date: 
History:       
*******************************************************************************/
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include "flash.h"
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
ErrorStatus Flash_Read(uint32_t address, uint8_t *buff, uint16_t length)
{
  ErrorStatus status;
  
  if(IS_FLASH_DATA_EEPROM_ADDRESS(address))     //EEPROM��ַ��
  {
    status = Flash_EepromMemoryRead(address, buff, length);
  }
  else if(IS_FLASH_PROGRAM_ADDRESS(address))    //FLASH��ַ��
  {
    status = Flash_ProgramMemoryRead(address, buff, length); 
  }
  
  return status;
}
/*******************************************************************************
 Function:      //
 Description:   //Program Memory ������
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
ErrorStatus Flash_Write(uint32_t address, uint8_t *buff, uint16_t length)
{
  ErrorStatus status;
  
  if(IS_FLASH_DATA_EEPROM_ADDRESS(address))     //EEPROM��ַ��
  {
    status = Flash_EepromMemoryWrite(address, buff, length);
  }
  else if(IS_FLASH_PROGRAM_ADDRESS(address))   //FLASH��ַ��
  {
    status = Flash_ProgramMemoryWrite(address, buff, length); 
  }
  
  return status;
}
/*******************************************************************************
 Function:      //
 Description:   //Program Memory ������
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
ErrorStatus Flash_ProgramMemoryRead(uint32_t address, uint8_t *buff, uint16_t length)
{
  uint16_t itemp = 0;

  do
  {FLASH_Unlock(FLASH_MemType_Program); }//����FLASH���
  while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET);
  
  while(itemp < length)
  {
    buff[itemp] = FLASH_ReadByte(address++); 
    itemp++;
  }
  FLASH_Lock(FLASH_MemType_Program);
  
  return SUCCESS;
}
/*******************************************************************************
 Function:      //
 Description:   //Program Memory д����
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
ErrorStatus Flash_ProgramMemoryWrite(uint32_t address, uint8_t *buff, uint16_t length)
{
  unsigned short itemp = 0; 
  unsigned short timeout = 0;
  
 do
 {FLASH_Unlock(FLASH_MemType_Program); }//����FLASH���
 while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET); 
 
 while (itemp < length)
 {
   FLASH_ProgramByte(address, buff[itemp]);
   while(FLASH_GetFlagStatus(FLASH_FLAG_EOP) == RESET)  //�ȴ�������
   {
    timeout++;
    if(timeout == 0xFFFF)
    {
      FLASH_Lock(FLASH_MemType_Program);
      return ERROR;
    }
   }
   itemp ++;
   address++;
 }
 FLASH_Lock(FLASH_MemType_Program);
 
 return SUCCESS;
}
/*******************************************************************************
 Function:      //
 Description:   //Eeprom Memory ������
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
ErrorStatus Flash_EepromMemoryRead(uint32_t address, uint8_t *buff, uint16_t length)
{
  unsigned short itemp = 0; 

  do
  {FLASH_Unlock(FLASH_MemType_Data); }
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);
  
  while(itemp < length)
  {
    buff[itemp] = FLASH_ReadByte(address++); 
    itemp++;
  }
  FLASH_Lock(FLASH_MemType_Data);
  
  return SUCCESS;
}
/*******************************************************************************
 Function:      //
 Description:   //Eeprom Memory д����
 Input:         //
 Output:        //
 Return:	//
 Others:        //
*******************************************************************************/
ErrorStatus Flash_EepromMemoryWrite(uint32_t address, uint8_t *buff, uint16_t length)
{
  unsigned short itemp = 0; 
  unsigned short timeout = 0;

 do
 {FLASH_Unlock(FLASH_MemType_Data); }//����FLASH���
 while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET); 
 
 
 while (itemp < length)
 {
   FLASH_ProgramByte(address, buff[itemp]);
   while(FLASH_GetFlagStatus(FLASH_FLAG_EOP) == RESET)  //�ȴ�������
   {
    timeout++;
    if(timeout == 0xFFFF)
    {
      FLASH_Lock(FLASH_MemType_Data);
      return ERROR;
    }
   }
   itemp ++;
   address++;
 }
 FLASH_Lock(FLASH_MemType_Data);
 
 return SUCCESS;
}
/*************************************END**************************************/
