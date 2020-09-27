/****************************************************************************
  Main Source File
  Company : Singular Software
  File Name : main.c
  Summary : main function of the bootloader test app.
  Description :  main initialize the system and start main loop.
****************************************************************************/

/*******************************************************************************
* Copyright (C) 2020 Singular Software Limited..
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

uint8_t triggerMsg[] = "\r\nTrigger bootloader? ";
uint8_t triggeringMsg[] = "\r\nTriggering bootloader now !!!";

int main ( void )
{
    Initialize();

    uint32_t i=0;
    bool start = false;
    
    while(true)
    {
        if (UARTRxReady(UART2))
        {            
            uint8_t b = UARTReadByte(UART2);
            if (b == 0x0D)
            {
                start = true;
                for(i=0; i < sizeof(triggerMsg); i++)
                {
                    UARTWriteByte(UART2, triggerMsg[i]);
                }
            }
            else if (b == 'y' && start)
            {
                for(i=0; i < sizeof(triggeringMsg); i++)
                {
                    UARTWriteByte(UART2, triggeringMsg[i]);
                }
                CP0DelayMs(500);
                SystemReset();
            }
            else
            {
                start = false;
                UARTWriteByte(UART2, b); 
            }
        }
    }
    
    return ( EXIT_FAILURE );
}
