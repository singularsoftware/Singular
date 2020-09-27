/****************************************************************************
  Initialize Source File
  Company : Singular Software
  File Name : initialize.c
  Summary : configure and control functions.
  Description :  main initialization of PIC32MZ.
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


#include "initialize.h"


/*** DEVCFG0 ***/
#pragma config DEBUG =      OFF
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx2
#pragma config TRCEN =      OFF
#pragma config BOOTISA =    MIPS32
#pragma config FECCCON =    OFF_UNLOCKED
#pragma config FSLEEP =     OFF
#pragma config DBGPER =     PG_ALL
#pragma config SMCLR =      MCLR_NORM
#pragma config SOSCGAIN =   GAIN_LEVEL_3
#pragma config SOSCBOOST =  ON
#pragma config POSCGAIN =   GAIN_LEVEL_3
#pragma config POSCBOOST =  ON
#pragma config EJTAGBEN =   NORMAL
#pragma config CP =         OFF

/*** DEVCFG1 ***/
#pragma config FNOSC =      SPLL
#pragma config DMTINTV =    WIN_127_128
#pragma config FSOSCEN =    OFF
#pragma config IESO =       OFF
#pragma config POSCMOD =    OFF
#pragma config OSCIOFNC =   OFF
#pragma config FCKSM =      CSECME
#pragma config WDTPS =      PS1048576
#pragma config WDTSPGM =    STOP
#pragma config FWDTEN =     OFF
#pragma config WINDIS =     NORMAL
#pragma config FWDTWINSZ =  WINSZ_25
#pragma config DMTCNT =     DMT31
#pragma config FDMTEN =     OFF

/*** DEVCFG2 ***/
#pragma config FPLLIDIV =   DIV_1
#pragma config FPLLRNG =    RANGE_5_10_MHZ
#pragma config FPLLICLK =   PLL_FRC
#pragma config FPLLMULT =   MUL_50
#pragma config FPLLODIV =   DIV_2
#pragma config UPLLFSEL =   FREQ_24MHZ

/*** DEVCFG3 ***/
#pragma config USERID =     0xffff
#pragma config FMIIEN =     ON
#pragma config FETHIO =     ON
#pragma config PGL1WAY =    ON
#pragma config PMDL1WAY =   ON
#pragma config IOL1WAY =    ON
#pragma config FUSBIDIO =   ON

/*** BF1SEQ0 ***/

#pragma config TSEQ =       0xffff
#pragma config CSEQ =       0x0

extern void BootloaderRunApplication( void );

void Initialize ( void )
{
    uint32_t digitalMode;
    uint32_t pullUp;
    uint32_t pullDown;
    volatile uint32_t* ppsRegs[2];
    uint32_t ppsValues[2];
    uint32_t size = 2;
    
     /* PMD (Peripheral Module Disable) Configuration */
    uint32_t pmd1 = 0x1001; /* disbale - ADC ADCMD + Comparator Voltage Reference */
    uint32_t pmd2 = 0x3; /* disbale - Comparator 1 + Comparator 2 */
    uint32_t pmd3 = 0x1ff01ff; /* disbale - Input Capture 1 to Input Capture 9 + Output Compare 1 to Output Compare 9 */
    uint32_t pmd4 = 0x1ff; /* disbale - Timer1 to Timer9 */
    uint32_t pmd5 = 0x301f3f3d;/* disbale - UART1, UART3 to UART6 +SPI1 to SPI6 +I2C1 to I2C5 + CAN1 to CAN2 */
    uint32_t pmd6 = 0x10830001; /* disbale - RTCC PMP EBI SQI1 Ethernet */
    uint32_t pmd7 = 0x500000;  /* disbale - Random Number Generator Crypto */
    /* Left enabled - UART2 + USB + Reference Clock Output 1 to Reference Clock Output 4  + DMA */
    
    __builtin_disable_interrupts();
    
    /* PMD */
    CLOCKInitialize(pmd1,pmd2,pmd3,pmd4,pmd5,pmd6,pmd7);
    
    PRECONbits.PREFEN = 3; /* Enable predictive prefetch for any address */
    PRECONbits.PFMWS  = 2; /* Two Wait states */
    CFGCONbits.ECCCON = 3; /* ECC (Flash Error Correcting Code) and dynamic ECC are disabled */

    pullUp = 0x0;
    pullDown = 0x0;
    /* PORT B */
    digitalMode = 0x0004000;//B14
    GPIOPortInitialize(PORT_B, digitalMode, pullUp, pullDown);
    /* PORT G */
    digitalMode = 0x00000040;//G6
    GPIOPortInitialize(PORT_G, digitalMode, pullUp, pullDown);
    /* PORT H */
    digitalMode = 0x0000008;//H3 SW
    pullUp = 0x08;
    GPIOPortInitialize(PORT_H, digitalMode, pullUp, pullDown);
    
    /* PPS */  
    /* pin used for UART2 RX */
    ppsRegs[0] = &U2RXR;
    ppsValues[0] = RPG6;
    /* pin used for UART2 TX */
    ppsRegs[1] = &RPB14R;
    ppsValues[1] = 2;
    
    GPIOPPSInitialize(ppsRegs, ppsValues, size);
    
    /* Start Firmware*/
    BootloaderRunApplication();//if no trigger will not return

    /* Coprocessor 0 */
    CP0Initialize(0,true);
    /* UARTs */
	UARTInitialize(UART2);
    /* Flash */
    FlashProgrammingInitialize();
    /* Interrupt controller */
    IRQInitialize();
}
