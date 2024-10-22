//Thank you, Lonely Wolf (щас спою)
//https://github.com/LonelyWolf/stm32/blob/master/cube-usb-msc/sdcard-sdio.h
//https://github.com/LonelyWolf/stm32/blob/master/cube-usb-msc/sdcard-sdio.c

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

// Mask for ACMD41
#define SD_STD_CAPACITY               ((uint32_t)0x00000000)
#define SD_HIGH_CAPACITY              ((uint32_t)0x40000000)

// Pattern for R6 response
#define SD_CHECK_PATTERN              ((uint32_t)0x000001AA)

// Argument for ACMD41 to select voltage window
#define SD_OCR_VOLTAGE                ((uint32_t)0x80100000)

// Bitmap to clear the SDIO static flags (command and data)
#define SDIO_ICR_STATIC               ((uint32_t)(SDIO_ICR_CCRCFAILC | SDIO_ICR_DCRCFAILC | SDIO_ICR_CTIMEOUTC | \
                                                  SDIO_ICR_DTIMEOUTC | SDIO_ICR_TXUNDERRC | SDIO_ICR_RXOVERRC  | \
                                                  SDIO_ICR_CMDRENDC  | SDIO_ICR_CMDSENTC  | SDIO_ICR_DATAENDC  | \
                                                  SDIO_ICR_DBCKENDC))

// Card state (OCR[12:9] bits CURRENT_STATE)
#define SD_STATE_IDLE                 ((uint8_t)0x00) // Idle
#define SD_STATE_READY                ((uint8_t)0x01) // Ready
#define SD_STATE_IDENT                ((uint8_t)0x02) // Identification
#define SD_STATE_STBY                 ((uint8_t)0x03) // Stand-by
#define SD_STATE_TRAN                 ((uint8_t)0x04) // Transfer
#define SD_STATE_DATA                 ((uint8_t)0x05) // Sending data
#define SD_STATE_RCV                  ((uint8_t)0x06) // Receive data
#define SD_STATE_PRG                  ((uint8_t)0x07) // Programming
#define SD_STATE_DIS                  ((uint8_t)0x08) // Disconnect
#define SD_STATE_ERROR                ((uint8_t)0xFF) // Error or unknown state

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

enum SD_CARD_TYPE{
    SDCT_UNKNOWN            = 0x00,
    SDCT_SDSC_V1            = 0x01,  // Standard capacity SD card v1.0
    SDCT_SDSC_V2            = 0x02,  // Standard capacity SD card v2.0
    SDCT_MMC                = 0x03,  // MMC
    SDCT_SDHC               = 0x04   // High capacity SD card (SDHC or SDXC)
};

//SD functions result
enum SdioResultEnum{
    SDR_Success             = 0x00,
    SDR_Timeout             = 0x01,  // Timeout
    SDR_CRCError            = 0x02,  // Response for command received but CRC check failed
    SDR_ReadError           = 0x03,  // Read block error (response for CMD17)
    SDR_WriteError          = 0x04,  // Write block error (response for CMD24)
    SDR_WriteErrorInternal  = 0x05,  // Write block error due to internal card error
    SDR_Unsupported         = 0x06,  // Unsupported card found
    SDR_BadResponse         = 0x07,
    SDR_SetBlockSizeFailed  = 0x08,  // Set block size command failed (response for CMD16)
    SDR_UnknownCard         = 0x09,
    SDR_NoResponse          = 0x0A,
    SDR_AddrOutOfRange      = 0x0B,  // Address out of range
    SDR_WriteCRCError       = 0x0C,  // Data write rejected due to a CRC error
    SDR_InvalidVoltage      = 0x0D,  // Unsupported voltage range
    SDR_DataTimeout         = 0x0E,  // Data block transfer timeout
    SDR_DataCRCFail         = 0x0F,  // Data block transfer CRC failed
    SDR_RXOverrun           = 0x10,  // Receive FIFO overrun
    SDR_TXUnderrun          = 0x11,  // Transmit FIFO underrun
    SDR_StartBitError       = 0x12,  // Start bit not detected on all data signals
    SDR_AddrMisaligned      = 0x13,  // A misaligned address which did not match the block length was used in the command
    SDR_BlockLenError       = 0x14,  // The transfer block length is not allowed for this card
    SDR_EraseSeqError       = 0x15,  // An error in the sequence of erase commands occurred
    SDR_EraseParam          = 0x16,  // An invalid selection of write-blocks for erase occurred
    SDR_WPViolation         = 0x17,  // Attempt to write to a protected block or to the write protected card
    SDR_LockUnlockFailed    = 0x18,  // Error in lock/unlock command
    SDR_ComCRCError         = 0x19,  // The CRC check of the previous command failed
    SDR_IllegalCommand      = 0x1A,  // Command is not legal for the the current card state
    SDR_CardECCFailed       = 0x1B,  // Card internal ECC was applied but failed to correct the data
    SDR_CCError             = 0x1C,  // Internal card controller error
    SDR_GeneralError        = 0x1D,  // A general or an unknown error occurred during the operation
    SDR_StreamUnderrun      = 0x1E,  // The card could not sustain data transfer in stream read operation
    SDR_StreamOverrun       = 0x1F,  // The card could not sustain data programming in stream mode
    SDR_CSDOverwrite        = 0x20,  // CSD overwrite error
    SDR_WPEraseSkip         = 0x21,  // Only partial address space was erased
    SDR_ECCDisabled         = 0x22,  // The command has been executed without using the internal ECC
    SDR_EraseReset          = 0x23,  // An erase sequence was cleared before executing
    SDR_AKESeqError         = 0x24,  // Error in the sequence of the authentication process
    SDR_UnknownError        = 0xFF   // Unknown error
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
    SDCard_TypeDef sd{};
    SDIO_WideBus sdioWideBus;
    uint32_t busFreq;
    uint8_t clkDiv;
    static void adjustGPIO(GPIO_Info gpioInfo);
    Result sendCmd(uint8_t cmd, uint32_t arg, SDIO_RESP_LEN respType);
    static Result recvResp(SDIO_RESP_TYPE resp_type, uint32_t *pResp);
    static Result getError(uint32_t cardStatus);
    Result stopTransfer();
    Result getCardState(uint8_t *pStatus);
    void parseCardInfo();
    static Result getResult(SdioResultEnum sdiOresultEnum);
public:
    SimpleSDIO(SDIO_WideBus sdioWideBus, uint8_t clkDiv, uint32_t busFreq, uint32_t errorDelay);
    void update1ms() override;
    Result init();
    Result readBlock(uint32_t addr, uint32_t *pBuf, uint32_t length);
    Result writeBlock(uint32_t addr, uint32_t *pBuf, uint32_t length);
};

#endif //SIMPLE_SDIO_H
