#include "spi_flash.h"
#include "spi.h"

struct Flash_Msg flash_msg;
uint8_t W25QXX_USER_BUF[W25_PAGE_SIZE];

static void delay_us(uint32_t us)
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 16000000 * us);
    while (delay--)
    {
        ;
    }
}

//SPI读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
static uint8_t W25QXX_SPI_ReadWriteByte(uint8_t TxData)
{
	uint8_t RxData = 0X00;
	if(HAL_SPI_TransmitReceive(W25QXX_SPI_Handle, &TxData, &RxData, 1, 100) != HAL_OK)
	{
		RxData = 0XFF;
	}
	return RxData;
}

//4Kbytes为一个Sector
//16个扇区为1个Block
//W25Q128
//容量为16M字节,共有128个Block,4096个Sector

//初始化SPI FLASH的IO口
int W25QXX_Init(void)
{
	W25QXX_CS_L();
	W25QXX_WAKEUP();

	W25QXX_CS_H(); 
	flash_msg.type = W25QXX_ReadID();          // 读取FLASH ID.
	flash_msg.size = W25QXX_ReadCapacity();    // 读取容量
	W25QXX_ReadUniqueID(flash_msg.uid);        // 读取唯一ID
	if((flash_msg.type & 0XEF00) != 0XEF00)
	{
		return -1;
	}
	return 0;
}

//读取W25QXX的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
uint8_t W25QXX_ReadSR(void)
{
    uint8_t byte = 0;
    W25QXX_CS_L(); 
    W25QXX_SPI_ReadWriteByte(W25X_ReadStatusReg); //发送读取状态寄存器命令
    byte = W25QXX_SPI_ReadWriteByte(0Xff);          //读取一个字节
    W25QXX_CS_H();  
    return byte;
}
//写W25QXX状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void W25QXX_Write_SR(uint8_t sr)
{
    W25QXX_CS_L(); 
    W25QXX_SPI_ReadWriteByte(W25X_WriteStatusReg);                 //发送写取状态寄存器命令
    W25QXX_SPI_ReadWriteByte(sr);               	//写入一个字节
    W25QXX_CS_H();  
}

void W25QXX_Write_Enable(void)
{
    W25QXX_CS_L(); 
    W25QXX_SPI_ReadWriteByte(W25X_WriteEnable); 	//发送写使能
    W25QXX_CS_H();  
}

void W25QXX_Write_Disable(void)
{
    W25QXX_CS_L(); 
    W25QXX_SPI_ReadWriteByte(W25X_WriteDisable);  //发送写禁止指令
    W25QXX_CS_H();  
}

uint16_t W25QXX_ReadID(void)
{
    uint16_t Temp = 0;
    W25QXX_CS_L();
    W25QXX_SPI_ReadWriteByte(0x90);                            //发送读取ID命令
    W25QXX_SPI_ReadWriteByte(0x00);
    W25QXX_SPI_ReadWriteByte(0x00);
    W25QXX_SPI_ReadWriteByte(0x00);
    Temp |= W25QXX_SPI_ReadWriteByte(0xFF) << 8;
    Temp |= W25QXX_SPI_ReadWriteByte(0xFF);
    W25QXX_CS_H();
    return Temp;
}

uint32_t W25QXX_ReadCapacity(void)
{
	int i = 0;
	uint8_t arr[4] = {0,0,0,0};
    W25QXX_CS_L();
    W25QXX_SPI_ReadWriteByte(0x5A);
    W25QXX_SPI_ReadWriteByte(0x00);
    W25QXX_SPI_ReadWriteByte(0x00);
    W25QXX_SPI_ReadWriteByte(0x84);
	W25QXX_SPI_ReadWriteByte(0x00);
	for(i = 0; i < sizeof(arr); i++)
	{
		arr[i] = W25QXX_SPI_ReadWriteByte(0xFF);
	}
    W25QXX_CS_H();
    return ((((*(uint32_t *)arr)) + 1) >> 3);
}

void W25QXX_ReadUniqueID(uint8_t UID[8])
{
	int i = 0;
	W25QXX_CS_L();
    W25QXX_SPI_ReadWriteByte(0x4B);
    W25QXX_SPI_ReadWriteByte(0x00);
    W25QXX_SPI_ReadWriteByte(0x00);
    W25QXX_SPI_ReadWriteByte(0x00);
	W25QXX_SPI_ReadWriteByte(0x00);
    for(i = 0; i < 8; i++)
	{
		UID[i] = W25QXX_SPI_ReadWriteByte(0xFF);
	}
	W25QXX_CS_H();
}

//读取SPI FLASH
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void W25QXX_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    W25QXX_CS_L(); 
    W25QXX_SPI_ReadWriteByte(W25X_FastReadData);         	//发送读取命令
		W25QXX_SPI_ReadWriteByte((uint8_t)((ReadAddr) >> 16));  	//发送24bit地址
    W25QXX_SPI_ReadWriteByte((uint8_t)((ReadAddr) >> 8));
    W25QXX_SPI_ReadWriteByte((uint8_t)ReadAddr);
	
    W25QXX_SPI_ReadWriteByte(0);
		HAL_SPI_Receive(W25QXX_SPI_Handle,pBuffer,NumByteToRead,2000);
    W25QXX_CS_H();
}
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
void W25QXX_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t i;
    W25QXX_Write_Enable();                  	
    W25QXX_CS_L(); 
    W25QXX_SPI_ReadWriteByte(W25X_PageProgram);      	//发送写页命令
    W25QXX_SPI_ReadWriteByte((uint8_t)((WriteAddr) >> 16));  	//发送24bit地址
    W25QXX_SPI_ReadWriteByte((uint8_t)((WriteAddr) >> 8));
    W25QXX_SPI_ReadWriteByte((uint8_t)WriteAddr);
    for (i = 0; i < NumByteToWrite; i++)
        W25QXX_SPI_ReadWriteByte(pBuffer[i]); //循环写数
    W25QXX_CS_H();  
    W25QXX_Wait_Busy();					  
}
//无检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void W25QXX_Write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t pageremain;
    pageremain = 256 - WriteAddr % 256; //单页剩余的字节数
    if (NumByteToWrite <= pageremain)
        pageremain = NumByteToWrite; //不大于256个字节
    while (1)
    {
        W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
        if (NumByteToWrite == pageremain)
            break; //写入结束了
        else //NumByteToWrite>pageremain
        {
            pBuffer += pageremain;
            WriteAddr += pageremain;

            NumByteToWrite -= pageremain;			  //减去已经写入了的字节数
            if (NumByteToWrite > 256)
                pageremain = 256; //一次可以写入256个字节
            else
                pageremain = NumByteToWrite; 	  //不够256个字节了
        }
    };
}
//写SPI FLASH
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
uint8_t W25QXX_BUFFER[4096];
void W25QXX_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t *W25QXX_BUF;
    W25QXX_BUF = W25QXX_BUFFER;
    secpos = WriteAddr / 4096; 	  //扇区地址
    secoff = WriteAddr % 4096; 	  //在扇区内的偏移
    secremain = 4096 - secoff; 	  //扇区剩余空间大小
    if (NumByteToWrite <= secremain)
        secremain = NumByteToWrite; 	  //不大于4096个字节
    while (1)
    {
        W25QXX_Read(W25QXX_BUF, secpos * 4096, 4096); 	  //读出整个扇区的内容
        for (i = 0; i < secremain; i++) //校验数据
        {
            if (W25QXX_BUF[secoff + i] != 0XFF)
                break; //需要擦除
        }
        if (i < secremain) //需要擦除
        {
            W25QXX_Erase_Sector(secpos);		//擦除这个扇区
            for (i = 0; i < secremain; i++)	   		//复制
            {
                W25QXX_BUF[i + secoff] = pBuffer[i];
            }
            W25QXX_Write_NoCheck(W25QXX_BUF, secpos * 4096, 4096);	   	//写入整个扇区

        }
        else
            W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain); //写已经擦除了的,直接写入扇区剩余区间.
        if (NumByteToWrite == secremain)
            break; //写入结束了
        else //写入未结束
        {
            secpos++; //扇区地址增1
            secoff = 0; //偏移位置为0

            pBuffer += secremain;  				//指针偏移
            WriteAddr += secremain;				//写地址偏移
            NumByteToWrite -= secremain;			//字节数递减
            if (NumByteToWrite > 4096)
                secremain = 4096;			//下一个扇区还是写不完
            else
                secremain = NumByteToWrite;		//下一个扇区可以写完了
        }
    };
}

//擦除整个芯片
//等待时间超长...
void W25QXX_Erase_Chip(void)
{
    W25QXX_Write_Enable();                 	 	
    W25QXX_Wait_Busy();
    W25QXX_CS_L(); 
    W25QXX_SPI_ReadWriteByte(W25X_ChipErase);    
    W25QXX_CS_H();  
    W25QXX_Wait_Busy();   				   		
}
//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个山区的最少时间:150ms
void W25QXX_Erase_Sector(uint32_t Dst_Addr)
{
	Dst_Addr *= 4096;
	W25QXX_Write_Enable();                  	
	W25QXX_Wait_Busy();
	W25QXX_CS_L(); 
	W25QXX_SPI_ReadWriteByte(W25X_SectorErase);      	//发送扇区擦除指令
	W25QXX_SPI_ReadWriteByte((uint8_t)((Dst_Addr) >> 16));  	//发送24bit地址
  W25QXX_SPI_ReadWriteByte((uint8_t)((Dst_Addr) >> 8));
  W25QXX_SPI_ReadWriteByte((uint8_t)Dst_Addr);
	W25QXX_CS_H();  
	W25QXX_Wait_Busy();   				   		
}

void W25QXX_Wait_Busy(void)
{
	if((W25QXX_ReadSR() & 0x01) == 0x01)	// 等待BUSY位清空
		osDelay(1);
}

void W25QXX_PowerDown(void)
{
    W25QXX_CS_L(); 
    W25QXX_SPI_ReadWriteByte(W25X_PowerDown);        
    W25QXX_CS_H();  
    delay_us(3);                               
}

void W25QXX_WAKEUP(void)
{
    W25QXX_CS_L(); 
    W25QXX_SPI_ReadWriteByte(W25X_ReleasePowerDown); 
    W25QXX_CS_H();  
    delay_us(3);                            	
}

