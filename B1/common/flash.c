/****************************************************************************
  Flash Programming System Source File
  Company : Singular Software
  File Name : flash.c
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

#include <xc.h>
#include <string.h>
#include <sys/kmem.h>

#include "flash.h"

typedef enum
{
    /* 0000 = No operation */
    NO_OPERATION                                = 0b0000,
    /* 0001 = Word program operation: programs word selected by NVMADDR, if it is not write-protected(1) */
    WORD_PROGRAM_OPERATION                      = 0b0001,
    /* 0010 = Quad Word (128-bit) program operation: programs the 128-bit Flash Word selected by NVMADDR, if it is not write-protected */
    QUAD_WORD_128_PROGRAM_OPERATION             = 0b0010,
    /* 0011 = Row program operation: programs row selected by NVMADDR, if it is not write-protected */
    ROW_PROGRAM_OPERATION                       = 0b0011,
    /* 0100 = Page erase operation: erases page selected by NVMADDR, if it is not write-protected */
    PAGE_ERASE_OPERATION                        = 0b0100,
    /* 0101 = Lower program Flash memory erase operation: erases only the lower mapped region of program Flash (all pages in that region must be unprotected) */
    LOWER_PROGRAM_FLASH_MEMORY_ERASE_OPERATION  = 0b0101,
    /* 0110 = Upper program Flash memory erase operation: erases only the upper mapped region of program Flash (all pages in that region must be unprotected) */
    UPPER_PROGRAM_FLASH_MEMORY_ERASE_OPERATION  = 0b0110,
    /* 0111 = Program erase operation: erase all of program Flash memory (all pages must be unprotected, PWP<23:0> = 0x000000) */
    PROGRAM_ERASE_OPERATION                     = 0b0111,
} MODE_OF_OPERATION;



//#define NVM_INTERRUPT_ENABLE_MASK   0x80
//#define NVM_INTERRUPT_FLAG_MASK     0x80

static void FlashUnlock( void )
{
    NVMKEY = 0x0;
    NVMKEY = 0xAA996655;
    NVMKEY = 0x556699AA;
}

static void FlashStartOperationAtAddress( uint32_t address,  MODE_OF_OPERATION modeOfOperation )
{
    bool interruptsEnabled = (bool)__builtin_disable_interrupts();

    /* NVMADDR Flash Address Register. This register is used to store the physical */
    NVMADDR = KVA_TO_PA(address);

    /* NVMCON: Programming Control Register 
       WREN: Write Enable bit
        1 = Enable writes to the WR bit and disables writes to the SWAP or PFSWAP bit and the BFSWAP and NVMOP<3:0> bits
        0 = Disable writes to WR bit and enables writes to the SWAP or PFSWAP bit and the BFSWAP and NVMOP<3:0> bits */
    /* CLEAR */
    NVMCONCLR = _NVMCON_WREN_MASK;

    /* NVMOP<3:0>: NVM Operation bits
        These bits are only writable when WREN = 0.
        1111-1000 = Reserved
        0111 = Program erase operation: erase all of program Flash memory (all pages must be unprotected, PWP<23:0> = 0x000000)
        0110 = Upper program Flash memory erase operation: erases only the upper mapped region of program Flash (all pages in that region must be unprotected)
        0101 = Lower program Flash memory erase operation: erases only the lower mapped region of program Flash (all pages in that region must be unprotected)
        0100 = Page erase operation: erases page selected by NVMADDR, if it is not write-protected
        0011 = Row program operation: programs row selected by NVMADDR, if it is not write-protected
        0010 = Quad Word (128-bit) program operation: programs the 128-bit Flash Word selected by NVMADDR,if it is not write-protected
        0001 = Word program operation: programs word selected by NVMADDR, if it is not write-protected(1)
        0000 = No operation*/
    /* CLEAR */
    NVMCONCLR = _NVMCON_NVMOP_MASK;
    /* SET */
    NVMCONSET = ( _NVMCON_NVMOP_MASK & (((uint32_t)modeOfOperation) << _NVMCON_NVMOP_POSITION) );

    /* NVMCON: Programming Control Register 
       WREN: Write Enable bit
        1 = Enable writes to the WR bit and disables writes to the SWAP or PFSWAP bit and the BFSWAP and NVMOP<3:0> bits
        0 = Disable writes to WR bit and enables writes to the SWAP or PFSWAP bit and the BFSWAP and NVMOP<3:0> bits */
    /* SET */
    NVMCONSET = _NVMCON_WREN_MASK;

    FlashUnlock();

    // Start the operation
    /* WR: Write Control bit(2) This bit cannot be cleared and can be set only when WREN = 1 and the unlock sequence has been performed.
        1 = Initiate a Flash operation
        0 = Flash operation is complete or inactive */
    /* SET */
    NVMCONSET = _NVMCON_WR_MASK;

    if (interruptsEnabled)
    {
        __builtin_mtc0(12, 0,(__builtin_mfc0(12, 0) | 0x0001)); /* enable interrupts */
    }

}

void FlashProgrammingInitialize( void )
{
    FlashStartOperationAtAddress( NVMADDR,  NO_OPERATION );
}

bool FlashOperationInProgress( void )
{
    return (bool)NVMCONbits.WR;
}

void FlashRowProgramOperation( uint32_t *data, uint32_t address )
{
   NVMSRCADDR = (uint32_t )KVA_TO_PA(data);

   FlashStartOperationAtAddress( address, ROW_PROGRAM_OPERATION);
}

void FlashPageEraseOperation( uint32_t address )
{
   FlashStartOperationAtAddress(address,  PAGE_ERASE_OPERATION);
}
