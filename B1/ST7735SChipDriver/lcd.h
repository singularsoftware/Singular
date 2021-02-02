/****************************************************************************
  Clock LCD Driver Source File
  Company : Singular Software
  File Name : lcd.h
  Summary : functions to access ST7735S Chip LCD.
  Description :  initialize and control LCD .
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

#ifndef _LCD_H
#define _LCD_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdbool.h>
    
#include "lcdfonts.h"
    
typedef struct
{
    uint16_t x;
    uint16_t y;
} LCDPoint;

typedef enum 
{
    WHITE   =   0xFFFF,
    BLACK   =   0x0000, 
    BLUE    =   0x001F,  
    GRED    =	0xFFE0,
    GBLUE   =	0x07FF,
    RED     =   0xF800,
    MAGENTA =   0xF81F,
    GREEN   =   0x07E0,
    CYAN    =   0x7FFF,
    YELLOW  =   0xFFE0,
    BROWN   =	0xBC40,
    GRAY    =   0x8430 
} LCDColor;

typedef enum
{
	LeftToRightUpToDown,
	LeftToRightDownToUp,
	RightToLeftUpToDown,
	RightToLeftDownToUp,
    UpToDownRightToLeft,
	UpToDownLeftToRight,
	DownToUpLeftToRight,
	DownToUpRightToLeft, 
}LCDDirection;  

typedef struct
{
    uint8_t width;
    uint8_t height;
    LCDDirection direction;
    uint16_t columns;
	uint16_t lines;
    LCDFont* font;
    LCDColor background;
    LCDColor textBackground;
    LCDColor textForeground;          
}LCD_DATA;

extern LCD_DATA LCDData;


void LCDInitialize();

void LCDClear(LCDColor color);
void LCDSetCursor (LCDPoint point);
void LCDSetPointlColor (LCDPoint point, LCDColor color );
void LCDSetArealColor (LCDPoint start, LCDPoint end, LCDColor color);

void LCDDrawPoint(LCDPoint point, uint16_t color);

void LCDDrawLine (LCDPoint start,LCDPoint end, LCDColor color);
void LCDDrawRectangle (LCDPoint start,LCDPoint end, LCDColor lineColor, LCDColor fillColor, bool fill);
void LCDDrawCircle (LCDPoint point, uint16_t radius, LCDColor lineColor, LCDColor fillColor, bool fill);
void LCDDrawChar(LCDPoint point, const char c);
void LCDDrawString (LCDPoint point, const char * string);
void LCDDrawNumber(LCDPoint point, int32_t nummber);

void LCDScrollSet(uint8_t startLine, uint8_t lenght);
void LCDScroll(uint8_t line);

#ifdef __cplusplus
}
#endif

#endif /* _LCD_H */
