/****************************************************************************
  Flash Programming System Source File
  Company : Singular Software
  File Name : flash.h
  Summary : erase and programming functions.
  Description :  initialize and control PIC32MZ flash system.
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

#ifndef _FLASH_H 
#define _FLASH_H

#include <stdbool.h> 

#ifdef __cplusplus
extern "C" {
#endif

#define FLASH_START_ADDRESS      (0x9d000000UL)
#define FLASH_LENGTH             (0x200000UL)
#define FLASH_PAGE_SIZE          (2048UL)
#define FLASH_ERASE_BLOCK_SIZE   (16384UL)
#define PAGES_IN_ERASE_BLOCK     (FLASH_ERASE_BLOCK_SIZE / FLASH_PAGE_SIZE)
#define OFFSET_ALIGN_MASK        (~FLASH_ERASE_BLOCK_SIZE + 1)
#define SIZE_ALIGN_MASK          (~FLASH_PAGE_SIZE + 1)

void FlashProgrammingInitialize( void );
bool FlashOperationInProgress( void );
void FlashRowProgramOperation( uint32_t *data, uint32_t address );
void FlashPageEraseOperation( uint32_t address );

#ifdef __cplusplus
}
#endif

#endif /* _FLASH_H */

