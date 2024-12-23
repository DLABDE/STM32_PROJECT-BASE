/**************
 *W25Q16
 **************/
 
#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16

#define W25_PAGE_SIZE 0x100
#define W25_SEC_SIZE	(W25_PAGE_SIZE * 16)

#define W25X_WriteEnable		0x06
#define W25X_WriteDisable		0x04
#define W25X_ReadStatusReg		0x05
#define W25X_WriteStatusReg		0x01
#define W25X_ReadData				0x03
#define W25X_FastReadData		0x0B
#define W25X_FastReadDual		0x3B
#define W25X_PageProgram		0x02
#define W25X_BlockErase			0xD8
#define W25X_SectorErase		0x20
#define W25X_ChipErase			0xC7
#define W25X_PowerDown			0xB9
#define W25X_ReleasePowerDown	0xAB
#define W25X_DeviceID			0xAB
#define W25X_ManufactDeviceID	0x90
#define W25X_JedecDeviceID		0x9F

#define W25QXX_SPI_Handle (&hspi1)
#define W25QXX_CS_L()  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET)
#define W25QXX_CS_H()  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET)

struct Flash_Msg
{
	uint16_t type;
	uint32_t size;
	uint8_t uid[8];
};

extern struct Flash_Msg flash_msg;
extern uint8_t W25QXX_USER_BUF[W25_PAGE_SIZE];


int W25QXX_Init(void);
void W25QXX_ReadUniqueID(uint8_t UID[8]);
uint16_t  W25QXX_ReadID(void);  	    	
uint8_t	 W25QXX_ReadSR(void);        	
void W25QXX_Write_SR(uint8_t sr);  	
void W25QXX_Write_Enable(void);  	
void W25QXX_Write_Disable(void);	
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);  
void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void W25QXX_Erase_Chip(void);    	
void W25QXX_Erase_Sector(uint32_t Dst_Addr);
void W25QXX_Wait_Busy(void);           
void W25QXX_PowerDown(void);        	
void W25QXX_WAKEUP(void);			
uint32_t W25QXX_ReadCapacity(void);



#ifdef __cplusplus
}
#endif

#endif /* __SPI_FLASH_H__ */

