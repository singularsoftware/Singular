/****************************************************************************
  System Source File
  Company : Singular Software
  File Name : system.c
  Summary : system PIC32MZ functions.
  Description :  PIC32MZ system functions.
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
#include <stdbool.h>

#include "system.h"

uint32_t *systemRamStartAddress = (uint32_t *)SYSTEM_RAM_START;

void SYSTEMUnlock( void )
{
    bool interruptsEnabled = (bool)__builtin_disable_interrupts();

    /* Execute the system unlock sequence by writing the Key values of 0xAA996655 and
       0x556699AA to the SYSKEY register in two back-to-back Assembly or ?C? instructions. */
    SYSKEY = 0x00000000;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    if (interruptsEnabled)
    {
        __builtin_mtc0(12, 0,(__builtin_mfc0(12, 0) | 0x0001)); /* enable interrupts */
    }
}

void SYSTEMLock( void )
{
     bool interruptsEnabled = (bool)__builtin_disable_interrupts();

    /* Write a non-key value (such as, 0x33333333) to the SYSKEY register to perform a lock. */
    SYSKEY = 0x33333333;

    if (interruptsEnabled) 
    {
        __builtin_mtc0(12, 0,(__builtin_mfc0(12, 0) | 0x0001));
    }
}

void SYSTEMControlRegisterUnlock( void )
{
    /* Clearing IOLOCK allows writes */
    CFGCONbits.IOLOCK = 0;
}
void SYSTEMControlRegisterLock( void )
{
    /* Setting IOLOCK prevents writes to the control registers */
    CFGCONbits.IOLOCK = 1;
}

void SystemReset( void )
{
    systemRamStartAddress[0] = SYSTEM_BOOTLOADER_TRIGGER_CODE;
    systemRamStartAddress[1] = SYSTEM_BOOTLOADER_TRIGGER_CODE;
    systemRamStartAddress[2] = SYSTEM_BOOTLOADER_TRIGGER_CODE;
    systemRamStartAddress[3] = SYSTEM_BOOTLOADER_TRIGGER_CODE;
            
    SYSTEMUnlock();
    
    RSWRSTSET = _RSWRST_SWRST_MASK;
    (void)RSWRST;
}