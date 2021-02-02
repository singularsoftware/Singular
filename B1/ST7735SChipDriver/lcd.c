/****************************************************************************
  LCD Source File
  Company : Singular Software
  File Name : lcd.c
  Summary : functions to access LCD .
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

#include <xc.h>
#include "lcd.h"
#include <stdbool.h>
#include <stdio.h>

//System commands
#define NOP         0x00
#define SWRESET     0x01    //Software reset
#define RDDID       0x04    //Read display ID
#define RDDST       0x09    //Read display status
#define RDDPM       0x0A    //Read display power mode
#define RDDMADCTL   0x0B    //Read display MADCTL
#define RDDCOLMOD   0x0C    //Read displaypixel format
#define RDDIM       0x0D    //Read display image mode
#define RDDSM       0x0E    //Read displaysignal mode
#define RDDSDR      0x0F    //Read display self-diagnostic result
#define SLPIN       0x10    //Sleep in 
#define SLPOUT      0x11    //Sleep out
#define PTLON       0x12    //Partial display mode on
#define NORON       0x13    //Normal display mode on
#define INVOFF      0x20    //Display inversion off
#define INVON       0x21    //Display inversion on
#define GAMSET      0x26    //Gamma set
#define DISPOFF     0x28    //Display off
#define DISPON      0x29    //Display on
#define CASET       0x2A    //Column address set
#define RASET       0x2B    //Row address	
#define RAMWR       0x2C    //Memory write
#define RGBSET      0x2D    //Color setting for 4K,65K and 262K
#define RAMRD       0x2E    //Memory read
#define PTLAR       0x30    //Partial area
#define SCRLAR      0x33    //Scroll area set
#define ZEOFF       0x34    //Tearing effect line off
#define ZEON        0x35    //Tearing effect line on
#define MADCTL      0x36    //Memory data acceass control
#define VSCSAD      0x37    //Vertical scroll start address of RAM
#define IDMOFF      0x38    //Idel mode off
#define IDMON       0x39    //Idel mode on
#define COLMOD      0x3A    //Interface pixel format
#define RDID1       0xDA    //Read ID1 value
#define RDID2       0xDB    //Read ID2 value
#define RDID3       0xDC    //Read ID3 value
//Panel commands
#define FRMCTR1     0xB1    //Fram rate control (in normal mode/full colors)
#define FRMCTR2     0xB2    //Fram rate control (in idle mode/8-colors)
#define FRMCTR3     0xB3    //Fram rate control (in partial mode/full colors)
#define INVCTR      0xB4    //Display inversion control
#define PWCTR1      0xC0    //Power control 1
#define PWCTR2      0xC1    //Power control 2
#define PWCTR3      0xC2    //Power control 3 (in normal mode/full colors)
#define PWCTR4      0xC3    //Power control 4 (in idle mode/8-colors)
#define PWCTR5      0xC4    //Power control 5 in partial mode/full colors)
#define VMCTR1      0xC5    //VCOM control 1
#define VMOFCTR     0xC7    //VCOM offset control
#define WRID2       0xD1    //Write ID2 value
#define WRID3       0xD2    //Write ID3 value
#define NVFCTR1     0xD9    //NVM control command
#define NVFCTR2     0xDE    //NVM read command
#define NVFCTR3     0xDF    //NVM write command
#define GMCTRP1     0xE0    //Gamma ('+'polarity) correction characteristics setting
#define GMCTRN1     0xE1    //Gamma ('-'polarity) correction characteristics setting
#define GCV         0xFC    //Gate pump clock frequency variable


LCD_DATA LCDData =
{
    .width = 162,
    .height = 132,
    .direction = UpToDownLeftToRight,
    
    .font = &LCDFont8x10,
    .background = WHITE,
    .textBackground = WHITE,
    .textForeground = BLACK
};

extern void SetLcdReset();
extern void ClearLcdReset();
extern void SetLcdDataCommand();
extern void ClearLcdDataCommand();
extern void SetLcdChipSelect();
extern void ClearLcdChipSelect();
extern void SetLcdBackLight();
extern void ClearLcdBackLight();
extern bool LCDWrite(void* data, uint32_t dize);

static void LCDReset(void)
{
	SetLcdReset();
	CP0DelayMs(100);
	ClearLcdReset();
	CP0DelayMs(100);
	SetLcdReset();
	CP0DelayMs(100);
}

static void LCDCommandSelect(uint8_t reg)
{
	ClearLcdDataCommand();
	ClearLcdChipSelect();
	LCDWrite(&reg, 1);
	SetLcdChipSelect();
}

static void LCDWriteByte(uint8_t data)
{
	SetLcdDataCommand();
	ClearLcdChipSelect();
	LCDWrite(&data, 1);
	SetLcdChipSelect();
}

static void LCDWriteShort(uint16_t data, uint32_t length)
{
	uint32_t i;
    uint8_t dataH = 0;
    uint8_t dataL = 0;
    
	SetLcdDataCommand();
	ClearLcdChipSelect();
    
	for(i = 0; i < length; i++)
    {
        dataH = data >> 8;
        dataL = data & 0XFF;
        
		LCDWrite(&dataH, 1);
		LCDWrite(&dataL, 1);
	}	
    
	SetLcdChipSelect();
}

static void LCDInitializeRegisters(void)
{	
	/* Frame rate control (in normal mode/full colors) */
	LCDCommandSelect(FRMCTR1); 
	LCDWriteByte(0x01); 
	LCDWriteByte(0x2C); 
	LCDWriteByte(0x2D); 
    /* Frame rate control (in idle mode/8-colors) */
	LCDCommandSelect(FRMCTR2); 
	LCDWriteByte(0x01); 
	LCDWriteByte(0x2C); 
	LCDWriteByte(0x2D); 
    /* Frame rate control (in partial mode/full colors) */
	LCDCommandSelect(FRMCTR3); 
	LCDWriteByte(0x01); 
	LCDWriteByte(0x2C); 
	LCDWriteByte(0x2D); 
	LCDWriteByte(0x01); 
	LCDWriteByte(0x2C); 
	LCDWriteByte(0x2D); 
    /* Display inversion control */
	LCDCommandSelect(INVCTR); 
	LCDWriteByte(0x07); 
	/* Power control 1 */
	LCDCommandSelect(PWCTR1); 
	LCDWriteByte(0xA2); 
	LCDWriteByte(0x02); 
	LCDWriteByte(0x84); 
    /* Power control 2 */
	LCDCommandSelect(PWCTR2); 
	LCDWriteByte(0xC5); 
    /* Power control 3 (in normal mode/full colors) */
	LCDCommandSelect(PWCTR3); 
	LCDWriteByte(0x0A); 
	LCDWriteByte(0x00); 
    /* Power control 4 (in idle mode/8-colors) */
	LCDCommandSelect(PWCTR4); 
	LCDWriteByte(0x8A); 
	LCDWriteByte(0x2A); 
    /* Power control 5 in partial mode/full colors) */
	LCDCommandSelect(PWCTR5); 
	LCDWriteByte(0x8A); 
	LCDWriteByte(0xEE); 
	/* VCOM control 1 */
	LCDCommandSelect(VMCTR1);
	LCDWriteByte(0x0E); 
	/* Gamma ('+'polarity) correction characteristics setting */
	LCDCommandSelect(GMCTRP1); 
	LCDWriteByte(0x0f); 
	LCDWriteByte(0x1a); 
	LCDWriteByte(0x0f); 
	LCDWriteByte(0x18); 
	LCDWriteByte(0x2f); 
	LCDWriteByte(0x28); 
	LCDWriteByte(0x20); 
	LCDWriteByte(0x22); 
	LCDWriteByte(0x1f); 
	LCDWriteByte(0x1b); 
	LCDWriteByte(0x23); 
	LCDWriteByte(0x37); 
	LCDWriteByte(0x00); 	
	LCDWriteByte(0x07); 
	LCDWriteByte(0x02); 
	LCDWriteByte(0x10); 
    /* Gamma ('-'polarity) correction characteristics setting */
	LCDCommandSelect(GMCTRN1); 
	LCDWriteByte(0x0f); 
	LCDWriteByte(0x1b); 
	LCDWriteByte(0x0f); 
	LCDWriteByte(0x17); 
	LCDWriteByte(0x33); 
	LCDWriteByte(0x2c); 
	LCDWriteByte(0x29); 
	LCDWriteByte(0x2e); 
	LCDWriteByte(0x30); 
	LCDWriteByte(0x30); 
	LCDWriteByte(0x39); 
	LCDWriteByte(0x3f); 
	LCDWriteByte(0x00); 
	LCDWriteByte(0x07); 
	LCDWriteByte(0x03); 
	LCDWriteByte(0x10);  	
    /* Interface pixel format */
	LCDCommandSelect(COLMOD); 
	LCDWriteByte(0x05);
}

void LCDSetGramScanWay();

void LCDInitialize()
{	
	/* Turn on the back-light */
	SetLcdBackLight();
	/* Hardware reset */
	LCDReset(); 
	/* Set the initialization register */
	LCDInitializeRegisters();
	/* Set the display scan and color transfer modes	 */
	LCDSetGramScanWay();
	CP0DelayMs(200); 
	/* Sleep out */
	LCDCommandSelect(SLPOUT);
	CP0DelayMs(120);
	/* Display on */
	LCDCommandSelect(DISPON);	
	/* clear */
	LCDClear(LCDData.background);	
}

void LCDSetGramScanWay()
{		
	/* set LCD width and height */
	if(LCDData.direction == LeftToRightUpToDown || LCDData.direction == LeftToRightDownToUp || 
       LCDData.direction == RightToLeftUpToDown || LCDData.direction == RightToLeftDownToUp)
    {
		LCDData.columns	= LCDData.height;
		LCDData.lines = LCDData.width;		
	}
    else
    {
		LCDData.columns	= LCDData.width;
		LCDData.lines = LCDData.height ;	
	}
	
	uint16_t MemoryAccessReg_Data=0;
    
	switch (LCDData.direction){				
		case LeftToRightUpToDown:
			MemoryAccessReg_Data = 0x00;// MY=0, MX=0, MV=0, ML=0, RGB=0, MH=0, D1=0, D0=0
			break;
		case LeftToRightDownToUp:
			MemoryAccessReg_Data = 0x80;// MY=1, MX=0, MV=0, ML=0, RGB=0, MH=0, D1=0, D0=0 
			break;
		case RightToLeftUpToDown:
			MemoryAccessReg_Data = 0x40;// MY=0, MX=1, MV=0, ML=0, RGB=0, MH=0, D1=0, D0=0 
			break;
		case RightToLeftDownToUp:
			MemoryAccessReg_Data = 0xC0;// MY=1, MX=1, MV=0, ML=0, RGB=0, MH=0, D1=0, D0=0 
			break;
		case UpToDownRightToLeft:
			MemoryAccessReg_Data = 0x20;// MY=0, MX=0, MV=1, ML=0, RGB=0, MH=0, D1=0, D0=0 
			break;
		case UpToDownLeftToRight:
			MemoryAccessReg_Data = 0x60;// MY=0, MX=1, MV=1, ML=0, RGB=0, MH=0, D1=0, D0=0 
			break;
		case DownToUpLeftToRight:
			MemoryAccessReg_Data = 0xA0;// MY=1, MX=0, MV=1, ML=0, RGB=0, MH=0, D1=0, D0=0 
			break;
		case DownToUpRightToLeft:
			MemoryAccessReg_Data = 0xE0;// MY=1, MX=1, MV=1, ML=0, RGB=0, MH=0, D1=0, D0=0 
			break;
	}
	
    //Memory data access control
	LCDCommandSelect(MADCTL);
     
	LCDWriteByte(MemoryAccessReg_Data & 0xf7);	//RGB color filter panel
}

void LCDSetWindows(LCDPoint start, LCDPoint end)
{
    //Column address set
	LCDCommandSelect(CASET);
    //Start column position
    //1st Parameter
	LCDWriteByte(0x00);
    //2nd Parameter
	LCDWriteByte((start.x & 0xff));
    //End column position
    //3rd Parameter
	LCDWriteByte(0x00);	
    //4th Parameter
	LCDWriteByte(((end.x - 1) & 0xff));
    
    //Row address	
	LCDCommandSelect(RASET);
    //Start row position
    //1st Parameter
	LCDWriteByte (0x00);
    //2nd Parameter
	LCDWriteByte ((start.y & 0xff));
    //End row position
    //3rd Parameter
	LCDWriteByte (0x00);
    //4th Parameter
	LCDWriteByte (((end.y - 1) & 0xff ));
    
	 //Memory write	
	LCDCommandSelect(RAMWR);	
}

void LCDSetCursor (LCDPoint point)
{
	LCDSetWindows(point, point);
}

void LCDSetPointlColor (LCDPoint point, LCDColor color)
{	
	if ((point.x <= LCDData.columns) && (point.y <= LCDData.lines))
    {		
		LCDSetCursor(point);		
        LCDWriteShort(color ,1); 
	}
}

void LCDSetArealColor (LCDPoint start, LCDPoint end, LCDColor  color)
{	
	if((end.x > start.x) && (end.y > start.y))
    {			
		LCDSetWindows(start, end);
        LCDWriteShort(color ,(uint32_t)((end.x - start.x) * (end.y - start.y))); 
	}
}

void LCDClear(LCDColor color)
{
    LCDPoint start = {0,0};
    LCDPoint end = {LCDData.columns, LCDData.lines};
    
	LCDSetArealColor(start, end, color);	
}

void LCDDrawPoint(LCDPoint point, uint16_t color)
{		
	if ( point.x > LCDData.columns || point.y > LCDData.lines ) 
    {
		return;
	}
	
    LCDSetPointlColor(point, color);
    return;
}

void LCDDrawLine (LCDPoint start, LCDPoint end, LCDColor color)
{
	if (start.x > LCDData.columns || start.y > LCDData.lines || end.x > LCDData.columns || end.y > LCDData.lines) 
    {
		return;
	}
    
    uint16_t dx = abs(end.x - start.x);

	uint16_t dy = abs(end.y - start.y);
 
    uint16_t step = (dx >= dy) ? dx : dy;
    
	dx /= step;
	dy /= step;
 
	uint16_t i=1;
	while(i <= step)
	{
        LCDDrawPoint(start, color);
        
		start.x += dx;
		start.y += dy;
		i++;
	}
}   

void LCDDrawRectangle (LCDPoint start, LCDPoint end, LCDColor lineColor, LCDColor fillColor, bool fill)
{
	if (start.x > LCDData.columns || start.y > LCDData.lines || end.x > LCDData.columns || end.y > LCDData.lines) 
    {
		return;
	}	
	LCDPoint p1= {start.x, start.y};
	LCDPoint p2= {end.x, start.y};
	LCDPoint p3= {start.x, end.y};
	LCDPoint p4= {end.x, end.y};
    
    LCDDrawLine(p1, p2, lineColor);
	LCDDrawLine(p1, p3, lineColor);
	LCDDrawLine(p4, p2, lineColor);
	LCDDrawLine(p4, p3, lineColor);		

    if (fill)
    {
        start.x -= 1;
        start.y -= 1;
        end.x -= 1;
        end.y -= 1;
		LCDSetArealColor(start, end, fillColor);
	}
}

void LCDDrawCircle (LCDPoint point, uint16_t radius, LCDColor lineColor, LCDColor fillColor, bool fill)
{						
	if (point.x > LCDData.columns || point.y >= LCDData.lines) 
    {
		return;
	}						
	
	int16_t x = 0; 
	int16_t y = radius;	
	int16_t d = 3 - (radius << 1);
    
    while (x <= y)
    { 	
        LCDPoint p1 = {point.x + x, point.y + y};
        LCDPoint p2 = {point.x - x, point.y + y};    
        LCDPoint p3 = {point.x - y, point.y + x};
        LCDPoint p4 = {point.x - y, point.y - x};
        LCDPoint p5 = {point.x - x, point.y - y};      
        LCDPoint p6 = {point.x + x, point.y - y};
        LCDPoint p7 = {point.x + y, point.y - x};
        LCDPoint p8 = {point.x + y, point.y + x};
        
        LCDDrawPoint(p1, lineColor);
        LCDDrawPoint(p2, lineColor);    
        LCDDrawPoint(p3, lineColor);
        LCDDrawPoint(p4, lineColor);
        LCDDrawPoint(p5, lineColor);      
        LCDDrawPoint(p6, lineColor);
        LCDDrawPoint(p7, lineColor);
        LCDDrawPoint(p8, lineColor);

        if (d < 0) 
        {
            d += 4 * x + 6;
        }
        else
        {
            d += 4 * ( x - y ) + 10;   
            y--;
        }
        x++;
    }

	if (fill)
    {
        radius--;
        x = 0; 
        y = radius;	
        d = 3 - (radius << 1);
	
        int16_t yPos;
		while ( x <= y )
        { 	
			for ( yPos = x; yPos <= y; yPos ++ ) 
            {     
                LCDPoint p1 = {point.x + x, point.y + yPos};
				LCDPoint p2 = {point.x - x, point.y + yPos}; 
				LCDPoint p3 = {point.x - yPos, point.y + x};
				LCDPoint p4 = {point.x - yPos, point.y - x};
				LCDPoint p5 = {point.x - x, point.y - yPos};
				LCDPoint p6 = {point.x + x, point.y - yPos};
				LCDPoint p7 = {point.x + yPos, point.y - x};	
				LCDPoint p8 = {point.x + yPos, point.y + x};
                
				LCDSetPointlColor(p1, fillColor);
				LCDSetPointlColor(p2, fillColor);    
				LCDSetPointlColor(p3, fillColor);
				LCDSetPointlColor(p4, fillColor);
				LCDSetPointlColor(p5, fillColor);  
				LCDSetPointlColor(p6, fillColor);
				LCDSetPointlColor(p7, fillColor);	
				LCDSetPointlColor(p8, fillColor);		
			}
            
			if ( d < 0 ) 
            {
				d += 4 * x + 6;	  
            }
			else
            {
				d += 4 * ( x - y ) + 10;   
				y--;
			}
			x++;
		}
	}
}

void LCDDrawChar (LCDPoint point, const char c)
{
	if (point.x >= LCDData.columns || point.y >= LCDData.lines) 
    {
		return;
	}
	if (c < ' ')/* not printable */
    {
        return;
    }
    
    uint32_t pos = (c - ' ');
    pos *= (LCDData.font->bytesHeight * LCDData.font->bytesWidth);
    
	const unsigned char* charFontPtr = &LCDData.font->characters[pos];

    uint16_t height, width;
    LCDPoint p;
    LCDColor color;
    
	for ( height = 0; height < LCDData.font->bytesHeight; height ++ )
    {
		for ( width = 0; width < LCDData.font->pixelWidth; width ++ )
        {
            p.x = point.x + width;
            p.y = point.y + height;
            
            if (*charFontPtr & (0x80 >> (width % 8)))
            {
                color = LCDData.textForeground;
            }
            else
            {
                color = LCDData.textBackground;
            }
           
            LCDSetPointlColor(p, color);

            if (width % 8 == 7) 
            {
                charFontPtr++;
            }	
		}
        
		if (LCDData.font->pixelWidth % 8 != 0) 
        {
				charFontPtr++;
		}
	}
}

void LCDDrawString (LCDPoint point, const char* string)
{
	if (point.x >= LCDData.columns || point.y >= LCDData.lines) {
		return;
	}
		
	LCDPoint p = {point.x, point.y};
	while (*string != '\0')
	{
		if ((p.x + LCDData.font->pixelWidth ) > LCDData.columns)
        {
			p.x = point.x;
			p.y += LCDData.font->pixelHeight;
		}
		
		if ((p.y  + LCDData.font->pixelHeight ) > LCDData.lines)
        {
			p.x = point.x;
			p.y = point.y;
		}		
        
		LCDDrawChar(p, *string);
		
		string++;
		
		p.x += LCDData.font->pixelWidth;	
	}
}
							
void LCDDrawNumber(LCDPoint point, int32_t number)
{
	if (point.x >= LCDData.columns || point.y >= LCDData.lines) 
    {
		return;
	}
	
    uint8_t stringBuffer[255];
    int16_t i = 254;
    stringBuffer[i--] = 0;
    
    if (number == 0)
    {
        stringBuffer[i--] = '0';
        i++;/* start of string */
    }
    else
    {
        char s = 0;
        if (number < 0)
        {
             s = '-';
             number *= -1;
        }
        
        while(number)
        {
            uint8_t c = number % 10 + '0';
            number /= 10;

            stringBuffer[i--] = c;
        }
        
        if (s != 0)
        {
             stringBuffer[i--] = s;
        }
        
        i++;/* start of string */
    }
    
	LCDDrawString(point, &stringBuffer[i]);
}

void LCDScroll(uint8_t line)
{
    /* Vertical scroll start address (line number from top) */
    LCDCommandSelect(VSCSAD); 
	LCDWriteByte(0x00); //SSA - Scroll start address
    LCDWriteByte(line); 
}

void LCDScrollSet(uint8_t startLine, uint8_t lenght)
{
    /* Scroll area set */
    LCDCommandSelect(SCRLAR); 
    LCDWriteByte(0x00);//TFA - Scroll top fixed area
    LCDWriteByte(startLine); 
    LCDWriteByte(0x00); //VSA - Scroll area
    LCDWriteByte(lenght);
    LCDWriteByte(0x00); //BFA - Bottom fixed area
    LCDWriteByte(162-lenght-startLine);
}


