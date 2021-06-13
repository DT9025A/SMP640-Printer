//  ”√”⁄XT25F08B-S–æ∆¨
#ifndef __FLASH_H__
#define __FLASH_H__

#include "spi.h"
#include <stdlib.h>

/**
 * Write Enable
 */
#define SPIFLASH_WRITEENABLE   0x06
/**
 * Write Disable
 */
#define SPIFLASH_WRITEDISABLE  0x04

/**
 * Read Status Register (S7-S0)
 */
#define SPIFLASH_READSTAT1     0x05
/**
 * Read Status Register-1 (S15-S8)
 */
#define SPIFLASH_READSTAT2     0x35

/**
 * Write Enable for Volatile Status Register
 */
#define SPIFLASH_WRITESTATEN   0x50
/**
 * Write Status Register
 */
#define SPIFLASH_WRITESTAT     0x01

/**
 * Read Data
 */
#define SPIFLASH_READDATA      0x03
/**
 * Fast Read
 */
#define SPIFLASH_FASTREAD      0x0B
/**
 * Continuous Read Reset
 */
#define SPIFLASH_READRESET     0xFF

/**
 * Page Program
 */
#define SPIFLASH_PAGEPROG      0x02
/**
 * Sector Erase
 */
#define SPIFLASH_SECTORERASE   0x20
/**
 * Block Erase(32KB)
 */
#define SPIFLASH_BLOCK32ERASE  0x52
/**
 * Block Erase(64KB)
 */
#define SPIFLASH_BLOCK64ERASE  0xD8
/**
 * Chip Erase
 */
#define SPIFLASH_CHIPERASE     0x60
/**
 * Deep Power-Down
 */
#define SPIFLASH_POWERDOWN     0xB9
/**
 * Release From Deep Power-Down
 */
#define SPIFLASH_RELEASEFROMPD 0xAB

/**
 * Manufacturer/Device ID
 */
#define	SPIFLASH_MANID         0x90
/**
 * Read Serial Flash Discoverable Parameters
 */
#define SPIFLASH_READSFDP      0x5A
/**
 * Read Unique ID
 */
#define SPIFLASH_UNIQUEID      0x5A
/**
 * Read Identification
 */
#define SPIFLASH_ID            0x9F

/**
 * Erase Security Register(8)
 */
#define SPIFLASH_ERASESECURITY 0x44
/**
 * Program Security Register(8)
 */
#define SPIFLASH_PROGSECURITY  0x42
/**
 * Read Security Register(8)
 */
#define SPIFLASH_READSECURITY  0x48

/**
 * Enable Reset
 */
#define SPIFLASH_ENABLERESET   0x66
/**
 * Reset
 */
#define SPIFLASH_RESET         0x99

void SPIFLASH_DUMMY (uint8_t cnt);
uint8_t SPIFLASH_ReadWithDummy(void);
void SPIFLASH_CMD (uint8_t cmd);
uint8_t SPIFLASH_CMDWithReturn (uint8_t cmd);
uint16_t SPIFLASH_ReadSR(void);
void SPIFLASH_WriteSR (uint16_t dat);
void SPIFLASH_WaitBUSY (void);
uint32_t SPIFLASH_ReadJEDECID(void);
uint16_t SPIFLASH_ReadMFDVID(void);
uint8_t SPIFLASH_ReadUID (void);
void SPIFLASH_PageProg (uint32_t addr, uint8_t *dat);
void SPIFLASH_ChipErase (void);
void SPIFLASH_EraseX (uint8_t cmd, uint32_t addr);
void SPIFLASH_PageRead (uint32_t addr, uint8_t *dat);

#define SPIFLASH_SectorErase(addr) SPIFLASH_EraseX(SPIFLASH_SECTORERASE,addr)
#define SPIFLASH_Block32Erase(addr) SPIFLASH_EraseX(SPIFLASH_BLOCK32ERASE,addr)
#define SPIFLASH_Block64Erase(addr) SPIFLASH_EraseX(SPIFLASH_BLOCK64ERASE,addr)
#define SPIFLASH_SPI_SendByte SPI_SendByte

#define SPIFLASH_CS W25Q_CS = 0
#define SPIFLASH_NCS W25Q_CS = 1


#endif
