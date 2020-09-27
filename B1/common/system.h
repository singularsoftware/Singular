/****************************************************************************
  System Source File
  Company : Singular Software
  File Name : system.h
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

#ifndef _SYSTEM_H 
#define _SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif
 #include "sys/kmem.h"
    
#define SYSTEM_FREQUENCY    100000000
    
#define SYSTEM_BOOTLOADER_TRIGGER_CODE (0x5048434DUL)
#define SYSTEM_RAM_START   KVA0_TO_KVA1(0x80000000)

void SYSTEMUnlock( void );
void SYSTEMLock( void );
void SYSTEMControlRegisterUnlock( void );
void SYSTEMControlRegisterLock( void );

#ifdef __cplusplus
}
#endif

#endif /* _SYSTEM_H */
