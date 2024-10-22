#include "simple_sdio.h"

void SimpleSDIO::update1ms() {
    OnDelayCommon::update();
}

SimpleSDIO::SimpleSDIO(SDIO_WideBus sdioWideBus, uint8_t clkDiv, uint32_t busFreq, uint32_t errorDelay):
                        OnDelayCommon(errorDelay), sdioWideBus(sdioWideBus), busFreq(busFreq), clkDiv(clkDiv){
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
    //Configure SDIO power control register
    setRegValShift(SDIO->POWER, SDIO_POWER_PWRCTRL, 0b00);
}

void SimpleSDIO::adjustGPIO(GPIO_Info gpioInfo) {
    GPIOconfig gpio(gpioInfo);
    gpio.start().setMODER(GPIO_MODER_AF).setOTYPER(GPIO_OTYPER_PUSH_PULL).setOSPEEDR(GPIO_OSPEEDR_VHIGH_SPEED)
    .setPUPDR(GPIO_PUPDR_PULL_UP).setAFR(GPIO_AFR12).fin();
}

Result SimpleSDIO::sendCmd(uint8_t cmd, uint32_t arg, SDIO_RESP_LEN respType) {
    // Clear the command flags
    setBit(SDIO->ICR, SDIO_ICR_CCRCFAILC | SDIO_ICR_CTIMEOUTC | SDIO_ICR_CMDRENDC | SDIO_ICR_CMDSENTC);
    // Command argument value
    SDIO->ARG = arg;
    // Write to SDIO CMD
    uint32_t tmp = cmd | (respType << SDIO_CMD_WAITRESP_Pos) | SDIO_CMD_CPSMEN;
//    setRegValShift(SDIO->CMD, SDIO_CMD_WAITRESP, respType);
//    setBit(SDIO->CMD,SDIO_CMD_CPSMEN);
//    setRegValShift(SDIO->CMD, SDIO_CMD_CMDINDEX, cmd);
    SDIO->CMD = tmp;
    // Block till get a response
    OnDelayCommon::again();
    if (respType == SDIO_RESP_NONE) {
        // Wait for timeout or CMD sent flag
        while (bitIsZero(SDIO->STA, SDIO_STA_CTIMEOUT | SDIO_STA_CMDSENT)){
            if(OnDelayCommon::get()) break;
        }
    } else {
        // Wait for CMDSENT or CRCFAIL
        while (bitIsZero(SDIO->STA, SDIO_STA_CTIMEOUT | SDIO_STA_CMDREND | SDIO_STA_CCRCFAIL)){
            if(OnDelayCommon::get()) break;
        }
    }
    // Check response
    if (bitIsOne(SDIO->STA, SDIO_STA_CTIMEOUT) || OnDelayCommon::get()){
        return getResult(SDR_Timeout);
    }
    if (bitIsOne(SDIO->STA, SDIO_STA_CCRCFAIL)) {
        return getResult(SDR_CRCError); // CRC fail will be always for R3 response
    }
    return getResult(SDR_Success);
}

Result SimpleSDIO::getError(uint32_t cardStatus) {
    if (cardStatus & SD_CS_ERROR_BITS) {
        if (cardStatus & SD_CS_OUT_OF_RANGE) return getResult(SDR_AddrOutOfRange);
        if (cardStatus & SD_CS_ADDRESS_ERROR) return getResult(SDR_AddrMisaligned);
        if (cardStatus & SD_CS_BLOCK_LEN_ERROR) return getResult(SDR_BlockLenError);
        if (cardStatus & SD_CS_ERASE_SEQ_ERROR) return getResult(SDR_EraseSeqError);
        if (cardStatus & SD_CS_ERASE_PARAM) return getResult(SDR_EraseParam);
        if (cardStatus & SD_CS_WP_VIOLATION) return getResult(SDR_WPViolation);
        if (cardStatus & SD_CS_LOCK_UNLOCK_FAILED) return getResult(SDR_LockUnlockFailed);
        if (cardStatus & SD_CS_COM_CRC_ERROR) return getResult(SDR_ComCRCError);
        if (cardStatus & SD_CS_ILLEGAL_COMMAND) return getResult(SDR_IllegalCommand);
        if (cardStatus & SD_CS_CARD_ECC_FAILED) return getResult(SDR_CardECCFailed);
        if (cardStatus & SD_CS_CC_ERROR) return getResult(SDR_CCError);
        if (cardStatus & SD_CS_ERROR) return getResult(SDR_GeneralError);
        if (cardStatus & SD_CS_STREAM_R_UNDERRUN) return getResult(SDR_StreamUnderrun);
        if (cardStatus & SD_CS_STREAM_W_OVERRUN) return getResult(SDR_StreamOverrun);
        if (cardStatus & SD_CS_CSD_OVERWRITE) return getResult(SDR_CSDOverwrite);
        if (cardStatus & SD_CS_WP_ERASE_SKIP) return getResult(SDR_WPEraseSkip);
        if (cardStatus & SD_CS_CARD_ECC_DISABLED) return getResult(SDR_ECCDisabled);
        if (cardStatus & SD_CS_ERASE_RESET) return getResult(SDR_EraseReset);
        if (cardStatus & SD_CS_AKE_SEQ_ERROR) return getResult(SDR_AKESeqError);
    }
    return getResult(SDR_Success);
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
                return getResult(SDR_BadResponse);
            }
            break;
        case SDIO_RESP_SD_R6:
            // RESP1 contains the RCA response value
            // Only CMD3 generates R6 response, so RESPCMD must be 0x03
            if (SDIO->RESPCMD != 0x03) {
                return getResult(SDR_BadResponse);
            }
            break;
        case SDIO_RESP_SD_R7:
            // RESP1 contains 'Voltage accepted' and echo-back of check pattern
            // Only CMD8 generates R7 response, so RESPCMD must be 0x08
            if (SDIO->RESPCMD != 0x08) {
                return getResult(SDR_BadResponse);
            }
            break;
        default:
            // Unknown response
            return getResult(SDR_UnknownError);
    }
    return getResult(SDR_Success);
}

void SimpleSDIO::parseCardInfo() {
    uint32_t dev_size, dev_size_mul, rd_block_len;

    // Parse the CSD register
    sd.CSDVer = sd.CSD[0] >> 6; // CSD version
    if (sd.Type != SDCT_MMC) {
        // SD
        sd.MaxBusClkFreq = sd.CSD[3];
        rd_block_len = sd.CSD[5] & 0x0f; // Max. read data block length
        if (sd.CSDVer == 0) {
            // CSD v1.00 (SDSCv1, SDSCv2)
            dev_size  = (uint32_t)(sd.CSD[6] & 0x03) << 10; // Device size
            dev_size |= (uint32_t)sd.CSD[7] << 2;
            dev_size |= (sd.CSD[8] & 0xc0) >> 6;
            dev_size_mul  = (sd.CSD[9] & 0x03) << 1; // Device size multiplier
            dev_size_mul |= (sd.CSD[10] & 0x80) >> 7;
            sd.BlockCount  = (dev_size + 1);
            sd.BlockCount *= (1 << (dev_size_mul + 2));
            sd.BlockSize =  1 << (rd_block_len);
        } else {
            // CSD v2.00 (SDHC, SDXC)
            dev_size  = (sd.CSD[7] & 0x3f) << 16;
            dev_size |=  sd.CSD[8] << 8;
            dev_size |=  sd.CSD[9];
            sd.BlockSize = 512;
            sd.BlockCount = dev_size + 1;
        }
        sd.Capacity = sd.BlockCount * sd.BlockSize;
    } else {
        // MMC
        sd.MaxBusClkFreq = sd.CSD[3];
        dev_size  = (uint32_t)(sd.CSD[6] & 0x03) << 8; // C_SIZE
        dev_size += (uint32_t)sd.CSD[7];
        dev_size <<= 2;
        dev_size += sd.CSD[8] >> 6;
        sd.BlockSize = 1 << (sd.CSD[5] & 0x0f); // MMC read block length
        dev_size_mul = ((sd.CSD[9] & 0x03) << 1) + ((sd.CSD[10] & 0x80) >> 7);
        sd.BlockCount = (dev_size + 1) * (1 << (dev_size_mul + 2));
        sd.Capacity = sd.BlockCount * sd.BlockSize;
    }

    // Parse the CID register
    if (sd.Type != SDCT_MMC) {
        // SD card
        sd.MID = sd.CID[0];
        sd.OID = (sd.CID[1] << 8) | sd.CID[2];
        sd.PNM[0] = sd.CID[3];
        sd.PNM[1] = sd.CID[4];
        sd.PNM[2] = sd.CID[5];
        sd.PNM[3] = sd.CID[6];
        sd.PNM[4] = sd.CID[7];
        sd.PRV = sd.CID[8];
        sd.PSN = (sd.CID[9] << 24) | (sd.CID[10] << 16) | (sd.CID[11] << 8) | sd.CID[12];
        sd.MDT = ((sd.CID[13] << 8) | sd.CID[14]) & 0x0fff;
    } else {
        // MMC
        sd.MID = 0x00;
        sd.OID = 0x0000;
        sd.PNM[0] = '*';
        sd.PNM[1] = 'M';
        sd.PNM[2] = 'M';
        sd.PNM[3] = 'C';
        sd.PNM[4] = '*';
        sd.PRV = 0;
        sd.PSN = 0x00000000;
        sd.MDT = 0x0000;
    }
}

Result SimpleSDIO::getCardState(uint8_t *pStatus) {
    uint32_t response;
    // Send SEND_STATUS command
    sendCmd(13,sd.RCA << 16,SDIO_RESP_SHORT); // CMD13
    recvResp(SDIO_RESP_SD_R1,&response);
    // Find out card status
    *pStatus = (response & 0x1e00) >> 9;
    // Check for errors
    return getError(response);
}

Result SimpleSDIO::init() {
    uint32_t response[4];
    uint32_t sdType = SD_STD_CAPACITY; // SD card capacity
    Result cmdRes = getResult(SDR_Success);

    // Populate SDCard structure with default values
    sd.Capacity = 0;
    sd.MaxBusClkFreq = 0;
    sd.BlockSize = 0;
    sd.CSDVer = 0;
    sd.Type = 0;
    sd.RCA = 0;

    //Configure SDIO clock control registers
    setRegValShift(SDIO->CLKCR, SDIO_CLKCR_WIDBUS, sdioWideBus);
    setRegValShift(SDIO->CLKCR, SDIO_CLKCR_CLKDIV, clkDiv);
    setBit(SDIO->CLKCR, SDIO_CLKCR_CLKEN | SDIO_CLKCR_PWRSAV);

    setRegValShift(SDIO->POWER, SDIO_POWER_PWRCTRL, 0b11); // Enable SDIO clock

    cmdRes = sendCmd(0,0x00,SDIO_RESP_NONE);
    // CMD8: SEND_IF_COND. Send this command to verify SD card interface operating condition
    // Argument: - [31:12]: Reserved (shall be set to '0')
    //           - [11:8]: Supply Voltage (VHS) 0x1 (Range: 2.7-3.6 V)
    //           - [7:0]: Check Pattern (recommended 0xAA)
    cmdRes = sendCmd(8, SD_CHECK_PATTERN, SDIO_RESP_SHORT); // CMD8
    if (cmdRes == rOK) {
        // SD v2.0 or later

        // Get and check R7 response
        if (recvResp(SDIO_RESP_SD_R7,response) != rOK) return getResult(SDR_BadResponse);
        // Check echo-back of check pattern
        if ((response[0] & 0x01ff) != (SD_CHECK_PATTERN & 0x01ff)) return getResult(SDR_Unsupported);
        sdType = SD_HIGH_CAPACITY; // SD v2.0 or later

        // Issue ACMD41 command
        OnDelayCommon::again();
        while (true) {
            // Send leading command for ACMD<n> command
            sendCmd(55,0,SDIO_RESP_SHORT); // CMD55 with RCA 0
            if (recvResp(SDIO_RESP_SD_R1,response) != rOK) return getResult(SDR_BadResponse);
            // ACMD41 - initiate initialization process.
            // Set 3.0-3.3V voltage window (bit 20)
            // Set HCS bit (30) (Host Capacity Support) to inform card what host support high capacity
            // Set XPC bit (28) (SDXC Power Control) to use maximum performance (SDXC only)
            sendCmd(41, SD_OCR_VOLTAGE | sdType, SDIO_RESP_SHORT);
            if (recvResp(SDIO_RESP_SD_R3,response) != rOK) return getResult(SDR_BadResponse);
            // Check if card finished power up routine
            if (response[0] & (1 << 31)) break;
            if(OnDelayCommon::get()){
                return getResult(SDR_InvalidVoltage); // Unsupported voltage range
            }
        }
        // Check if card is SDHC/SDXC
        sd.Type = (response[0] & SD_HIGH_CAPACITY) ? SDCT_SDHC : SDCT_SDSC_V2;
    } else if (cmdRes.id == 2) {
        // SD v1.x or MMC

        // Issue CMD55 to reset 'Illegal command' bit of the SD card
        cmdRes = sendCmd(55, 0, SDIO_RESP_SHORT); // CMD55 with RCA 0

        // Issue ACMD41 command with zero argument
        while (true) {
            // Send leading command for ACMD<n> command
            cmdRes = sendCmd(55,0,SDIO_RESP_SHORT); // CMD55 with RCA 0
            if (recvResp(SDIO_RESP_SD_R1,response) != rOK) return getResult(SDR_BadResponse);
            // ACMD41 - initiate initialization process (bit HCS = 0)
            // R3 response do not protected with CRC and here will be CRC error
            cmdRes = sendCmd(41,SD_OCR_VOLTAGE,SDIO_RESP_SHORT);
            if (cmdRes.id == 2) break; // MMC will not respond to this command
            if (recvResp(SDIO_RESP_SD_R3,response) != rOK) return getResult(SDR_BadResponse);
            // Check if card finished power up routine
            if (response[0] & (1 << 31)) break;
            if(OnDelayCommon::get()){
                return getResult(SDR_UnknownCard);// Unsupported card
            }
        }
        if (cmdRes.id != 2) {
            // SD v1.x
            sd.Type = SDCT_SDSC_V1; // SDv1
        } else {
            // MMC or not SD memory card
            ///////////////////////////////////////////////////////////
            // This part is not checked due to lack of MMCmicro card //
            ///////////////////////////////////////////////////////////
            OnDelayCommon::again();
            while (true) {
                // Issue CMD1: initiate initialization process.
                sendCmd(1,SD_OCR_VOLTAGE,SDIO_RESP_SHORT); // CMD1
                if (recvResp(SDIO_RESP_SD_R3,response) != rOK) return getResult(SDR_BadResponse);
                // Check if card finished power up routine
                if (response[0] & (1 << 31)) break;
                if(OnDelayCommon::get()){
                    return getResult(SDR_UnknownCard);
                }
            }
            sd.Type = SDCT_MMC; // MMC
        }
    } else return cmdRes;

    // Now the CMD2 and CMD3 commands should be issued in cycle until timeout to enumerate all cards on the bus.
    // Since this module suitable to work with single card, issue this commands one time only.

    // Send ALL_SEND_CID command
    cmdRes = sendCmd(2,0,SDIO_RESP_LONG); // CMD2
    if (cmdRes != rOK) return cmdRes;
    recvResp(SDIO_RESP_SD_R2,(uint32_t *)sd.CID); // Retrieve CID register from the card

    // Send SEND_REL_ADDR command (ask the card to publish a new RCA (Relative Card Address)
    // Once the RCA is received the card state changes to the stand-by state
    if (sd.Type != SDCT_MMC) {
        // SD
        cmdRes = sendCmd(3,0,SDIO_RESP_SHORT); // CMD3
        if (cmdRes != rOK) return cmdRes;
        recvResp(SDIO_RESP_SD_R6,response);
    } else {
        // MMC
        cmdRes = sendCmd(3,1 << 16,SDIO_RESP_SHORT); // CMD3
        if (cmdRes != rOK) return cmdRes;
        recvResp(SDIO_RESP_SD_R2,response);
    }
    sd.RCA = response[0] >> 16;

    // Send SEND_CSD command
    cmdRes = sendCmd(9,sd.RCA << 16,SDIO_RESP_LONG); // CMD9
    if (cmdRes != rOK) return cmdRes;
    recvResp(SDIO_RESP_SD_R2,(uint32_t *)sd.CSD); // Retrieve CSD register from the card

    // Parse CID/CSD registers
    parseCardInfo();

    // Now card must be in stand-by mode, from this point it is possible to increase bus speed.

    // Configure SDIO peripheral clock
    // HW flow control disabled, Rising edge of SDIOCLK, 1-bit bus, Power saving disabled, SDIOCLK bypass disabled
//	SDIO->CLKCR = SD_BUS_1BIT | SDIO_CLK_DIV_TRAN | SDIO_CLKCR_CLKEN; // 1-bit bus, data transfer speed, clock enabled
    // HW flow control disabled, Rising edge of SDIOCLK, 1-bit bus, Power saving enabled, SDIOCLK bypass disabled
    //Configure SDIO clock control register
    setRegValShift(SDIO->CLKCR, SDIO_CLKCR_WIDBUS, sdioWideBus);
    setRegValShift(SDIO->CLKCR, SDIO_CLKCR_CLKDIV, clkDiv);
    setBit(SDIO->CLKCR, SDIO_CLKCR_CLKEN | SDIO_CLKCR_PWRSAV);

    // Put the SD card in transfer mode
    sendCmd(7,sd.RCA << 16,SDIO_RESP_SHORT); // CMD7
    cmdRes = recvResp(SDIO_RESP_SD_R1b,response);
    if (cmdRes != rOK) return cmdRes;

    // Disable the pull-up resistor on CD/DAT3 pin of card
    // Send leading command for ACMD<n> command
    sendCmd(55, sd.RCA << 16, SDIO_RESP_SHORT); // CMD55
    cmdRes = recvResp(SDIO_RESP_SD_R1, response);
    if (cmdRes != rOK) return cmdRes;
    // Send SET_CLR_CARD_DETECT command
    sendCmd(42, 0, SDIO_RESP_SHORT); // ACMD42
    cmdRes = recvResp(SDIO_RESP_SD_R1,response);
    if (cmdRes != rOK) return cmdRes;

    // For SDv1,SDv2 and MMC card must set block size (SDHC/SDXC always have fixed size 512bytes)
    if ((sd.Type == SDCT_SDSC_V1) || (sd.Type == SDCT_SDSC_V2) || (sd.Type == SDCT_MMC)) {
        sendCmd(16,512,SDIO_RESP_SHORT); // CMD16
        cmdRes = recvResp(SDIO_RESP_SD_R1,response);
        if (cmdRes != rOK) return getResult(SDR_SetBlockSizeFailed);
    }
/*
	// Turn off CRC checks - will it work in SDIO mode?
	SD_Cmd(SD_CMD_CRC_ON_OFF,0,SD_R1,resp); // CMD59
	// Turn on CRC checks
	SD_Cmd(SD_CMD_CRC_ON_OFF,1,SD_R1,resp); // CMD59
*/

    return getResult(SDR_Success);
}

// Abort an ongoing data transfer
// return: SDResult value
Result SimpleSDIO::stopTransfer() {
    uint32_t response;
    // Send STOP_TRANSMISSION command
    sendCmd(12,0,SDIO_RESP_SHORT); // CMD12
    return recvResp(SDIO_RESP_SD_R1,&response);
}

Result SimpleSDIO::readBlock(uint32_t addr, uint32_t *pBuf, uint32_t length) {
    Result cmdRes = getResult(SDR_Success);
    uint32_t response;
    uint32_t blkCount = length / 512; // Sectors in block
    uint32_t STA; // to speed up SDIO flags checking
    uint32_t STAmask; // mask for SDIO flags checking
    // Initialize the data control register
    SDIO->DCTRL = 0;
    // SDSC card uses byte unit address and
    // SDHC/SDXC cards use block unit address (1 unit = 512 bytes)
    // For SDHC card addr must be converted to block unit address
    if (sd.Type == SDCT_SDHC) addr >>= 9;
    // Clear the static SDIO flags
    SDIO->ICR = SDIO_ICR_STATIC;
    if (blkCount > 1) {
        // Send READ_MULT_BLOCK command
        sendCmd(18, addr, SDIO_RESP_SHORT); // CMD18
        // Prepare bit checking variable for multiple block transfer
        STAmask = SDIO_STA_RXOVERR | SDIO_STA_DCRCFAIL | SDIO_STA_DTIMEOUT | SDIO_STA_STBITERR | SDIO_STA_DATAEND;
    } else {
        // Send READ_SINGLE_BLOCK command
        cmdRes = sendCmd(17,addr,SDIO_RESP_SHORT); // CMD17
        // Prepare bit checking variable for single block transfer
        STAmask = SDIO_STA_RXOVERR | SDIO_STA_DCRCFAIL | SDIO_STA_DTIMEOUT | SDIO_STA_STBITERR | SDIO_STA_DBCKEND;
    }
    cmdRes = recvResp(SDIO_RESP_SD_R1, &response);
    if (cmdRes != rOK) return cmdRes;
    // Configure the SDIO data transfer
    SDIO->DTIMER = (busFreq / (clkDiv + 2) / 1000) * 100; // Data read timeout
    SDIO->DLEN = length; // Data length
    // Data transfer: block, card -> controller, size: 2^9 = 512bytes, enable transfer
    SDIO->DCTRL = SDIO_DCTRL_DTDIR | (9 << 4) | SDIO_DCTRL_DTEN;
    // Receive a data block from the SDIO
    // ----> TIME CRITICAL SECTION BEGIN <----
    do {
        STA = SDIO->STA;
        if (STA & SDIO_STA_RXFIFOHF) {
            // Receive FIFO half full, there are at least 8 words in it
            // This code is 80 bytes more than the 'for' loop, but faster
            *pBuf++ = SDIO->FIFO;
            *pBuf++ = SDIO->FIFO;
            *pBuf++ = SDIO->FIFO;
            *pBuf++ = SDIO->FIFO;
            *pBuf++ = SDIO->FIFO;
            *pBuf++ = SDIO->FIFO;
            *pBuf++ = SDIO->FIFO;
            *pBuf++ = SDIO->FIFO;
        }
    } while (!(STA & STAmask));
    // <---- TIME CRITICAL SECTION END ---->
    // Send stop transmission command in case of multiple block transfer
    if ((sd.Type != SDCT_MMC) && (blkCount > 1)) cmdRes = stopTransfer();
    // Check for errors
    if (STA & SDIO_STA_DTIMEOUT) cmdRes = getResult(SDR_DataTimeout);
    if (STA & SDIO_STA_DCRCFAIL) cmdRes = getResult(SDR_DataCRCFail);
    if (STA & SDIO_STA_RXOVERR) cmdRes = getResult(SDR_RXOverrun);
    if (STA & SDIO_STA_STBITERR) cmdRes = getResult(SDR_StartBitError);
    // Read data remnant from RX FIFO (if there is still any data)
    while (SDIO->STA & SDIO_STA_RXDAVL) *pBuf++ = SDIO->FIFO;
    // Clear the static SDIO flags
    SDIO->ICR = SDIO_ICR_STATIC;
    return cmdRes;
}

Result SimpleSDIO::writeBlock(uint32_t addr, uint32_t *pBuf, uint32_t length) {
    Result cmdRes = getResult(SDR_Success);
    uint32_t response; // SDIO command response
    uint32_t blkCount = length / 512; // Sectors in block
    uint32_t STA; // To speed up SDIO flags checking
    uint32_t STAmask; // Mask for SDIO flags checking
    uint32_t bsent = 0; // Counter of transferred bytes
    uint32_t wLeft; // Words counter in last portion of data
    uint8_t cardState; // Card state
    uint32_t cntr;

    // Initialize the data control register
    SDIO->DCTRL = 0;

    // SDSC card uses byte unit address and
    // SDHC/SDXC cards use block unit address (1 unit = 512 bytes)
    // For SDHC card addr must be converted to block unit address
    if (sd.Type == SDCT_SDHC) addr >>= 9;

    if (blkCount > 1) {
        // Send WRITE_MULTIPLE_BLOCK command
        sendCmd(25,addr,SDIO_RESP_SHORT); // CMD25
        // Prepare bit checking variable for multiple block transfer
        STAmask = SDIO_STA_TXUNDERR | SDIO_STA_DCRCFAIL | SDIO_STA_DTIMEOUT | SDIO_STA_STBITERR | SDIO_STA_DATAEND;
    } else {
        // Send WRITE_BLOCK command
        sendCmd(24, addr, SDIO_RESP_SHORT); // CMD24
        // Prepare bit checking variable for single block transfer
        STAmask = SDIO_STA_TXUNDERR | SDIO_STA_DCRCFAIL | SDIO_STA_DTIMEOUT | SDIO_STA_STBITERR | SDIO_STA_DBCKEND;
    }
    cmdRes = recvResp(SDIO_RESP_SD_R1, &response);
    if (cmdRes != rOK) return cmdRes;

    // Clear the static SDIO flags
    SDIO->ICR = SDIO_ICR_STATIC;

    // Configure the SDIO data transfer
    SDIO->DTIMER = (busFreq / (clkDiv + 2) / 1000) * 250; // Data write timeout
    SDIO->DLEN   = length; // Data length
    // Data transfer: block, controller -> card, size: 2^9 = 512bytes, enable transfer
    SDIO->DCTRL  = (9 << 4) | SDIO_DCTRL_DTEN;

    // Transfer data block to the SDIO
    // ----> TIME CRITICAL SECTION BEGIN <----
    if (!(length % 32)) {
        // The block length is multiple of 32, use simplified and faster transfer
        do {
            STA = SDIO->STA;
            if ((STA & SDIO_STA_TXFIFOHE) && (bsent < length)) {
                // TX FIFO half empty, at least 8 words can be written
                SDIO->FIFO = *pBuf++;
                SDIO->FIFO = *pBuf++;
                SDIO->FIFO = *pBuf++;
                SDIO->FIFO = *pBuf++;
                SDIO->FIFO = *pBuf++;
                SDIO->FIFO = *pBuf++;
                SDIO->FIFO = *pBuf++;
                SDIO->FIFO = *pBuf++;
                bsent += 32;
            }
        } while (!(STA & STAmask));
    } else {
        // The block length is not a multiple of 32, so it is necessary to apply additional calculations
        do {
            STA = SDIO->STA;
            if ((STA & SDIO_STA_TXFIFOHE) && (bsent < length)) {
                // TX FIFO half empty, at least 8 words can be written
                if (length - bsent < 32) {
                    // Write last portion of data to the TX FIFO
                    wLeft = ((length - bsent) % 4 == 0) ? ((length - bsent) >> 2) : (((length - bsent) >> 2) + 1);
                    for (cntr = 0; cntr < wLeft; cntr++) SDIO->FIFO = *pBuf++;
                    bsent += wLeft << 2;
                } else {
                    // Write 8 words to the TX FIFO
                    SDIO->FIFO = *pBuf++;
                    SDIO->FIFO = *pBuf++;
                    SDIO->FIFO = *pBuf++;
                    SDIO->FIFO = *pBuf++;
                    SDIO->FIFO = *pBuf++;
                    SDIO->FIFO = *pBuf++;
                    SDIO->FIFO = *pBuf++;
                    SDIO->FIFO = *pBuf++;
                    bsent += 32;
                }
            }
        } while (!(STA & STAmask));
    }
    // <---- TIME CRITICAL SECTION END ---->

    // Send stop transmission command in case of multiple block transfer
    if ((sd.Type != SDCT_MMC) && (blkCount > 1)) cmdRes = stopTransfer();

    // Check for errors+
    if (STA & SDIO_STA_DTIMEOUT) cmdRes = getResult(SDR_DataTimeout);
    if (STA & SDIO_STA_DCRCFAIL) cmdRes = getResult(SDR_DataCRCFail);
    if (STA & SDIO_STA_TXUNDERR) cmdRes = getResult(SDR_TXUnderrun);
    if (STA & SDIO_STA_STBITERR) cmdRes = getResult(SDR_StartBitError);

    // Wait till the card is in programming state
    do {
        if (getCardState(&cardState) != rOK) break;
    } while ((cardState == SD_STATE_PRG) || (cardState == SD_STATE_RCV));

    // Clear the static SDIO flags
    SDIO->ICR = SDIO_ICR_STATIC;

    return cmdRes;
}

Result SimpleSDIO::getResult(SdioResultEnum resultENum) {
    switch (resultENum) {
        case SDR_Success:
            return ResultBuilder::getOK(resultENum, "SDR Success");
        case SDR_Timeout:
            return ResultBuilder::getError(resultENum, "SDR Timeout");
        case SDR_CRCError:
            return ResultBuilder::getError(resultENum, "SDR CRCError");
        case SDR_ReadError:
            return ResultBuilder::getError(resultENum, "SDR ReadError");
        case SDR_WriteError:
            return ResultBuilder::getError(resultENum, "SDR WriteError");
        case SDR_WriteErrorInternal:
            return ResultBuilder::getError(resultENum, "SDR WriteErrorInternal");
        case SDR_Unsupported:
            return ResultBuilder::getError(resultENum, "SDR Unsupported");
        case SDR_BadResponse:
            return ResultBuilder::getError(resultENum, "SDR BadResponse");
        case SDR_SetBlockSizeFailed:
            return ResultBuilder::getError(resultENum, "SDR SetBlockSizeFailed");
        case SDR_UnknownCard:
            return ResultBuilder::getError(resultENum, "SDR UnknownCard");
        case SDR_NoResponse:
            return ResultBuilder::getError(resultENum, "SDR NoResponse");
        case SDR_AddrOutOfRange:
            return ResultBuilder::getError(resultENum, "SDR AddrOutOfRange");
        case SDR_WriteCRCError:
            return ResultBuilder::getError(resultENum, "SDR WriteCRCError");
        case SDR_InvalidVoltage:
            return ResultBuilder::getError(resultENum, "SDR InvalidVoltage");
        case SDR_DataTimeout:
            return ResultBuilder::getError(resultENum, "SDR DataTimeout");
        case SDR_DataCRCFail:
            return ResultBuilder::getError(resultENum, "SDR DataCRCFail");
        case SDR_RXOverrun:
            return ResultBuilder::getError(resultENum, "SDR RXOverrun");
        case SDR_TXUnderrun:
            return ResultBuilder::getError(resultENum, "SDR TXUnderrun");
        case SDR_StartBitError:
            return ResultBuilder::getError(resultENum, "SDR StartBitError");
        case SDR_AddrMisaligned:
            return ResultBuilder::getError(resultENum, "SDR AddrMisaligned");
        case SDR_BlockLenError:
            return ResultBuilder::getError(resultENum, "SDR BlockLenError");
        case SDR_EraseSeqError:
            return ResultBuilder::getError(resultENum, "SDR EraseSeqError");
        case SDR_EraseParam:
            return ResultBuilder::getError(resultENum, "SDR EraseParam");
        case SDR_WPViolation:
            return ResultBuilder::getError(resultENum, "SDR WPViolation");
        case SDR_LockUnlockFailed:
            return ResultBuilder::getError(resultENum, "SDR LockUnlockFailed");
        case SDR_ComCRCError:
            return ResultBuilder::getError(resultENum, "SDR ComCRCError");
        case SDR_IllegalCommand:
            return ResultBuilder::getError(resultENum, "SDR IllegalCommand");
        case SDR_CardECCFailed:
            return ResultBuilder::getError(resultENum, "SDR CardECCFailed");
        case SDR_CCError:
            return ResultBuilder::getError(resultENum, "SDR CCError");
        case SDR_GeneralError:
            return ResultBuilder::getError(resultENum, "SDR GeneralError");
        case SDR_StreamUnderrun:
            return ResultBuilder::getError(resultENum, "SDR StreamUnderrun");
        case SDR_StreamOverrun:
            return ResultBuilder::getError(resultENum, "SDR StreamOverrun");
        case SDR_CSDOverwrite:
            return ResultBuilder::getError(resultENum, "SDR CSDOverwrite");
        case SDR_WPEraseSkip:
            return ResultBuilder::getError(resultENum, "SDR WPEraseSkip");
        case SDR_ECCDisabled:
            return ResultBuilder::getError(resultENum, "SDR ECCDisabled");
        case SDR_EraseReset:
            return ResultBuilder::getError(resultENum, "SDR EraseReset");
        case SDR_AKESeqError:
            return ResultBuilder::getError(resultENum, "SDR AKESeqError");
        case SDR_UnknownError:
            return ResultBuilder::getError(resultENum, "SDR UnknownError");
        default:
            return ResultBuilder::getError(255, "Unknown result");
    }
}
