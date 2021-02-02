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
#include "../common/spi.h"
#include "../ST7735SChipDriver/lcd.h"


bool LCDWrite(void* data, uint32_t size)
{
    return SPIWrite(SPI6,data, size);
}

inline void SetLcdReset()
{
    /* LCD Reset pin E5 */
    LATESET = (1<<5);
}
inline void ClearLcdReset()
{
    /* LCD Reset pin E5 */
    LATECLR = (1<<5);
}
inline void SetLcdDataCommand()
{
    /* LCD Data Command pin C3 */
    LATCSET = (1<<3);
}
void ClearLcdDataCommand()
{
    /* LCD Data Command pin C3 */
    LATCCLR = (1<<3);
}
void SetLcdChipSelect()
{
    /* LCD Chip Select pin B10 */
    LATBSET = (1<<10);
}
inline void ClearLcdChipSelect()
{
    /* LCD Chip Select pin B10 */ 
    LATBCLR = (1<<10);
}
inline void SetLcdBackLight()
{
    /* LCD Back Light Control pin E4 */
    LATESET = (1<<4);
}
inline void ClearLcdBackLight()
{
    /* LCD Back Light Control pin E4 */
    LATECLR = (1<<4);
    
}




uint8_t triggerMsg[] = "\r\nTrigger bootloader? ";
uint8_t triggeringMsg[] = "\r\nTriggering bootloader now !!!";

uint8_t scrollStartMsg[] = "\r\nStart Scrolling LCD ";
uint8_t scrollStopMsg[] = "\r\nLCD Scrolling Stopped";

int main ( void )
{
    Initialize();

    uint32_t i=0;
    bool start = false;

    LCDInitialize();
    
    LCDData.textForeground = RED;
    LCDPoint p = {10,20};
    LCDDrawString(p," Singular Software ");

    bool doScroll = false;
    uint8_t SSA = 0;
    LCDScrollSet(0, 162);
    
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
            else if (b == 's')
            {
                doScroll = !doScroll;
                if (doScroll)
                {
                 for(i=0; i < sizeof(scrollStartMsg); i++)
                    {
                        UARTWriteByte(UART2, scrollStartMsg[i]);
                    }
                }
                else
                {
                    for(i=0; i < sizeof(scrollStopMsg); i++)
                    {
                        UARTWriteByte(UART2, scrollStopMsg[i]);
                    }
                }
            }
            else
            {
                start = false;
                UARTWriteByte(UART2, b); 
            }
        }
        
        if (doScroll)
        {
            CP0DelayMs(50);
            SSA += 1;
            if (SSA > 162)
            {
                SSA = 0;
            }
            LCDScroll(SSA++);
        }   
    }
    
    return ( EXIT_FAILURE );
}

