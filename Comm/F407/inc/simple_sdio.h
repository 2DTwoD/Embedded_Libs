#ifndef SIMPLE_SDIO_H
#define SIMPLE_SDIO_H

#include <cstdint>
#include "result.h"
#include "gpio_common.h"
#include "stm32f407xx.h"
#include "init.h"
#include "on_delay.h"

// Mask for errors in card status value
#define SD_CS_ERROR_BITS              ((uint32_t)0xFDFFE008) // All possible error bits
#define SD_CS_OUT_OF_RANGE            ((uint32_t)0x80000000) // The command's argument was out of allowed range
#define SD_CS_ADDRESS_ERROR           ((uint32_t)0x40000000) // A misaligned address used in the command
#define SD_CS_BLOCK_LEN_ERROR         ((uint32_t)0x20000000) // The transfer block length is not allowed for this card
#define SD_CS_ERASE_SEQ_ERROR         ((uint32_t)0x10000000) // An error in the sequence of erase commands occurred
#define SD_CS_ERASE_PARAM             ((uint32_t)0x08000000) // An invalid selection of write-blocks for erase occurred
#define SD_CS_WP_VIOLATION            ((uint32_t)0x04000000) // Attempt to write to a protected block or to the write protected card
#define SD_CS_LOCK_UNLOCK_FAILED      ((uint32_t)0x01000000) // Sequence or password error in lock/unlock card command
#define SD_CS_COM_CRC_ERROR           ((uint32_t)0x00800000) // The CRC check of the previous command failed
#define SD_CS_ILLEGAL_COMMAND         ((uint32_t)0x00400000) // Command not legal for the card state
#define SD_CS_CARD_ECC_FAILED         ((uint32_t)0x00200000) // Card internal ECC was applied but failed to correct the data
#define SD_CS_CC_ERROR                ((uint32_t)0x00100000) // Internal card controller error
#define SD_CS_ERROR                   ((uint32_t)0x00080000) // A general or an unknown error occurred during the operation
#define SD_CS_STREAM_R_UNDERRUN       ((uint32_t)0x00040000) // The card could not sustain data transfer in stream read operation
#define SD_CS_STREAM_W_OVERRUN        ((uint32_t)0x00020000) // The card could not sustain data programming in stream mode
#define SD_CS_CSD_OVERWRITE           ((uint32_t)0x00010000) // CSD overwrite error
#define SD_CS_WP_ERASE_SKIP           ((uint32_t)0x00008000) // Only partial address space was erased
#define SD_CS_CARD_ECC_DISABLED       ((uint32_t)0x00004000) // The command has been executed without using the internal ECC
#define SD_CS_ERASE_RESET             ((uint32_t)0x00002000) // An erase sequence was cleared before executing
#define SD_CS_AKE_SEQ_ERROR           ((uint32_t)0x00000008) // Error in the sequence of the authentication process

enum SDIO_WideBus{
    SDIO_1BIT = 0b00,
    SDIO_4BIT = 0b01
};

enum SDIO_RESP_LEN{
    SDIO_RESP_NONE = 0b00,
    SDIO_RESP_SHORT = 0b01,
    SDIO_RESP_LONG = 0b11
};

enum SDIO_RESP_TYPE{
    SDIO_RESP_SD_R1,
    SDIO_RESP_SD_R1b,
    SDIO_RESP_SD_R2,
    SDIO_RESP_SD_R3,
    SDIO_RESP_SD_R6,
    SDIO_RESP_SD_R7,
};

// SD card description
struct SDCard_TypeDef{
    uint8_t     Type;            // Card type (detected by SD_Init())
    uint32_t    Capacity;        // Card capacity (MBytes for SDHC/SDXC, bytes otherwise)
    uint32_t    BlockCount;      // SD card blocks count
    uint32_t    BlockSize;       // SD card block size (bytes), determined in SD_ReadCSD()
    uint32_t    MaxBusClkFreq;   // Maximum card bus frequency (MHz)
    uint8_t     CSDVer;          // SD card CSD register version
    uint16_t    RCA;             // SD card RCA address (only for SDIO)
    uint8_t     MID;             // SD card manufacturer ID
    uint16_t    OID;             // SD card OEM/Application ID
    uint8_t     PNM[5];          // SD card product name (5-character ASCII string)
    uint8_t     PRV;             // SD card product revision (two BCD digits: '6.2' will be 01100010b)
    uint32_t    PSN;             // SD card serial number
    uint16_t    MDT;             // SD card manufacturing date
    uint8_t     CSD[16];         // SD card CSD register (card structure data)
    uint8_t     CID[16];         // SD card CID register (card identification number)
    uint8_t     SCR[8];          // SD card SCR register (SD card configuration)
};

class SimpleSDIO: private OnDelayCommon, public IUpdated1ms{
private:
    SDCard_TypeDef sd;
    void adjustGPIO(GPIO_Info gpioInfo);
    Result sendCmd(uint8_t cmd, uint32_t arg, SDIO_RESP_LEN respType);
    Result recvResp(SDIO_RESP_TYPE resp_type, uint32_t *pResp);
    Result getError(uint32_t cardStatus);
    Result getSCR(uint32_t *pSCR);
public:
    SimpleSDIO(SDIO_WideBus sdioWideBus, uint8_t clkDiv, uint32_t errorDelay);
    void update1ms() override;

};

#endif //SIMPLE_SDIO_H
