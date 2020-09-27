/****************************************************************************
  GPIO System Source File
  Company : Singular Software
  File Name : gpio.h
  Summary : configure and control functions.
  Description :  initialize and control PIC32MZ gpio system.
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

#ifndef _GPIO_H    
#define _GPIO_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    PORT_A,
    PORT_B,
    PORT_C,
    PORT_D,
    PORT_E,
    PORT_F,
    PORT_G,
    PORT_H,
    PORT_J,
    PORT_K
} GPIO_PORT;

#define RPD2  0b0000
#define RPG8  0b0001
#define RPF4  0b0010
#define RPD10 0b0011
#define RPF1  0b0100
#define RPB9  0b0101
#define RPB10 0b0110
#define RPC14 0b0111
#define RPB5  0b1000
//Reserved    0b1001
#define RPC1  0b1010
#define RPD14 0b1011
#define RPG1  0b1100
#define RPA14 0b1101
#define RPD6  0b1110
//Reserved    0b1111        
#define RPD3  0b1110
#define RPG7  0b1110
#define RPF5  0b1110
#define RPD11 0b1110
#define RPF0  0b1110
#define RPB1  0b1110
#define RPE5  0b1110
#define RPC13 0b1110
#define RPB3  0b1110
//Reserved    0b1110
#define RPC4  0b1110
#define RPD15 0b1110
#define RPG0  0b1110
#define RPA15 0b1110
#define RPD7  0b1110
//Reserved    0b1111 
#define RPD9  0b0000
#define RPG6  0b0001
#define RPB8  0b0010
#define RPB15 0b0011
#define RPD4  0b0100
#define RPB0  0b0101
#define RPE3  0b0110
#define RPB7  0b0111
//Reserved    0b1000
#define RPF12 0b1001
#define RPD12 0b1010
#define RPF8  0b1011
#define RPC3  0b1100
#define RPE9  0b1101
//Reserved    0b1110
//Reserved    0b1111
#define RPD1  0b0000
#define RPG9  0b0001
#define RPB14 0b0010
#define RPD0  0b0011
//Reserved    0b0100
#define RPB6  0b0101
#define RPD5  0b0110
#define RPB2  0b0111
#define RPF3  0b1000
#define RPF13 0b1001
//No Connect  0b1010
#define RPF2  0b1011
#define RPC2  0b1100
#define RPE8  0b1101
//Reserved    0b1110
//Reserved    0b1111
        
void GPIOInitialize( void );
void GPIOPPSInitialize ( volatile uint32_t ** ppsRegs, uint32_t* ppsValues, uint32_t size );
void GPIOInputEnablePort( GPIO_PORT port, uint32_t mask );
void GPIOOutputEnablePort( GPIO_PORT port, uint32_t mask );
uint32_t GPIOReadPort( GPIO_PORT port );
void GPIOWritePort( GPIO_PORT port, uint32_t mask, uint32_t value );
uint32_t GPIOLatchReadPort ( GPIO_PORT port );
void GPIOSetPort( GPIO_PORT port, uint32_t mask );
void GPIOClearPort( GPIO_PORT port, uint32_t mask );
void GPIOTogglePort( GPIO_PORT port, uint32_t mask );
    
#ifdef __cplusplus
}
#endif

#endif /* _GPIO_H */
