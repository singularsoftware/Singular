/****************************************************************************
  Clock System Source File
  Company : Singular Software
  File Name : clock.c
  Summary : functions to access PIC32MZ clock system.
  Description :  initialize and control PIC32MZ clock system.
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

#include "clock.h"
#include "system.h"

void CLOCKInitialize( uint32_t  pmd1, uint32_t  pmd2, uint32_t  pmd3, 
                      uint32_t  pmd4, uint32_t  pmd5, uint32_t  pmd6, uint32_t  pmd7 )
{
    SYSTEMUnlock();
    
    /* Set up Reference Clock 2 */
    /* Enable oscillator (ON bit) */
    REFO2CONSET = 0x00008000;
    
    /* PMD (Peripheral Module Disable) Configuration */
    PMD1 = pmd1; 
    PMD2 = pmd2; 
    PMD3 = pmd3; 
    PMD4 = pmd4; 
    PMD5 = pmd5; 
    PMD6 = pmd6; 
    PMD7 = pmd7; 

    SYSTEMLock();
}