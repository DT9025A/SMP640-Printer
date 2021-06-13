#include "flash.h"

void SPIFLASH_DUMMY(uint8_t cnt)
{
    uint8_t dummy;
    for (; cnt > 0; cnt--)
    {
        dummy = rand() % 256;
        SPIFLASH_SPI_SendByte(dummy);
    }
}

uint8_t SPIFLASH_ReadWithDummy()
{
    uint8_t dummy = rand() % 256;
    return SPIFLASH_SPI_SendByte(dummy);
}

void SPIFLASH_CMD(uint8_t cmd)
{
    SPIFLASH_CS;
    SPIFLASH_SPI_SendByte(cmd);
    SPIFLASH_NCS;
}

uint8_t SPIFLASH_CMDWithReturn(uint8_t cmd)
{
    uint8_t dat;
    SPIFLASH_CS;
    SPIFLASH_SPI_SendByte(cmd);
    dat = SPIFLASH_ReadWithDummy();
    SPIFLASH_NCS;
    return dat;
}

uint16_t SPIFLASH_ReadSR()
{
    uint16_t dat;

    dat = SPIFLASH_CMDWithReturn(SPIFLASH_READSTAT2) << 8;
    dat |= SPIFLASH_CMDWithReturn(SPIFLASH_READSTAT1);
    return dat;
}

void SPIFLASH_WriteSR(uint16_t dat)
{
    SPIFLASH_CMD(SPIFLASH_WRITEENABLE);
    SPIFLASH_CMD(SPIFLASH_WRITESTATEN);

    SPIFLASH_CS;
    SPIFLASH_SPI_SendByte(SPIFLASH_WRITESTAT);
    SPIFLASH_SPI_SendByte(dat);
    SPIFLASH_SPI_SendByte(dat >> 8);
    SPIFLASH_NCS;
}

void SPIFLASH_WaitBUSY()
{
    while ((SPIFLASH_ReadSR() & 1) == 1)
        ;
}

uint32_t SPIFLASH_ReadID()
{
    uint32_t dat;
    SPIFLASH_CS;
    SPIFLASH_SPI_SendByte(SPIFLASH_ID);
    dat = SPIFLASH_ReadWithDummy() << 16;
    dat |= SPIFLASH_ReadWithDummy() << 8;
    dat |= SPIFLASH_ReadWithDummy();
    SPIFLASH_NCS;
    return dat;
}

uint16_t SPIFLASH_ReadMFDVID()
{
    uint16_t dat;
    SPIFLASH_CS;
    SPIFLASH_SPI_SendByte(SPIFLASH_MANID);
    SPIFLASH_DUMMY(2);
    SPIFLASH_SPI_SendByte(0);
    dat = SPIFLASH_SPI_SendByte(0) << 8;
    dat |= SPIFLASH_SPI_SendByte(0);
    SPIFLASH_NCS;
    return dat;
}

uint8_t SPIFLASH_ReadUID(void)
{
    uint8_t cnt;
    SPIFLASH_CS;
    SPIFLASH_SPI_SendByte(SPIFLASH_UNIQUEID);
    SPIFLASH_SPI_SendByte(0);
    SPIFLASH_SPI_SendByte(1);
    SPIFLASH_SPI_SendByte(0x94);
    SPIFLASH_DUMMY(1);
    cnt = SPIFLASH_ReadWithDummy();
    SPIFLASH_NCS;
    return cnt;
}

void SPIFLASH_PageProg(uint32_t addr, uint8_t *dat)
{
    uint16_t cnt;
    SPIFLASH_CMD(SPIFLASH_WRITEENABLE);
    SPIFLASH_CS;
    SPIFLASH_SPI_SendByte(SPIFLASH_PAGEPROG);
    SPIFLASH_SPI_SendByte(addr >> 16);
    SPIFLASH_SPI_SendByte(addr >> 8);
    SPIFLASH_SPI_SendByte(addr);
    for (cnt = 0; cnt < 256; cnt++)
    {
        SPIFLASH_SPI_SendByte(dat[cnt]);
    }
    SPIFLASH_NCS;
}

void SPIFLASH_ChipErase()
{
    SPIFLASH_CMD(SPIFLASH_WRITEENABLE);
    SPIFLASH_CMD(SPIFLASH_CHIPERASE);
}

void SPIFLASH_EraseX(uint8_t cmd, uint32_t addr)
{
    SPIFLASH_CMD(SPIFLASH_WRITEENABLE);
    SPIFLASH_CS;
    SPIFLASH_SPI_SendByte(cmd);
    SPIFLASH_SPI_SendByte(addr >> 16);
    SPIFLASH_SPI_SendByte(addr >> 8);
    SPIFLASH_SPI_SendByte(addr);
    SPIFLASH_NCS;
}

void SPIFLASH_PageRead(uint32_t addr, uint8_t *dat)
{
    uint16_t cnt;
    SPIFLASH_CS;
    SPIFLASH_SPI_SendByte(SPIFLASH_READDATA);
    SPIFLASH_SPI_SendByte(addr >> 16);
    SPIFLASH_SPI_SendByte(addr >> 8);
    SPIFLASH_SPI_SendByte(addr);
    for (cnt = 0; cnt < 256; cnt++)
        dat[cnt] = SPIFLASH_ReadWithDummy();
    SPIFLASH_NCS;
}
