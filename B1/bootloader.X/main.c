/****************************************************************************
  Main Source File
  Company : Singular Software
  File Name : main.c
  Summary : main function of the bootloader.
  Description :  main initialize the system and start the bootloader task.
****************************************************************************/

/*******************************************************************************
* Copyright (C) 2020 Singular Software Limited. 
* THIS SOFTWARE IS PROVIDED BY SIGNULAR SOFTWARE "AS IS" AND ANY EXPRESS OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
* IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY 
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "sys/kmem.h"
#include <xc.h>
#include <sys/attribs.h>
#include <sys/types.h>
#include <string.h>

#include "initialize.h"

static const uint8_t BootloaderVersion[3] = { 1,0,0 };
/*
 * bootloader version 1.0.0 - first release
 */

#define CRC_SIZE 4
//#define RAM_START   KVA0_TO_KVA1 (0x80000000)
//#define TRIGGER_CODE             (0x5048434DUL)
#define FIRMWARE_START_ADDRESS   (PA_TO_KVA0(0x1d000000UL))

 typedef enum 
{
    START  = 0x01,
    END    = 0x04,
    ESCAPE = 0x10,
} MSG_FRAM;
typedef enum
{
    HANDSHAKE_CMD = 55,
    VERSION_CMD,
    ADDRESS_CMD,
    PROGRAM_CMD,
    VERIFY_CMD,
    START_FIRMWARE_CMD,
    RESERT_CMD
} COMMANDS;
typedef enum
{
    HANDSHAKE_REP = HANDSHAKE_CMD + 100,
    VERSION_REP,
    ADDRESS_REP,
    PROGRAM_REP,
    VERIFY_REP,
    START_FIRMWARE_REP,
    RESERT_REP,
    ERROR_REP
} RESPONSE;
typedef struct
{  
    uint8_t buffer[FLASH_ERASE_BLOCK_SIZE + 1 + 4 + CRC_SIZE];//command + address + crc
    bool handshake;
    int32_t receivedDataLength;
    int32_t sendDataLength;
    bool validCommand;  
    uint32_t startAddress;
    uint32_t endAddress;    
} DATA;
DATA bootloaderData =
{
    .handshake = false,
    .receivedDataLength = 0,
    .sendDataLength = 0,
    .validCommand = false,
    .startAddress = 0,
    .endAddress = 0
};

#define BOOTLOADER_UART UART2

void bootloaderTask( void );

int main ( void )
{
    Initialize();

    //forever loop, does not return
    bootloaderTask();
    
    return ( EXIT_FAILURE );
}

/*     CRC     */
static uint32_t _crc32Table[256] = {0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
                                    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
                                    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
                                    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
                                    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
                                    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
                                    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
                                    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
                                    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
                                    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
                                    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
                                    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
                                    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
                                    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
                                    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
                                    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
                                    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
                                    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
                                    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
                                    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
                                    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
                                    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
                                    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
                                    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
                                    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
                                    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
                                    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
                                    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
                                    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
                                    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
                                    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
                                    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D };   
uint32_t crc32(uint8_t *data, uint32_t len)
{
    uint32_t crc = 0xFFFFFFFF;
    uint32_t i = 0;
    uint8_t b;

    while (len-- != 0)
    {
        b = *(data + i++);
        crc = _crc32Table[(crc ^ b) & 0xFF] ^ (crc >> 8);
    }

    crc ^= 0xFFFFFFFF;

    return crc;
}
uint32_t flashCrc32(uint32_t start, uint32_t len)
{    
    uint32_t crc = 0xFFFFFFFF;
    uint32_t i = 0;
    uint8_t b;

    for (i = 0; i < len; i++)
    {
        b = *(uint8_t *)KVA0_TO_KVA1((start + i));
        crc = _crc32Table[(crc ^ b) & 0xFF] ^ (crc >> 8);
    }
    
    crc ^= 0xFFFFFFFF;

    return crc;
}



/*     BOOTLOADER     */
uint32_t __attribute__((coherent)) block[FLASH_ERASE_BLOCK_SIZE / sizeof(uint32_t)];

extern uint32_t *systemRamStartAddress;// = (uint32_t *)RAM_START;

bool bootloaderSoftwareTrigger(void)
{
    if (SYSTEM_BOOTLOADER_TRIGGER_CODE == systemRamStartAddress[0] && 
        SYSTEM_BOOTLOADER_TRIGGER_CODE == systemRamStartAddress[1] &&
        SYSTEM_BOOTLOADER_TRIGGER_CODE == systemRamStartAddress[2] &&
        SYSTEM_BOOTLOADER_TRIGGER_CODE == systemRamStartAddress[3])
    {
        systemRamStartAddress[0] = 0;
        return true;
    }
    
    return false;
}
bool bootloaderHardwareTrigger( void )
{
    uint32_t portH = GPIOReadPort(PORT_H);
            
    return !(portH & 0x00000008);/* RH3 */
}

UART_ERROR bootloaderGetError (void )
{
   return UARTGetError(BOOTLOADER_UART, true);
}
bool bootloaderRxReady( void )
{
    return UARTRxReady(BOOTLOADER_UART);
}
uint8_t bootloaderReadByte( void )
{
    return UARTReadByte(BOOTLOADER_UART);
}
void bootloaderWriteByte( uint8_t b )
{
    return UARTWriteByte(BOOTLOADER_UART, (int)b);
}
void bootloaderSendResponse()
{
    int i;
    
    uint32_t crc = crc32(bootloaderData.buffer, bootloaderData.sendDataLength);
    
    bootloaderData.buffer[bootloaderData.sendDataLength++] = (uint8_t)crc;
    bootloaderData.buffer[bootloaderData.sendDataLength++] = (crc >> 8);
    bootloaderData.buffer[bootloaderData.sendDataLength++] = (crc >> 16);
    bootloaderData.buffer[bootloaderData.sendDataLength++] = (crc >> 24);
        
    /* Start */
    bootloaderWriteByte(START);
    
    for (i = 0; i < bootloaderData.sendDataLength; i++)
    {
        switch (bootloaderData.buffer[i])
        {
            case START:
            case END:
            case ESCAPE:
            {
                bootloaderWriteByte(ESCAPE);
            }
            default:
                bootloaderWriteByte(bootloaderData.buffer[i]);
        }
    }
    
    /* End */
    bootloaderWriteByte(END);
}
void bootloaderSendErrorResponse()
{
    bootloaderData.buffer[0] = ERROR_REP;
    bootloaderData.sendDataLength = 1;
    bootloaderSendResponse();
}
void bootloaderReceive( void )
{
    bool escape = false;
    bootloaderData.validCommand = false;
    bootloaderData.receivedDataLength = 0;
    
    while (!bootloaderData.validCommand)
    {
        if (bootloaderGetError() != ERROR_NONE)
        {
            bootloaderData.validCommand = false;
            bootloaderData.receivedDataLength = 0;
        }
        
        if (bootloaderRxReady())
        {            
            uint8_t b = bootloaderReadByte();

            switch (b)
            {
                case START: /* check if start */
                {
                    if (escape)
                    {
                        /* Received byte is not Start, it is data */
                        bootloaderData.buffer[bootloaderData.receivedDataLength++] = b;
                        escape = false;
                    }
                    else
                    {
                        /* Received byte is Start */
                        bootloaderData.receivedDataLength = 0; 
                    }
                    break;
                }

                case END: /* check if end */
                {
                    if (escape)
                    {
                        /* Received byte is not End, it is data */
                        bootloaderData.buffer[bootloaderData.receivedDataLength++] = b;
                        escape = false;
                    }
                    else
                    {
                        /* Received byte is end , Check CRC */
                        uint32_t crc = 0;
                        if (bootloaderData.receivedDataLength > sizeof(crc))
                        {
                            crc = (bootloaderData.buffer[bootloaderData.receivedDataLength - 4]) + 
                                 ((bootloaderData.buffer[bootloaderData.receivedDataLength - 3]) << 8) +
                                 ((bootloaderData.buffer[bootloaderData.receivedDataLength - 2]) << 16) +
                                 ((bootloaderData.buffer[bootloaderData.receivedDataLength - 1]) << 24);
                            
                            bootloaderData.receivedDataLength -= CRC_SIZE;
       
                            bootloaderData.validCommand = crc32(bootloaderData.buffer,
                                                               bootloaderData.receivedDataLength) == crc;
                        }
                    }
                    break;
                }

                case ESCAPE: /* Check if escape */
                {
                    if (escape)
                    {
                        /* Received byte is not escape, it is data. */
                        bootloaderData.buffer[bootloaderData.receivedDataLength++] = b;
                        escape = false;
                    }
                    else
                    {
                        /* Received byte is escape */
                        escape = true;
                    }
                    break;
                }
                default: // Data
                {
                    bootloaderData.buffer[bootloaderData.receivedDataLength++] = b;
                    escape = false;
                    break;
                }
            }
                 
            if (bootloaderData.receivedDataLength > sizeof(bootloaderData.buffer))
            {
                /* should never get here */
                bootloaderSendErrorResponse();
                escape = false;
                bootloaderData.validCommand = false;
                bootloaderData.receivedDataLength = 0;
            }

        }
    }
}
void bootloaderStartFirmware( void )
{    
    uint32_t firstWordOfFirmware = *(uint32_t *)(FIRMWARE_START_ADDRESS);
    if (firstWordOfFirmware == 0xFFFFFFFF)
    {
        /* flash is not programmed nothing to run */
        return;
    }
    
    void (*entryPoint)(void);
    entryPoint = (void (*)(void))FIRMWARE_START_ADDRESS;
    entryPoint();
}
void bootloaderResetBoard( void )
{
    /* system unlock sequence */ 
    SYSKEY = 0x00000000;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    RSWRSTSET = _RSWRST_SWRST_MASK;
    (void)RSWRST;
}
void bootloaderProcess( void )
{
    uint32_t crc;
    uint32_t start;
    uint32_t length;
    bool success = false;
    
    /* the first command to receive must be handshake */
    if (!bootloaderData.handshake && bootloaderData.buffer[0] != HANDSHAKE_CMD)
    {
        bootloaderSendErrorResponse();
    }
    
    switch (bootloaderData.buffer[0])
    {
        case HANDSHAKE_CMD:
        {
            bootloaderData.handshake = true;
            bootloaderData.buffer[0] = HANDSHAKE_REP;
            bootloaderData.sendDataLength = 1;
            bootloaderSendResponse();
            break;
        }
        case VERSION_CMD:
        {
            bootloaderData.buffer[0] = VERSION_REP;
            memcpy(&bootloaderData.buffer[1], BootloaderVersion, sizeof(BootloaderVersion));
            bootloaderData.sendDataLength = 1 +  sizeof(BootloaderVersion);
            bootloaderSendResponse();
            break;
        }
        case ADDRESS_CMD:
        {
            bootloaderData.buffer[0] = ADDRESS_REP;
            memcpy(&start, &bootloaderData.buffer[1], sizeof(start));
            memcpy(&length, &bootloaderData.buffer[5], sizeof(length));
            
            bootloaderData.startAddress = (start & OFFSET_ALIGN_MASK);
            bootloaderData.endAddress = bootloaderData.startAddress + (length & SIZE_ALIGN_MASK);
            
            /* only allow programming of program flash */
            if (bootloaderData.endAddress > bootloaderData.startAddress && 
                bootloaderData.endAddress <= (FLASH_START_ADDRESS + FLASH_LENGTH))
            {
                bootloaderData.sendDataLength = 1;
                bootloaderSendResponse();
            }
            else
            {
                bootloaderData.startAddress = 0;
                bootloaderData.endAddress = 0;
                bootloaderSendErrorResponse();
            }
            break;
        }
        case PROGRAM_CMD:
        {
            success = false;
            /* only allow programming of program flash */
            if (bootloaderData.endAddress > bootloaderData.startAddress && 
                bootloaderData.endAddress <= (FLASH_START_ADDRESS + FLASH_LENGTH))
            {
                uint32_t currentAddress;
                uint32_t currentPage = 0;
                uint32_t currentPageIndex  = 0; 

                memcpy(&currentAddress, &bootloaderData.buffer[1], sizeof(currentAddress));
                memcpy(block, &bootloaderData.buffer[5], FLASH_ERASE_BLOCK_SIZE);

                if (currentAddress >= bootloaderData.startAddress &&
                    currentAddress < bootloaderData.endAddress)
                {
                    FlashPageEraseOperation(currentAddress);

                    /* clear flash memory block */
                    while( FlashOperationInProgress());

                    /* program pages */
                    for (currentPage = 0; currentPage < PAGES_IN_ERASE_BLOCK; currentPage++)
                    {
                        FlashRowProgramOperation(&block[currentPageIndex], currentAddress);
                        
                        while (FlashOperationInProgress());

                        currentAddress += FLASH_PAGE_SIZE;
                        currentPageIndex += (FLASH_PAGE_SIZE / sizeof(uint32_t));
                    }
                    success = currentPage == PAGES_IN_ERASE_BLOCK;
                }
            }
            
            if (success)
            {
                bootloaderData.buffer[0] = PROGRAM_REP;
                bootloaderData.sendDataLength = 1;
                bootloaderSendResponse();
            }
            else
            {
                 bootloaderSendErrorResponse();
            }
            
            break;
        }
        case VERIFY_CMD:
        {            
            memcpy(&start,  &bootloaderData.buffer[1], sizeof(start));
            memcpy(&length, &bootloaderData.buffer[5], sizeof(length));
            memcpy(&crc,    &bootloaderData.buffer[9], sizeof(crc));
           
            if (flashCrc32(start, length) == crc)
            {
                bootloaderData.buffer[0] = VERIFY_REP;
                bootloaderData.sendDataLength = 1;
                bootloaderSendResponse();
            }
            else
            {
                bootloaderSendErrorResponse();
            }
        
            break;
        }
        case START_FIRMWARE_CMD:
        {
            bootloaderData.buffer[0] = START_FIRMWARE_REP;
            bootloaderData.sendDataLength = 1;
            bootloaderSendResponse();
            bootloaderStartFirmware();
            
            break;
        }
        case RESERT_CMD:
        {
            bootloaderData.buffer[0] = RESERT_REP;
            bootloaderData.sendDataLength = 1;
            bootloaderSendResponse();
            bootloaderResetBoard();
            
            break;
        }
        default:
            bootloaderSendErrorResponse();
            break;
    }
}

void BootloaderRunApplication( void )
{
    if (bootloaderHardwareTrigger() || bootloaderSoftwareTrigger())
    {
        return;
    }

    //if firmware found on flash this call will not return
    bootloaderStartFirmware();
}

void bootloaderTask( void )
{    
    while (true)
    {
        bootloaderReceive();

        if (bootloaderData.validCommand)
        {   
            bootloaderProcess();
        }
    }
}