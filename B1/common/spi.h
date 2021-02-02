/****************************************************************************
  interrupt System Source File
  Company : Singular Software
  File Name : spi.h
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

#ifndef SPI_H
#define SPI_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    SPI_CLOCK_PHASE_TRAILING_EDGE   = 0x00000000,
    SPI_CLOCK_PHASE_LEADING_EDGE    = 0x00000100,
    SPI_CLOCK_PHASE_INVALID         = 0xFFFFFFFF
}SPI_CLOCK_PHASE;

typedef enum
{
    SPI_CLOCK_POLARITY_IDLE_LOW     = 0x00000000,
    SPI_CLOCK_POLARITY_IDLE_HIGH    = 0x00000040,
    SPI_CLOCK_POLARITY_INVALID      = 0xFFFFFFFF
}SPI_CLOCK_POLARITY;

typedef enum
{
    SPI_DATA_BITS_8         = 0x00000000,
    SPI_DATA_BITS_16        = 0x00000400,
    SPI_DATA_BITS_32        = 0x00000800,
    SPI_DATA_BITS_INVALID   = 0xFFFFFFFF
}SPI_DATA_BITS;

typedef struct
{
    uint32_t            clockFrequency;
    SPI_CLOCK_PHASE     clockPhase;
    SPI_CLOCK_POLARITY  clockPolarity;
    SPI_DATA_BITS       dataBits;
}SPI_TRANSFER_SETUP;

typedef  void (*SPI_CALLBACK) (uintptr_t context);

typedef enum
{
    SPI1,
    SPI2,
    SPI3,
    SPI4,
    SPI5,
    SPI6
}SPI_NUMBER;

typedef struct
{
    SPI_NUMBER              spi;
    void*                   txBuffer;
    void*                   rxBuffer;
    size_t                  txSize;
    size_t                  rxSize;
    size_t                  dummySize;
    size_t                  rxCount;
    size_t                  txCount;
    bool                    transferIsBusy;
    SPI_CALLBACK            callback;
    uintptr_t               context;
} SPI_DATA ;


void SPIInitialize (SPI_NUMBER spiNumber);
bool SPIWriteRead (SPI_NUMBER spiNumber, void* pTransmitData, size_t txSize, void* pReceiveData, size_t rxSize);
bool SPIWrite(SPI_NUMBER spiNumber, void* pTransmitData, size_t txSize);
bool SPIRead(SPI_NUMBER spiNumber, void* pReceiveData, size_t rxSize);
bool SPITransferSetup (SPI_NUMBER spiNumber, SPI_TRANSFER_SETUP *setup, uint32_t spiSourceClock);
bool SPIIsBusy(SPI_NUMBER spiNumber);
void SPICallbackRegister(SPI_NUMBER spiNumber,SPI_CALLBACK callback, uintptr_t context);


#ifdef __cplusplus
    }
#endif

#endif // SPI_H
