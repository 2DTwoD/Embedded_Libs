#include "simple_sdio.h"

void SimpleSDIO::update1ms() {
    OnDelayCommon::update();
}

SimpleSDIO::SimpleSDIO(SDIO_WideBus sdioWideBus, uint8_t clkDiv, uint32_t errorDelay): OnDelayCommon(errorDelay) {
    //GPIO set
    //cmd
    adjustGPIO({GPIOD, 2});
    //d0-d4
    adjustGPIO({GPIOC, 8});
    adjustGPIO({GPIOC, 9});
    adjustGPIO({GPIOC, 10});
    adjustGPIO({GPIOC, 11});
    //clk
    adjustGPIO({GPIOC, 12});
    //RCC GPIO
    enableRCC(CLOCK_SDIO);
    //Configure SDIO clock control register
    setRegValShift(SDIO->CLKCR, SDIO_CLKCR_WIDBUS, sdioWideBus);
    setRegValShift(SDIO->CLKCR, SDIO_CLKCR_CLKDIV, clkDiv);
    setBit(SDIO->CLKCR, SDIO_CLKCR_CLKEN);
    setBit(SDIO->CLKCR, SDIO_CLKCR_PWRSAV);
    //Configure SDIO power control register
    setRegValShift(SDIO->POWER, SDIO_POWER_PWRCTRL, 0b00);
}

void SimpleSDIO::adjustGPIO(GPIO_Info gpioInfo) {
    GPIOconfig gpio(gpioInfo);
    gpio.start().setMODER(GPIO_MODER_AF).setOTYPER(GPIO_OTYPER_PUSH_PULL).setOSPEEDR(GPIO_OSPEEDR_HIGH_SPEED)
    .setPUPDR(GPIO_PUPDR_PULL_UP).setAFR(GPIO_AFR12).fin();
}

Result SimpleSDIO::sendCmd(uint8_t cmd, uint32_t arg, SDIO_RESP_LEN respType) {
    // Clear the command flags
    setBit(SDIO->ICR, SDIO_ICR_CCRCFAILC);
    setBit(SDIO->ICR, SDIO_ICR_CTIMEOUTC);
    setBit(SDIO->ICR, SDIO_ICR_CMDRENDC);
    setBit(SDIO->ICR, SDIO_ICR_CMDSENTC);
    // Command argument value
    SDIO->ARG = arg;
    // Write to SDIO CMD
    setRegValShift(SDIO->CMD, SDIO_CMD_WAITRESP, respType);
    setRegValShift(SDIO->CMD, SDIO_CMD_CMDINDEX, cmd);
    setBit(SDIO->CMD, SDIO_CMD_CPSMEN);
    // Block till get a response
    OnDelayCommon::again();
    if (respType == SDIO_RESP_NONE) {
        // Wait for timeout or CMD sent flag
        while (bitIsZero(SDIO->STA, SDIO_STA_CTIMEOUT) && bitIsZero(SDIO->STA, SDIO_STA_CMDSENT)){
            if(OnDelayCommon::get()) break;
        }
    } else {
        // Wait for CMDSENT or CRCFAIL
        while (bitIsZero(SDIO->STA, SDIO_STA_CTIMEOUT) && bitIsZero(SDIO->STA, SDIO_STA_CMDREND)
        && bitIsZero(SDIO->STA, SDIO_STA_CCRCFAIL)){
            if(OnDelayCommon::get()) break;
        }
    }
    // Check response
    if (bitIsOne(SDIO->STA, SDIO_STA_CTIMEOUT) || OnDelayCommon::get()){
        return ResultBuilder::getError("SDR Timeout");
    }
    if (bitIsOne(SDIO->STA, SDIO_STA_CCRCFAIL)) {
        return ResultBuilder::getError("SDR CRCError"); // CRC fail will be always for R3 response
    }
    return ResultBuilder::getOK();
}

Result SimpleSDIO::getError(uint32_t cardStatus) {
    if (cardStatus & SD_CS_ERROR_BITS) {
        if (cardStatus & SD_CS_OUT_OF_RANGE) return ResultBuilder::getError("SDR AddrOutOfRange");
        if (cardStatus & SD_CS_ADDRESS_ERROR) return ResultBuilder::getError("SDR AddrMisaligned");
        if (cardStatus & SD_CS_BLOCK_LEN_ERROR) return ResultBuilder::getError("SDR BlockLenError");
        if (cardStatus & SD_CS_ERASE_SEQ_ERROR) return ResultBuilder::getError("SDR EraseSeqError");
        if (cardStatus & SD_CS_ERASE_PARAM) return ResultBuilder::getError("SDR EraseParam");
        if (cardStatus & SD_CS_WP_VIOLATION) return ResultBuilder::getError("SDR WPViolation");
        if (cardStatus & SD_CS_LOCK_UNLOCK_FAILED) return ResultBuilder::getError("SDR LockUnlockFailed");
        if (cardStatus & SD_CS_COM_CRC_ERROR) return ResultBuilder::getError("SDR ComCRCError");
        if (cardStatus & SD_CS_ILLEGAL_COMMAND) return ResultBuilder::getError("SDR IllegalCommand");
        if (cardStatus & SD_CS_CARD_ECC_FAILED) return ResultBuilder::getError("SDR CardECCFailed");
        if (cardStatus & SD_CS_CC_ERROR) return ResultBuilder::getError("SDR CCError");
        if (cardStatus & SD_CS_ERROR) return ResultBuilder::getError("SDR GeneralError");
        if (cardStatus & SD_CS_STREAM_R_UNDERRUN) return ResultBuilder::getError("SDR StreamUnderrun");
        if (cardStatus & SD_CS_STREAM_W_OVERRUN) return ResultBuilder::getError("SDR StreamOverrun");
        if (cardStatus & SD_CS_CSD_OVERWRITE) return ResultBuilder::getError("SDR CSDOverwrite");
        if (cardStatus & SD_CS_WP_ERASE_SKIP) return ResultBuilder::getError("SDR WPEraseSkip");
        if (cardStatus & SD_CS_CARD_ECC_DISABLED) return ResultBuilder::getError("SDR ECCDisabled");
        if (cardStatus & SD_CS_ERASE_RESET) return ResultBuilder::getError("SDR EraseReset");
        if (cardStatus & SD_CS_AKE_SEQ_ERROR) return ResultBuilder::getError("SDR AKESeqError");
    }
    return ResultBuilder::getOK("SDR Success");
}

Result SimpleSDIO::recvResp(SDIO_RESP_TYPE resp_type, uint32_t *pResp) {
    // Get first 32-bit value, it similar for all types of response except R2
    *pResp = SDIO->RESP1;
    switch (resp_type) {
        case SDIO_RESP_SD_R1:
        case SDIO_RESP_SD_R1b:
            // RESP1 contains card status information
            // Check for error bits in card status
            return getError(*pResp);
        case SDIO_RESP_SD_R2:
            *pResp++ = __builtin_bswap32(SDIO->RESP1);
            *pResp++ = __builtin_bswap32(SDIO->RESP2);
            *pResp++ = __builtin_bswap32(SDIO->RESP3);
            *pResp   = __builtin_bswap32(SDIO->RESP4);
            break;
        case SDIO_RESP_SD_R3:
            // RESP1 contains the OCR register value
            // Check for correct OCR header
            if (SDIO->RESPCMD != 0x3f) {
                return ResultBuilder::getError("Bad response");
            }
            break;
        case SDIO_RESP_SD_R6:
            // RESP1 contains the RCA response value
            // Only CMD3 generates R6 response, so RESPCMD must be 0x03
            if (SDIO->RESPCMD != 0x03) {
                return ResultBuilder::getError("Bad response");
            }
            break;
        case SDIO_RESP_SD_R7:
            // RESP1 contains 'Voltage accepted' and echo-back of check pattern
            // Only CMD8 generates R7 response, so RESPCMD must be 0x08
            if (SDIO->RESPCMD != 0x08) {
                return ResultBuilder::getError("Bad response");
            }
            break;
        default:
            // Unknown response
            return ResultBuilder::getError("Unknown type");
    }
    return ResultBuilder::getOK();
}
