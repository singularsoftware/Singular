/****************************************************************************
  Timers System Source File
  Company : Singular Software
  File Name : cp0.c
  Summary : Coprocessor 0configure and control functions.
  Description :  initialize and control PIC32MZ Coprocessor 0.
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

#include "cp0.h"
#include "system.h"

static uint32_t cp0CompareValue = 100000;/* 1ms */

void CP0Initialize( uint32_t cp0Compare, bool enableTimer )
{
    /* Clear CP0 Count Register */
    _CP0_SET_COUNT(0);
    
    /* Ser CP0 Compare Register value*/
    if (cp0Compare > 0)
    {
        cp0CompareValue = cp0Compare;
    }
    _CP0_SET_COMPARE(cp0CompareValue);

    if (enableTimer)
    {
        /* Clear DC bit(Disable Count Register bit) on CP0 Cause Register */
        _CP0_SET_CAUSE(_CP0_GET_CAUSE() & (~_CP0_CAUSE_DC_MASK));
    }
}

void CP0SetTimerCompareValue ( uint32_t cp0Compare /* 100000 = 1ms */ )
{
    cp0CompareValue = cp0Compare;
    _CP0_SET_COMPARE(cp0CompareValue);
}

void CP0EnableTimer( void )
{
    /* Set DC bit(Disable Count Register bit) on CP0 Cause Register */
    _CP0_SET_CAUSE(_CP0_GET_CAUSE() | _CP0_CAUSE_DC_MASK);

    /* Clear Compare Timer Interrupt bit */
    IFS0CLR = 0x1;

    /* Clear CP0 Count Register */
    _CP0_SET_COUNT(0);
    
    /* Ser CP0 Compare Register value*/
    _CP0_SET_COMPARE(cp0CompareValue);

    /* Clear DC bit(Disable Count Register bit) on CP0 Cause Register */
    _CP0_SET_CAUSE(_CP0_GET_CAUSE() & (~_CP0_CAUSE_DC_MASK));
}

void CP0DisableTimer( void )
{
    /* Set DC bit(Disable Count Register bit) on CP0 Cause Register */
    _CP0_SET_CAUSE(_CP0_GET_CAUSE() | _CP0_CAUSE_DC_MASK);
}

void CP0DelayMs ( uint32_t ms )
{
    uint32_t end = (SYSTEM_FREQUENCY / 1000) * ms;
    uint32_t start = _CP0_GET_COUNT();
    
    while((_CP0_GET_COUNT() - start) < end);
}

void CP0DelayUs ( uint32_t us )
{
    uint32_t end = (SYSTEM_FREQUENCY / 1000000) * us;
    uint32_t start = _CP0_GET_COUNT();
    
    while((_CP0_GET_COUNT() - start) < end);
}
