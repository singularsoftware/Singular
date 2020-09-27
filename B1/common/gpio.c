/****************************************************************************
  GPIO System Source File
  Company : Singular Software
  File Name : gpio.c
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

#include <xc.h>

#include "gpio.h"
#include "system.h"

void GPIOPortInitialize ( GPIO_PORT port, uint32_t digitalMode, uint32_t pullUp, uint32_t pullDown)
{
    *(volatile uint32_t *)(&ANSELACLR + (port * 0x40)) = digitalMode;   
    
    *(volatile uint32_t *)(&CNPUASET + (port * 0x40)) = pullUp;
    *(volatile uint32_t *)(&CNPDASET + (port * 0x40)) = pullDown;
}

void GPIOPPSInitialize (volatile uint32_t ** ppsRegs, uint32_t* ppsValues, uint32_t size)
{
    int i;
    
    /* Unlock the system for PPS configuration */
    SYSTEMUnlock();
    SYSTEMControlRegisterUnlock();

    for(i=0; i < size; i++)
    {
        *(ppsRegs[i]) = ppsValues[i];
    }

    /* Lock back the system after PPS configuration */
    SYSTEMLock();
    SYSTEMControlRegisterLock();

}

void GPIOInputEnablePort(GPIO_PORT port, uint32_t mask)
{
    *(volatile uint32_t *)(&TRISASET + (port * 0x40)) = mask;
}

void GPIOOutputEnablePort(GPIO_PORT port, uint32_t mask)
{
    *(volatile uint32_t *)(&TRISACLR + (port * 0x40)) = mask;
}

uint32_t GPIOReadPort(GPIO_PORT port)
{
    return (*(volatile uint32_t *)(&PORTA + (port * 0x40)));
}

void GPIOWritePort(GPIO_PORT port, uint32_t mask, uint32_t value)
{
    *(volatile uint32_t *)(&LATA + (port * 0x40)) = (*(volatile uint32_t *)(&LATA + (port * 0x40)) & (~mask)) | (mask & value);
}

uint32_t GPIOLatchReadPort(GPIO_PORT port)
{
    return (*(volatile uint32_t *)(&LATA + (port * 0x40)));
}

void GPIOSetPort(GPIO_PORT port, uint32_t mask)
{
    *(volatile uint32_t *)(&LATASET + (port * 0x40)) = mask;
}

void GPIOClearPort(GPIO_PORT port, uint32_t mask)
{
    *(volatile uint32_t *)(&LATACLR + (port * 0x40)) = mask;
}

void GPIOPortToggle(GPIO_PORT port, uint32_t mask)
{
    *(volatile uint32_t *)(&LATAINV + (port * 0x40)) = mask;
}

