/****************************************************************************
  GPIO System Source File
  Company : Singular Software
  File Name : spi.c
  Summary : SPI configure and control functions.
  Description :  initialize and control PIC32MZ spi system.
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
#include <sys/attribs.h>
#include "spi.h"

/* SPI constants */
#define SPI_CON_MSTEN                   (1 << _SPI1CON_MSTEN_POSITION)
#define SPI_CON_CKP                     (0 << _SPI1CON_CKP_POSITION)
#define SPI_CON_CKE                     (1 << _SPI1CON_CKE_POSITION)
#define SPI_CON_MODE_32_MODE_16         (0 << _SPI1CON_MODE16_POSITION)
#define SPI_CON_ENHBUF                  (1 << _SPI1CON_ENHBUF_POSITION)
#define SPI_CON_MCLKSEL                 (0 << _SPI1CON_MCLKSEL_POSITION)
#define SPI_CON_MSSEN                   (0 << _SPI1CON_MSSEN_POSITION)
#define SPI_CON_SMP                     (0 << _SPI1CON_SMP_POSITION)

#define SPI_CON_ON_MASK                 _SPI1CON_ON_MASK
#define SPI_STAT_SPIROV_MASK            _SPI1STAT_SPIROV_MASK

#define SPI_CON_MODE16_MASK             _SPI1CON_MODE16_MASK 
#define SPI_CON_MODE32_MASK             _SPI1CON_MODE32_MASK
#define SPI_CON_CKP_MASK                _SPI1CON_CKP_MASK 
#define SPI_CON_CKE_MASK                _SPI1CON_CKE_MASK

#define SPI_STAT_SPIRBE_MASK            _SPI1STAT_SPIRBE_MASK
#define SPI_CON_SRXISEL_MASK            _SPI1CON_SRXISEL_MASK
#define SPI_STAT_SRMT_MASK              _SPI1STAT_SRMT_MASK
#define SPI_CON_STXISEL_MASK            _SPI1CON_STXISEL_MASK
#define SPI_STAT_SPITBE_MASK            _SPI1STAT_SPITBE_MASK

uint32_t SPI_FaultInterruptEnableBitLocation[6]    = {13,14,26,3,16,25};
uint32_t SPI_ReceiveInterruptEnableBitLocation[6]  = {14,15,27,4,17,26};
uint32_t SPI_TransferInterruptEnableBitLocation[6] = {15,16,28,5,18,27};

uint32_t SPI_FaultInterruptFlagBitLocation[6]    = {13,14,26,3,16,25};
uint32_t SPI_ReceiveInterruptFlagBitLocation[6]  = {14,15,27,4,17,26};
uint32_t SPI_TransferInterruptFlagBitLocation[6] = {15,16,28,5,18,27};

uint32_t SPI_FaultInterruptPriorityBitLocation[6]    = {10,18,18,26,2,10};
uint32_t SPI_ReceiveInterruptPriorityBitLocation[6]  = {18,26,26,2,10,18};
uint32_t SPI_TransferInterruptPriorityBitLocation[6] = {26,2,2,10,18,26};

uint32_t SPI_FaultInterruptSubPriorityBitLocation[6]    = {8,16,16,24,0,8};
uint32_t SPI_ReceiveInterruptSubPriorityBitLocation[6]  = {16,24,24,0,8,16};
uint32_t SPI_TransferInterruptSubPriorityBitLocation[6] = {24,0,0,8,16,24}; 

typedef struct
{
    volatile uint32_t * SPICON;
    volatile uint32_t * SPIBUF;
    volatile uint32_t * SPIBRG;
    volatile uint32_t * SPISTAT;
    volatile uint32_t * SPISTATCLR;
    volatile uint32_t * SPICON2SET;
    volatile uint32_t * SPICONSET;
    volatile uint32_t * SPICONCLR;
    
    volatile uint32_t * IECCLR;
    volatile uint32_t * IECSET;
    volatile uint32_t * IFSCLR;
    volatile uint32_t * IPCSETL;
    volatile uint32_t * IPCSETH;
} SPI_SFRS_PTRS;


static SPI_SFRS_PTRS Spi[6];
static SPI_DATA* SpiData[6];

bool static SetSfrs( SPI_NUMBER spiNumber, SPI_SFRS_PTRS * sfrsPtrs)
{
    bool result = true;
    
    if (sfrsPtrs != NULL)
    {
        switch(spiNumber)
        {
            case SPI6: 
                sfrsPtrs->SPICON     = &SPI6CON;
                sfrsPtrs->SPIBUF     = &SPI6BUF;
                sfrsPtrs->SPIBRG     = &SPI6BRG;
                sfrsPtrs->SPISTAT    = &SPI6STAT;
                sfrsPtrs->SPISTATCLR = &SPI6STATCLR;
                sfrsPtrs->SPICON2SET = &SPI6CON2SET;
                sfrsPtrs->SPICONSET  = &SPI6CONSET; 
                sfrsPtrs->SPICONCLR  = &SPI6CONCLR;

                sfrsPtrs->IECCLR     = &IEC5CLR;
                sfrsPtrs->IECSET     = &IEC5SET;
                sfrsPtrs->IFSCLR     = &IFS5CLR;
                sfrsPtrs->IPCSETL    = &IPC46SET;
                sfrsPtrs->IPCSETH    = &IPC46SET;
                break;
            case SPI5: 
                sfrsPtrs->SPICON     = &SPI5CON;
                sfrsPtrs->SPIBUF     = &SPI5BUF;
                sfrsPtrs->SPIBRG     = &SPI5BRG;
                sfrsPtrs->SPISTAT    = &SPI5STAT;
                sfrsPtrs->SPISTATCLR = &SPI5STATCLR;
                sfrsPtrs->SPICON2SET = &SPI5CON2SET;
                sfrsPtrs->SPICONSET  = &SPI5CONSET; 
                sfrsPtrs->SPICONCLR  = &SPI5CONCLR;

                sfrsPtrs->IECCLR     = &IEC5CLR;
                sfrsPtrs->IECSET     = &IEC5SET;  
                sfrsPtrs->IFSCLR     = &IFS5CLR;
                sfrsPtrs->IPCSETL    = &IPC44SET;
                sfrsPtrs->IPCSETH    = &IPC44SET;
                break;
            case SPI4: 
                sfrsPtrs->SPICON     = &SPI4CON;
                sfrsPtrs->SPIBUF     = &SPI4BUF;
                sfrsPtrs->SPIBRG     = &SPI4BRG;
                sfrsPtrs->SPISTAT    = &SPI4STAT;
                sfrsPtrs->SPISTATCLR = &SPI4STATCLR;
                sfrsPtrs->SPICON2SET = &SPI4CON2SET;
                sfrsPtrs->SPICONSET  = &SPI4CONSET; 
                sfrsPtrs->SPICONCLR  = &SPI4CONCLR;

                sfrsPtrs->IECCLR     = &IEC5CLR;
                sfrsPtrs->IECSET     = &IEC5SET;
                sfrsPtrs->IFSCLR     = &IFS5CLR;
                sfrsPtrs->IPCSETL    = &IPC40SET;
                sfrsPtrs->IPCSETH    = &IPC41SET;
                break;
            case SPI3: 
                sfrsPtrs->SPICON     = &SPI3CON;
                sfrsPtrs->SPIBUF     = &SPI3BUF;
                sfrsPtrs->SPIBRG     = &SPI3BRG;
                sfrsPtrs->SPISTAT    = &SPI3STAT;
                sfrsPtrs->SPISTATCLR = &SPI3STATCLR;
                sfrsPtrs->SPICON2SET = &SPI3CON2SET;
                sfrsPtrs->SPICONSET  = &SPI3CONSET; 
                sfrsPtrs->SPICONCLR  = &SPI3CONCLR;

                sfrsPtrs->IECCLR     = &IEC4CLR;
                sfrsPtrs->IECSET     = &IEC4SET;
                sfrsPtrs->IFSCLR     = &IFS4CLR;
                sfrsPtrs->IPCSETL    = &IPC38SET;
                sfrsPtrs->IPCSETH    = &IPC39SET;
                break;
            case SPI2: 
                sfrsPtrs->SPICON     = &SPI2CON;
                sfrsPtrs->SPIBUF     = &SPI2BUF;
                sfrsPtrs->SPIBRG     = &SPI2BRG;
                sfrsPtrs->SPISTAT    = &SPI2STAT;
                sfrsPtrs->SPISTATCLR = &SPI2STATCLR;
                sfrsPtrs->SPICON2SET = &SPI2CON2SET;
                sfrsPtrs->SPICONSET  = &SPI2CONSET; 
                sfrsPtrs->SPICONCLR  = &SPI2CONCLR;

                sfrsPtrs->IECCLR     = &IEC4CLR;
                sfrsPtrs->IECSET     = &IEC4SET;
                sfrsPtrs->IFSCLR     = &IFS4CLR;
                sfrsPtrs->IPCSETL    = &IPC35SET;
                sfrsPtrs->IPCSETH    = &IPC36SET;
                break;
            case SPI1: 
                sfrsPtrs->SPICON     = &SPI1CON;
                sfrsPtrs->SPIBUF     = &SPI1BUF;
                sfrsPtrs->SPIBRG     = &SPI1BRG;
                sfrsPtrs->SPISTAT    = &SPI1STAT;
                sfrsPtrs->SPISTATCLR = &SPI1STATCLR;
                sfrsPtrs->SPICON2SET = &SPI1CON2SET;
                sfrsPtrs->SPICONSET  = &SPI1CONSET; 
                sfrsPtrs->SPICONCLR  = &SPI1CONCLR;

                sfrsPtrs->IECCLR     = &IEC3CLR;
                sfrsPtrs->IECSET     = &IEC3SET;
                sfrsPtrs->IFSCLR     = &IFS3CLR;
                sfrsPtrs->IPCSETL    = &IPC27SET;
                sfrsPtrs->IPCSETH    = &IPC27SET;
            break;
            default:
                result = false;
            break;
        }
    } 
    return result;
}
void SPI_Initialize( void )
{
    static int i = 6;
    /* one time init. */
    while (i > 0)
    {
        i--;
        SpiData[i] = NULL;
        SetSfrs(i, &Spi[i]);
    }
}

void SPISetPriority(SPI_NUMBER spiNumber,  uint32_t priority, uint32_t sub_priority)
{
    /* Set up priority / sub-priority of enabled interrupts */
    /* Three bits priority, two bits sub priority */    
    if (SPI_FaultInterruptPriorityBitLocation[spiNumber] > SPI_ReceiveInterruptPriorityBitLocation[spiNumber])
    {
        *Spi[spiNumber].IPCSETL = ((priority && 0x07) << SPI_FaultInterruptPriorityBitLocation[spiNumber]) |
                                  ((sub_priority && 0x03) << SPI_FaultInterruptSubPriorityBitLocation[spiNumber]);   
    }
    else
    {
        *Spi[spiNumber].IPCSETH = ((priority && 0x07) << SPI_FaultInterruptPriorityBitLocation[spiNumber]) |
                                  ((sub_priority && 0x03) << SPI_FaultInterruptSubPriorityBitLocation[spiNumber]);           
    }

    if (SPI_ReceiveInterruptPriorityBitLocation[spiNumber] > SPI_TransferInterruptPriorityBitLocation[spiNumber])
    {
        *Spi[spiNumber].IPCSETL = ((priority && 0x07) << SPI_ReceiveInterruptPriorityBitLocation[spiNumber]) | 
                                  ((sub_priority && 0x03) << SPI_ReceiveInterruptSubPriorityBitLocation[spiNumber]);       
    }
    else
    {
        *Spi[spiNumber].IPCSETH = ((priority && 0x07) << SPI_ReceiveInterruptPriorityBitLocation[spiNumber]) |
                                  ((sub_priority && 0x03) << SPI_ReceiveInterruptSubPriorityBitLocation[spiNumber]);
    }
    
    *Spi[spiNumber].IPCSETH = ((priority && 0x07) << SPI_TransferInterruptPriorityBitLocation[spiNumber]) |
                             ((sub_priority && 0x03) << SPI_TransferInterruptSubPriorityBitLocation[spiNumber]);
}

void SPIInitialize (SPI_NUMBER spiNumber)
{
    uint32_t rdata;

    SPI_Initialize();
    if (SpiData[spiNumber] == NULL)
    {
        SpiData[spiNumber] = calloc(1, sizeof(SPI_DATA));///new SPI_DATA;
    }
    
    SPISetPriority(spiNumber, 1,0);      
    
    /* Disable SPI Interrupts */
    *Spi[spiNumber].IECCLR = /*0x2000000;*/ 1 << SPI_FaultInterruptEnableBitLocation[spiNumber];
    *Spi[spiNumber].IECCLR = /*0x4000000;*/ 1 << SPI_ReceiveInterruptEnableBitLocation[spiNumber];
    *Spi[spiNumber].IECCLR = /*0x8000000;*/ 1 << SPI_TransferInterruptEnableBitLocation[spiNumber];

    /* STOP and Reset the SPI */
    *Spi[spiNumber].SPICON = 0;

    /* Clear the Receiver buffer */
    rdata = *Spi[spiNumber].SPIBUF;
    rdata = rdata;

    /* Clear SPI Interrupt flags */
    *Spi[spiNumber].IFSCLR = 1 << SPI_FaultInterruptFlagBitLocation[spiNumber];// 0x2000000;
    *Spi[spiNumber].IFSCLR = 1 <<  SPI_ReceiveInterruptFlagBitLocation[spiNumber];// 0x4000000;
    *Spi[spiNumber].IFSCLR = 1 << SPI_TransferInterruptFlagBitLocation[spiNumber];// 0x8000000;

    /* BAUD Rate register Setup */
    *Spi[spiNumber].SPIBRG = 49;

    /* CLear the Overflow */
    *Spi[spiNumber].SPISTATCLR = SPI_STAT_SPIROV_MASK;

    /* FRMERREN = 1 Frame Error overflow generates error events */
    *Spi[spiNumber].SPICON2SET = 0x2000;//0b10 0000 0000 0000; 
            
    /*
    MSTEN = 1
    CKP = 0
    CKE = 1
    MODE<32,16> = 0 When AUDEN = 0:0 0 8-bit
    ENHBUF = 1 1 = Enhanced Buffer mode is enabled
    MSSEN = 0 0 = Slave select SPI support is disabled.
    MCLKSEL = 0 0 = PBCLK2 is used by the Baud Rate Generator
    */
    *Spi[spiNumber].SPICONSET = (SPI_CON_MSSEN | SPI_CON_MCLKSEL | SPI_CON_ENHBUF | 
                                 SPI_CON_MODE_32_MODE_16 | SPI_CON_CKE | SPI_CON_CKP | 
                                 SPI_CON_MSTEN | SPI_CON_SMP);

    /* Enable transmit interrupt when transmit buffer is completely empty (STXISEL = '01') */
    /* Enable receive interrupt when the receive buffer is not empty (SRXISEL = '01') */
    *Spi[spiNumber].SPICONSET = 0x00000005;

    /* Initialize global variables */
    SpiData[spiNumber]->transferIsBusy = false;
    SpiData[spiNumber]->callback = NULL;

    /* Enable SPI */
    *Spi[spiNumber].SPICONSET = SPI_CON_ON_MASK;
}

bool SPITransferSetup (SPI_NUMBER spiNumber, SPI_TRANSFER_SETUP* setup, uint32_t spiSourceClock )
{
    uint32_t baudRegValue;
    uint32_t baudHigh;
    uint32_t baudLow;
    uint32_t errorHigh;
    uint32_t errorLow;

    if ((setup == NULL) || (setup->clockFrequency == 0))
    {
        return false;
    }

    if(spiSourceClock == 0)
    {
        
        spiSourceClock = 100000000;/* Master Clock Frequency */
    }

    baudRegValue = (((spiSourceClock / (setup->clockFrequency)) / 2u) - 1u);
    baudHigh = spiSourceClock / (2u * (baudRegValue + 1u));
    baudLow = spiSourceClock / (2u * (baudRegValue + 2u));
    errorHigh = baudHigh - setup->clockFrequency;
    errorLow = setup->clockFrequency - baudLow;

    if (errorHigh > errorLow)
    {
        baudRegValue++;
    }

    if(baudRegValue > 511)
    {
        return false;
    }

    *Spi[spiNumber].SPIBRG = baudRegValue;
    *Spi[spiNumber].SPICON = (*Spi[spiNumber].SPICON & 
                             (~(SPI_CON_MODE16_MASK | SPI_CON_MODE32_MASK | SPI_CON_CKP_MASK | SPI_CON_CKE_MASK))) |
                             (setup->clockPolarity | setup->clockPhase | setup->dataBits);

    return true;
}

bool SPIWrite(SPI_NUMBER spiNumber, void* pTransmitData, size_t txSize)
{
    return(SPIWriteRead(spiNumber, pTransmitData, txSize, NULL, 0));
}

bool SPIRead(SPI_NUMBER spiNumber, void* pReceiveData, size_t rxSize)
{
    return(SPIWriteRead(spiNumber, NULL, 0, pReceiveData, rxSize));
}

bool SPIWriteRead (SPI_NUMBER spiNumber, void* pTransmitData, size_t txSize, void* pReceiveData, size_t rxSize)
{
    bool isRequestAccepted = false;
    uint32_t dummyData;

    /* Verify the request */
    if((((txSize > 0) && (pTransmitData != NULL)) || 
        ((rxSize > 0) && (pReceiveData != NULL))) && 
        (SpiData[spiNumber]->transferIsBusy == false))
    {
        isRequestAccepted = true;
        SpiData[spiNumber]->txBuffer = pTransmitData;
        SpiData[spiNumber]->rxBuffer = pReceiveData;
        SpiData[spiNumber]->rxCount = 0;
        SpiData[spiNumber]->txCount = 0;
        SpiData[spiNumber]->dummySize = 0;

        if (pTransmitData != NULL)
        {
            SpiData[spiNumber]->txSize = txSize;
        }
        else
        {
            SpiData[spiNumber]->txSize = 0;
        }

        if (pReceiveData != NULL)
        {
            SpiData[spiNumber]->rxSize = rxSize;
        }
        else
        {
            SpiData[spiNumber]->rxSize = 0;
        }

        SpiData[spiNumber]->transferIsBusy = true;

        if (SpiData[spiNumber]->rxSize > SpiData[spiNumber]->txSize)
        {
            SpiData[spiNumber]->dummySize = SpiData[spiNumber]->rxSize - SpiData[spiNumber]->txSize;
        }

        /* Clear the receive overflow error if any */
        *Spi[spiNumber].SPISTATCLR = SPI_STAT_SPIROV_MASK;

        /* Make sure there is no data pending in the RX FIFO */
        /* Depending on 8/16/32 bit mode, there may be 16/8/4 bytes in the FIFO */
        while ((bool)(*Spi[spiNumber].SPISTAT & SPI_STAT_SPIRBE_MASK) == false)
        {
            dummyData = *Spi[spiNumber].SPIBUF;
            (void)dummyData;
        }

        /* Configure SPI to generate receive interrupt when receive buffer is empty (SRXISEL = '01') */
        *Spi[spiNumber].SPICONCLR = SPI_CON_SRXISEL_MASK;
        *Spi[spiNumber].SPICONSET = 0x00000001;

        /* Configure SPI to generate transmit interrupt when the transmit shift register is empty (STXISEL = '00')*/
        *Spi[spiNumber].SPICONCLR = SPI_CON_STXISEL_MASK;

        /* Disable the receive interrupt */
        *Spi[spiNumber].IECCLR = 1 << SPI_ReceiveInterruptEnableBitLocation[spiNumber];//0x4000000;

        /* Disable the transmit interrupt */
        *Spi[spiNumber].IECCLR = 1 << SPI_TransferInterruptEnableBitLocation[spiNumber];//0x8000000;

        /* Clear the receive interrupt flag */
        *Spi[spiNumber].IFSCLR = 1 <<  SPI_ReceiveInterruptFlagBitLocation[spiNumber];//0x4000000;

        /* Clear the transmit interrupt flag */
        *Spi[spiNumber].IFSCLR = 1 << SPI_TransferInterruptFlagBitLocation[spiNumber];//0x8000000;

        /* Start the first write here itself, rest will happen in ISR context */
        if((SPI_CON_MODE32_MASK) == (*Spi[spiNumber].SPICON & (SPI_CON_MODE32_MASK)))
        {
            SpiData[spiNumber]->txSize >>= 2;
            SpiData[spiNumber]->dummySize >>= 2;
            SpiData[spiNumber]->rxSize >>= 2;

            if(SpiData[spiNumber]->txCount < SpiData[spiNumber]->txSize)
            {
                *Spi[spiNumber].SPIBUF = *((uint32_t*)SpiData[spiNumber]->txBuffer);
                SpiData[spiNumber]->txCount++;
            }
            else if (SpiData[spiNumber]->dummySize > 0)
            {
                *Spi[spiNumber].SPIBUF = (uint32_t)(0xff);
                SpiData[spiNumber]->dummySize--;
            }
        }
        else if((SPI_CON_MODE16_MASK) == (*Spi[spiNumber].SPICON & (SPI_CON_MODE16_MASK)))
        {
            SpiData[spiNumber]->txSize >>= 1;
            SpiData[spiNumber]->dummySize >>= 1;
            SpiData[spiNumber]->rxSize >>= 1;

            if (SpiData[spiNumber]->txCount < SpiData[spiNumber]->txSize)
            {
                *Spi[spiNumber].SPIBUF = *((uint16_t*)SpiData[spiNumber]->txBuffer);
                SpiData[spiNumber]->txCount++;
            }
            else if (SpiData[spiNumber]->dummySize > 0)
            {
                *Spi[spiNumber].SPIBUF = (uint16_t)(0xff);
                SpiData[spiNumber]->dummySize--;
            }
        }
        else
        {
            if (SpiData[spiNumber]->txCount < SpiData[spiNumber]->txSize)
            {
                *Spi[spiNumber].SPIBUF = *((uint8_t*)SpiData[spiNumber]->txBuffer);
                SpiData[spiNumber]->txCount++;
            }
            else if (SpiData[spiNumber]->dummySize > 0)
            {
                *Spi[spiNumber].SPIBUF = (uint8_t)(0xff);
                SpiData[spiNumber]->dummySize--;
            }
        }

        if (rxSize > 0)
        {
            /* Enable receive interrupt to complete the transfer in ISR context.
             * Keep the transmit interrupt disabled. Transmit interrupt will be
             * enabled later if txCount < txSize, when rxCount = rxSize.
             */
            *Spi[spiNumber].IECSET = 1 << SPI_ReceiveInterruptEnableBitLocation[spiNumber];//0x4000000;
        }
        else
        {
            if (SpiData[spiNumber]->txCount != SpiData[spiNumber]->txSize)
            {
                /* Configure SPI to generate transmit buffer empty interrupt only if more than
                 * data is pending (STXISEL = '01')  */
                *Spi[spiNumber].SPICONSET = 0x00000004;
            }
            /* Enable transmit interrupt to complete the transfer in ISR context */
            *Spi[spiNumber].IECSET = 1 << SPI_TransferInterruptEnableBitLocation[spiNumber];//0x8000000;
        }
    }

    return isRequestAccepted;
}

bool SPIIsBusy (SPI_NUMBER spiNumber)
{
    return ( (SpiData[spiNumber]->transferIsBusy) || ((*Spi[spiNumber].SPISTAT & SPI_STAT_SRMT_MASK) == 0));
}

void SPICallbackRegister (SPI_NUMBER spiNumber, SPI_CALLBACK callback, uintptr_t context)
{
    SpiData[spiNumber]->callback = callback;

    SpiData[spiNumber]->context = context;
}

void SPIRXInterruptHandler (SPI_NUMBER spiNumber)
{
    uint32_t receivedData = 0;

    /* Check if the receive buffer is empty or not */
    if ((bool)(*Spi[spiNumber].SPISTAT & SPI_STAT_SPIRBE_MASK) == false)
    {
        /* Receive buffer is not empty. Read the received data. */
        receivedData = *Spi[spiNumber].SPIBUF;

        if (SpiData[spiNumber]->rxCount < SpiData[spiNumber]->rxSize)
        {
            /* Copy the received data to the user buffer */
            if((SPI_CON_MODE32_MASK) == (*Spi[spiNumber].SPICON & (SPI_CON_MODE32_MASK)))
            {
                ((uint32_t*)SpiData[spiNumber]->rxBuffer)[SpiData[spiNumber]->rxCount++] = receivedData;
            }
            else if((SPI_CON_MODE16_MASK) == (*Spi[spiNumber].SPICON & (SPI_CON_MODE16_MASK)))
            {
                ((uint16_t*)SpiData[spiNumber]->rxBuffer)[SpiData[spiNumber]->rxCount++] = receivedData;
            }
            else
            {
                ((uint8_t*)SpiData[spiNumber]->rxBuffer)[SpiData[spiNumber]->rxCount++] = receivedData;
            }
            if ((SpiData[spiNumber]->rxCount == SpiData[spiNumber]->rxSize) && (SpiData[spiNumber]->txCount < SpiData[spiNumber]->txSize))
            {
                /* Reception of all bytes is complete. However, there are few more
                 * bytes to be transmitted as txCount != txSize. Finish the
                 * transmission of the remaining bytes from the transmit interrupt. */

                /* Disable the receive interrupt */
                *Spi[spiNumber].IECCLR = 1 << SPI_ReceiveInterruptEnableBitLocation[spiNumber];//0x4000000;

                /* Generate TX interrupt when buffer is completely empty (STXISEL = '00') */
                *Spi[spiNumber].SPICONCLR = SPI_CON_STXISEL_MASK;
                *Spi[spiNumber].SPICONSET = 0x00000004;

                /* Enable the transmit interrupt. Callback will be given from the
                 * transmit interrupt, when all bytes are shifted out. */
                *Spi[spiNumber].IECSET = 1 << SPI_TransferInterruptEnableBitLocation[spiNumber];//0x8000000;
            }
        }
        if (SpiData[spiNumber]->rxCount < SpiData[spiNumber]->rxSize)
        {
            /* More bytes pending to be received .. */
            if((SPI_CON_MODE32_MASK) == (*Spi[spiNumber].SPICON & (SPI_CON_MODE32_MASK)))
            {
                if (SpiData[spiNumber]->txCount < SpiData[spiNumber]->txSize)
                {
                    *Spi[spiNumber].SPIBUF = ((uint32_t*)SpiData[spiNumber]->txBuffer)[SpiData[spiNumber]->txCount++];
                }
                else if (SpiData[spiNumber]->dummySize > 0)
                {
                    *Spi[spiNumber].SPIBUF = (uint32_t)(0xff);
                    SpiData[spiNumber]->dummySize--;
                }
            }
            else if((SPI_CON_MODE16_MASK) == (*Spi[spiNumber].SPICON & (SPI_CON_MODE16_MASK)))
            {
                if (SpiData[spiNumber]->txCount < SpiData[spiNumber]->txSize)
                {
                    *Spi[spiNumber].SPIBUF = ((uint16_t*)SpiData[spiNumber]->txBuffer)[SpiData[spiNumber]->txCount++];
                }
                else if (SpiData[spiNumber]->dummySize > 0)
                {
                    *Spi[spiNumber].SPIBUF = (uint16_t)(0xff);
                    SpiData[spiNumber]->dummySize--;
                }
            }
            else
            {
                if (SpiData[spiNumber]->txCount < SpiData[spiNumber]->txSize)
                {
                    *Spi[spiNumber].SPIBUF = ((uint8_t*)SpiData[spiNumber]->txBuffer)[SpiData[spiNumber]->txCount++];
                }
                else if (SpiData[spiNumber]->dummySize > 0)
                {
                    *Spi[spiNumber].SPIBUF = (uint8_t)(0xff);
                    SpiData[spiNumber]->dummySize--;
                }
            }
        }
        else
        {
            if((SpiData[spiNumber]->rxCount == SpiData[spiNumber]->rxSize) && (SpiData[spiNumber]->txCount == SpiData[spiNumber]->txSize))
            {
                /* Clear receiver overflow error if any */
                *Spi[spiNumber].SPISTATCLR = SPI_STAT_SPIROV_MASK;

                /* Disable receive interrupt */
                *Spi[spiNumber].IECCLR = 1 << SPI_ReceiveInterruptEnableBitLocation[spiNumber];//0x4000000;

                /* Transfer complete. Give a callback */
                SpiData[spiNumber]->transferIsBusy = false;

                if(SpiData[spiNumber]->callback != NULL)
                {
                    SpiData[spiNumber]->callback(SpiData[spiNumber]->context);
                }
            }
        }
    }

    /* Clear SPI RX Interrupt flag */
    /* This flag should cleared only after reading buffer */
    *Spi[spiNumber].IFSCLR = 1 <<  SPI_ReceiveInterruptFlagBitLocation[spiNumber];//0x4000000;
}

void SPITXInterruptHandler (SPI_NUMBER spiNumber)
{
    /* If there are more words to be transmitted, then transmit them here and keep track of the count */
    if((*Spi[spiNumber].SPISTAT & SPI_STAT_SPITBE_MASK) == SPI_STAT_SPITBE_MASK)
    {
        if (SpiData[spiNumber]->txCount < SpiData[spiNumber]->txSize)
        {
            if((SPI_CON_MODE32_MASK) == (*Spi[spiNumber].SPICON & (SPI_CON_MODE32_MASK)))
            {
                *Spi[spiNumber].SPIBUF = ((uint32_t*)SpiData[spiNumber]->txBuffer)[SpiData[spiNumber]->txCount++];
            }
            else if((SPI_CON_MODE16_MASK) == (*Spi[spiNumber].SPICON & (SPI_CON_MODE16_MASK)))
            {
                *Spi[spiNumber].SPIBUF = ((uint16_t*)SpiData[spiNumber]->txBuffer)[SpiData[spiNumber]->txCount++];
            }
            else
            {
                *Spi[spiNumber].SPIBUF = ((uint8_t*)SpiData[spiNumber]->txBuffer)[SpiData[spiNumber]->txCount++];
            }

            if (SpiData[spiNumber]->txCount == SpiData[spiNumber]->txSize)
            {
                /* All bytes are submitted to the SPI module. Now, enable transmit
                 * interrupt when the shift register is empty (STXISEL = '00')*/
                *Spi[spiNumber].SPICONCLR = SPI_CON_STXISEL_MASK;
            }
        }
        else if ((SpiData[spiNumber]->txCount == SpiData[spiNumber]->txSize) && (*Spi[spiNumber].SPISTAT & SPI_STAT_SRMT_MASK))
        {
            /* This part of code is executed when the shift register is empty. */

            /* Clear receiver overflow error if any */
            *Spi[spiNumber].SPISTATCLR = SPI_STAT_SPIROV_MASK;

            /* Disable transmit interrupt */
            *Spi[spiNumber].IECCLR = 1 << SPI_TransferInterruptEnableBitLocation[spiNumber];//0x8000000;

            /* Transfer complete. Give a callback */
            SpiData[spiNumber]->transferIsBusy = false;

            if(SpiData[spiNumber]->callback != NULL)
            {
                SpiData[spiNumber]->callback(SpiData[spiNumber]->context);
            }
        }
    }
    /* Clear the transmit interrupt flag */
    *Spi[spiNumber].IFSCLR = 1 << SPI_TransferInterruptFlagBitLocation[spiNumber];//0x8000000;
}

void SPIFalutInterruptHandler (SPI_NUMBER spiNumber)
{
    /* Clear the receive overflow error if any */
    *Spi[spiNumber].SPISTATCLR = SPI_STAT_SPIROV_MASK;
}



/* SPI1 interrupt handlers */
void __ISR(_SPI1_FAULT_VECTOR, ipl1AUTO) SPI1_FALUT_Handler (void)
{
    SPIFalutInterruptHandler(SPI1);
}
void __ISR(_SPI1_RX_VECTOR, ipl1AUTO) SPI1_RX_Handler (void)
{
    SPIRXInterruptHandler(SPI1);
}
void __ISR(_SPI1_TX_VECTOR, ipl1AUTO) SPI1_TX_Handler (void)
{
    SPITXInterruptHandler(SPI1);
}
/* SPI2 interrupt handlers */
void __ISR(_SPI2_FAULT_VECTOR, ipl1AUTO) SPI2_FALUT_Handler (void)
{
    SPIFalutInterruptHandler(SPI2);
}
void __ISR(_SPI2_RX_VECTOR, ipl1AUTO) SPI2_RX_Handler (void)
{
    SPIRXInterruptHandler(SPI2);
}
void __ISR(_SPI2_TX_VECTOR, ipl1AUTO) SPI2_TX_Handler (void)
{
    SPITXInterruptHandler(SPI2);
}
/* SPI3 interrupt handlers */
void __ISR(_SPI3_FAULT_VECTOR, ipl1AUTO) SPI3_FALUT_Handler (void)
{
    SPIFalutInterruptHandler(SPI3);
}
void __ISR(_SPI3_RX_VECTOR, ipl1AUTO) SPI3_RX_Handler (void)
{
    SPIRXInterruptHandler(SPI3);
}
void __ISR(_SPI3_TX_VECTOR, ipl1AUTO) SPI3_TX_Handler (void)
{
    SPITXInterruptHandler(SPI3);
}
/* SPI4 interrupt handlers */
void __ISR(_SPI4_FAULT_VECTOR, ipl1AUTO) SPI4_FALUT_Handler (void)
{
    SPIFalutInterruptHandler(SPI4);
}
void __ISR(_SPI4_RX_VECTOR, ipl1AUTO) SPI4_RX_Handler (void)
{
    SPIRXInterruptHandler(SPI4);
}
void __ISR(_SPI4_TX_VECTOR, ipl1AUTO) SPI4_TX_Handler (void)
{
    SPITXInterruptHandler(SPI4);
}
/* SPI5 interrupt handlers */
void __ISR(_SPI5_FAULT_VECTOR, ipl1AUTO) SPI5_FALUT_Handler (void)
{
    SPIFalutInterruptHandler(SPI5);
}
void __ISR(_SPI5_RX_VECTOR, ipl1AUTO) SPI5_RX_Handler (void)
{
    SPIRXInterruptHandler(SPI5);
}
void __ISR(_SPI5_TX_VECTOR, ipl1AUTO) SPI5_TX_Handler (void)
{
    SPITXInterruptHandler(SPI5);
}
/* SPI6 interrupt handlers */
void __ISR(_SPI6_FAULT_VECTOR, ipl1AUTO) SPI6_FALUT_Handler (void)
{
    SPIFalutInterruptHandler(SPI6);
}
void __ISR(_SPI6_RX_VECTOR, ipl1AUTO) SPI6_RX_Handler (void)
{
    SPIRXInterruptHandler(SPI6);
}
void __ISR(_SPI6_TX_VECTOR, ipl1AUTO) SPI6_TX_Handler (void)
{
    SPITXInterruptHandler(SPI6);
}


