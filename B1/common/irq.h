/****************************************************************************
  interrupt System Source File
  Company : Singular Software
  File Name : irq.h
  Summary : configure and control functions.
  Description :  initialize and control PIC32MZ interrupt system.
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

#ifndef _IRQ_H
#define _IRQ_H

#ifdef __cplusplus
extern "C" {
#endif
    
void IRQInitialize( void );
    
#ifdef __cplusplus
}
#endif

#endif /* _IRQ_H */
