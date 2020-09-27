/****************************************************************************
  UART System Source File
  Company : Singular Software
  File Name : uart.h
  Summary : configure and control functions.
  Description :  initialize and control PIC32MZ uart system.
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

#ifndef _UART_H 
#define _UART_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
    
typedef enum
{
    UART1,
    UART2,
    UART3,
    UART4,
    UART5,
    UART6
}UART_NUMBER;

typedef enum
{
    BAUDRATE_110 = 110,
    BAUDRATE_150 = 150,
    BAUDRATE_300 = 300,
    BAUDRATE_1200 = 1200,
    BAUDRATE_2400 = 2400,
    BAUDRATE_4800 = 4800,
    BAUDRATE_9600 = 9600,
    BAUDRATE_19200 = 19200,
    BAUDRATE_38400 = 38400,
    BAUDRATE_57600 = 57600,
    BAUDRATE_115200 = 115200,
    BAUDRATE_230400 = 230400,
    BAUDRATE_460800 = 460800,
    BAUDRATE_921600 = 921600        
} UART_BAUDRATE;

typedef enum
{
    DATA_8_BITS = 0x00,
    DATA_9_BITS = 0x01,
} UART_DATA_BITS;

typedef enum
{
    STOP_1_BIT = 0x00,
    STOP_2_BIT = 0x01,
} UART_STOP_BIT;

typedef enum
{
    PARITY_NONE = 0x00,
    PARITY_EVEN = 0x02,
    PARITY_ODD =  0x04,
} UART_PARITY;

typedef enum
{
    ERROR_NONE = 0,
    ERROR_OVERRUN = 0x00000002,
    ERROR_FRAMING = 0x00000004,
    ERROR_PARITY  = 0x00000008

} UART_ERROR;

void UART_Initialize( void );
void UARTInitialize( UART_NUMBER uart );
bool UARTSetup( UART_NUMBER uart, UART_BAUDRATE baudRate, UART_DATA_BITS dataBits, UART_STOP_BIT stopBits, UART_PARITY parity, uint32_t uartCloclFrequency );
UART_ERROR UARTGetError( UART_NUMBER uart, bool clear );
bool UARTTxReady( UART_NUMBER uart );
void UARTWriteByte(UART_NUMBER uart, int data);
bool UARTTxComplete( UART_NUMBER uart );
bool UARTRxReady( UART_NUMBER uart );
int UARTReadByte( UART_NUMBER uart );

#ifdef __cplusplus
}
#endif

#endif /* _UART_H */

