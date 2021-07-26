struct SPIFLASH
{
	unsigned char PAGE_BUFFER[256];
	void (*CleanPageBuffer)(unsigned char);
	void (*EraseChip)(void);
	void (*Init)(void);
	unsigned char (*WritePage)(unsigned int);
	unsigned char (*ReadByte)(unsigned long ulAddress);
	void (*GetID)(void);
	unsigned char (*WriteByte)(unsigned long ulAddress, unsigned char ucData);
};

extern struct SPIFLASH SpiFlash;

void SpiFlash_Struct_Init(void);
unsigned char SPI3_Write_SpiFlash_Clean_Page_BuferByte(unsigned char writebyte);
void SpiFlash_Clean_Page_Buffer(unsigned char ucData);
void SpiFlash_Get_ID(void);
void SpiFlash_Erase_Chip(void);
void SpiFlash_Check_Busy(void);
unsigned char SPIFALSH_Read_Byte(unsigned int uiAddress);
unsigned char SpiFlash_Write_Page(unsigned int uiAddress);


