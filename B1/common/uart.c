/****************************************************************************
  UART System Source File
  Company : Singular Software
  File Name : uart.c
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

#include <xc.h>

#include "uart.h"

#ifndef UART_CLOCK_FREQUENCY
#define UART_CLOCK_FREQUENCY  (uint32_t)(100000000UL)
#endif

#define USTA_OERR_MASK   _U1STA_OERR_MASK
#define USTA_FERR_MASK   _U1STA_FERR_MASK
#define USTA_PERR_MASK   _U1STA_PERR_MASK
#define USTA_UTXBF_MASK  _U1STA_UTXBF_MASK
#define USTA_UTXBF_MASK  _U1STA_UTXBF_MASK
#define USTA_TRMT_MASK   _U1STA_TRMT_MASK
#define USTA_URXDA_MASK  _U1STA_URXDA_MASK
#define USTA_UTXEN_MASK  _U1STA_UTXEN_MASK
#define USTA_URXEN_MASK  _U1STA_URXEN_MASK
#define UMODE_ON_MASK    _U1MODE_ON_MASK
#define UMODE_PDSEL_MASK _U1MODE_PDSEL_MASK
#define UMODE_BRGH_MASK  _U1MODE_BRGH_MASK
#define UMODE_STSEL_MASK _U1MODE_STSEL_MASK

typedef struct
{
    volatile uint32_t * USTACLR;
    volatile uint32_t * USTA;
    volatile uint32_t * URXREG;
    volatile uint32_t * UTXREG;
    volatile uint32_t * UBRG;
    volatile uint32_t * UMODESET;
    volatile uint32_t * UMODECLR;
    volatile uint32_t * UMODE;
    volatile uint32_t * USTASET;
} UART_SFRS_PTRS;

static UART_SFRS_PTRS Uart[6];

bool static SetSfrs( UART_NUMBER uart, UART_SFRS_PTRS * sfrsPtrs)
{
    bool result = true;
    if (sfrsPtrs != NULL)
    {
        switch(uart)
        {
            case UART1: 
                sfrsPtrs->UMODE    = &U1MODE;
                sfrsPtrs->USTA     = &U1STA;
                sfrsPtrs->URXREG   = &U1RXREG;
                sfrsPtrs->UTXREG   = &U1TXREG;
                sfrsPtrs->UBRG     = &U1BRG;
                sfrsPtrs->USTACLR  = &U1STACLR;
                sfrsPtrs->UMODESET = &U1MODESET;
                sfrsPtrs->UMODECLR = &U1MODECLR;
                sfrsPtrs->USTASET  = &U1STASET;
            break;
            case UART2: 
                sfrsPtrs->UMODE    = &U2MODE;
                sfrsPtrs->USTA     = &U2STA;
                sfrsPtrs->URXREG   = &U2RXREG;
                sfrsPtrs->UTXREG   = &U2TXREG;
                sfrsPtrs->UBRG     = &U2BRG;
                sfrsPtrs->USTACLR  = &U2STACLR;
                sfrsPtrs->UMODESET = &U2MODESET;
                sfrsPtrs->UMODECLR = &U2MODECLR;
                sfrsPtrs->USTASET  = &U2STASET;
            break;
            case UART3: 
                sfrsPtrs->UMODE    = &U3MODE;
                sfrsPtrs->USTA     = &U3STA;
                sfrsPtrs->URXREG   = &U3RXREG;
                sfrsPtrs->UTXREG   = &U3TXREG;
                sfrsPtrs->UBRG     = &U3BRG;
                sfrsPtrs->USTACLR  = &U3STACLR;
                sfrsPtrs->UMODESET = &U3MODESET;
                sfrsPtrs->UMODECLR = &U3MODECLR;
                sfrsPtrs->USTASET  = &U3STASET;
            break;
            case UART4: 
                sfrsPtrs->UMODE    = &U4MODE;
                sfrsPtrs->USTA     = &U4STA;
                sfrsPtrs->URXREG   = &U4RXREG;
                sfrsPtrs->UTXREG   = &U4TXREG;
                sfrsPtrs->UBRG     = &U4BRG;
                sfrsPtrs->USTACLR  = &U4STACLR;
                sfrsPtrs->UMODESET = &U4MODESET;
                sfrsPtrs->UMODECLR = &U4MODECLR;
                sfrsPtrs->USTASET  = &U4STASET;
            break;
            case UART5: 
                sfrsPtrs->UMODE    = &U5MODE;
                sfrsPtrs->USTA     = &U5STA;
                sfrsPtrs->URXREG   = &U5RXREG;
                sfrsPtrs->UTXREG   = &U5TXREG;
                sfrsPtrs->UBRG     = &U5BRG;
                sfrsPtrs->USTACLR  = &U5STACLR;
                sfrsPtrs->UMODESET = &U5MODESET;
                sfrsPtrs->UMODECLR = &U5MODECLR;
                sfrsPtrs->USTASET  = &U5STASET;
            break;
            case UART6: 
                sfrsPtrs->UMODE    = &U6MODE;
                sfrsPtrs->USTA     = &U6STA;
                sfrsPtrs->URXREG   = &U6RXREG;
                sfrsPtrs->UTXREG   = &U6TXREG;
                sfrsPtrs->UBRG     = &U6BRG;
                sfrsPtrs->USTACLR  = &U6STACLR;
                sfrsPtrs->UMODESET = &U6MODESET;
                sfrsPtrs->UMODECLR = &U6MODECLR;
                sfrsPtrs->USTASET  = &U6STASET;
            break;
            default:
                sfrsPtrs->UMODE    = NULL;
                sfrsPtrs->USTA     = NULL;
                sfrsPtrs->URXREG   = NULL;
                sfrsPtrs->UTXREG   = NULL;
                sfrsPtrs->UBRG     = NULL;
                sfrsPtrs->USTACLR  = NULL;
                sfrsPtrs->UMODESET = NULL;
                sfrsPtrs->UMODECLR = NULL;
                sfrsPtrs->USTASET  = NULL;
                result = false;
            break;
        }
    }
    
    return result;
}

void UART_Initialize( void )
{
    static int i = 6;
    /* one time init. */
    while (i > 0)
    {
        SetSfrs(i-1, &Uart[i-1]);
        i--;
    }
}
void UARTInitialize( UART_NUMBER uart )
{
    UART_Initialize();
    
    UARTSetup(uart, BAUDRATE_115200, DATA_8_BITS, STOP_1_BIT, PARITY_NONE, UART_CLOCK_FREQUENCY);
}

bool UARTSetup(UART_NUMBER uart, UART_BAUDRATE baudRate, UART_DATA_BITS dataBits, UART_STOP_BIT stopBits, UART_PARITY parity, uint32_t uartCloclFrequency)
{
    bool result = false;
    uint32_t brgh1 = 0;
    uint32_t brgh0 = 0;
    uint32_t uartMode;

    /* Initialize the UxBRG register for the appropriate baud rate. */
    /* For BRGH = 0, UxBRG = (FPB / 16 * Baud Rate) - 1 */
    /* For BRGH = 1, UxBRG = (FPB / 4 * Baud Rate) - 1 */
    brgh0 = ((uartCloclFrequency / (uint32_t)baudRate) >> 4) - 1;
    brgh1 = ((uartCloclFrequency / (uint32_t)baudRate) >> 2) - 1;
    if((brgh1 >= 0) && (brgh1 <= UINT16_MAX))
    {
        *Uart[uart].UMODESET = UMODE_BRGH_MASK;
        *Uart[uart].UBRG = brgh1;
        result = true;
    }
    else if ((brgh0 >= 0) && (brgh0 <= UINT16_MAX))
    {
        *Uart[uart].UMODECLR = UMODE_BRGH_MASK;
        *Uart[uart].UBRG = brgh0;
        result = true;
    }

    if (result)
    {
        /*Set the number of data and Stop bits, and parity selection */
        switch (dataBits)
        {
            case DATA_8_BITS:
                uartMode = *Uart[uart].UMODE;
                uartMode &= ~UMODE_PDSEL_MASK;
                *Uart[uart].UMODE = uartMode | (uint32_t)parity ;
                break;
            case DATA_9_BITS:
                result = parity == PARITY_NONE;
                if(result)
                {
                   uartMode = *Uart[uart].UMODE;
                   uartMode &= ~UMODE_PDSEL_MASK;
                   *Uart[uart].UMODE = uartMode | dataBits;
                }
                break;
            default:
                result = false;
                break;
        }

        if (result)
        {
            /* Configure UART mode */
            uartMode = *Uart[uart].UMODE;
            uartMode &= ~UMODE_STSEL_MASK;
            *Uart[uart].UMODE = uartMode | (uint32_t)stopBits ;
        }
    }

     if (result)
    {
        /* Enable the UART receiver by setting the URXEN bit. */
        *Uart[uart].USTASET = (USTA_UTXEN_MASK | USTA_URXEN_MASK);

        /* Enable the UART module by setting the ON bit. */
        *Uart[uart].UMODESET = UMODE_ON_MASK;
    }
    
    return result;
}

void static UARTClearErrors( UART_NUMBER uart )
{
    uint8_t b = 0;
    uint8_t c = 8 + 1;/* 8 Fifo + 1 RX register */

    /* Clear overrun error */
    if(*Uart[uart].USTA & USTA_OERR_MASK)
    {
        *Uart[uart].USTACLR = USTA_OERR_MASK;
    }

    /* Read bytes to clear errors */
    while((*Uart[uart].USTA & (USTA_FERR_MASK | USTA_PERR_MASK)) && c > 0)
    {
        b = (uint8_t )(*Uart[uart].URXREG );
        c--;
    }
}

UART_ERROR UARTGetError( UART_NUMBER uart, bool clear )
{
    uint32_t status;
    UART_ERROR errors;
    
    status = *Uart[uart].USTA;

    errors = (UART_ERROR)(status & (USTA_OERR_MASK | USTA_FERR_MASK | USTA_PERR_MASK));

    if(errors != ERROR_NONE && clear)
    {
        UARTClearErrors(uart);
    }

    return errors;
}

bool UARTTxReady( UART_NUMBER uart )
{
    bool result = false;
    
    result = (!(*Uart[uart].USTA & USTA_UTXBF_MASK));

    return result;
}

void UARTWriteByte(UART_NUMBER uart, int data)
{
    while ((*Uart[uart].USTA & USTA_UTXBF_MASK));

    *Uart[uart].UTXREG = data;
}

bool UARTTxComplete( UART_NUMBER uart )
{
    bool result = false;
        
    result = (*Uart[uart].USTA & USTA_TRMT_MASK);
    
    return result;
}

bool UARTRxReady( UART_NUMBER uart )
{
    bool result = false;

    result = (USTA_URXDA_MASK == (*Uart[uart].USTA & USTA_URXDA_MASK));

    return result;
}

int UARTReadByte( UART_NUMBER uart )
{
    return(*Uart[uart].URXREG);
}
