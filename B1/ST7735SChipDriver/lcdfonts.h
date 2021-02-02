/****************************************************************************
  Clock LCD Driver Source File
  Company : Singular Software
  File Name : fonts.h
  Summary : LCD fonts.
  Description :  initialize LCD fonts.
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

#ifndef _FONTS_H
#define _FONTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct 
{    
    const uint16_t bytesWidth;
    const uint16_t bytesHeight;
    const uint16_t pixelWidth;
    const uint16_t pixelHeight;
    const uint8_t *characters;
}LCDFont;

extern LCDFont LCDFont8x10;


#ifdef __cplusplus
}
#endif

#endif /* _FONTS_H */

