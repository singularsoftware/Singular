/****************************************************************************
  Timers System Source File
  Company : Singular Software
  File Name : cp0.h
  Summary : Coprocessor 0 configure and control functions.
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

#ifndef _CP0_H 
#define _CP0_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void CP0Initialize( uint32_t cp0Compare, bool enableTimer );
void CP0SetTimerCompareValue ( uint32_t cp0Compare /* 100000 = 1ms */ );
void CP0EnableTimer( void );
void CP0DisableTimer( void );
void CP0DelayMs ( uint32_t ms ); 
void CP0DelayUs ( uint32_t us );

#ifdef __cplusplus
}
#endif

#endif /* _CP0_H */
