/*****************************************************************************
* | File      	:   EPD_4in2_V2.h
* | Author      :   Waveshare team
* | Function    :   4.2inch e-paper V2
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-09-11
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "EPD_4in2_V2.h"

const unsigned char LUT_ALL[ 233 ] = {
    0x01, 0x0A, 0x1B, 0x0F, 0x03, 0x01, 0x01, 0x05, 0x0A, 0x01, 0x0A, 0x01, 0x01, 0x01, 0x05, 0x08, 0x03, 0x02, 0x04, 0x01, 0x01, 0x01, 0x04, 0x04, 0x02, 0x00, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x0A, 0x1B, 0x0F, 0x03, 0x01, 0x01, 0x05, 0x4A, 0x01, 0x8A, 0x01, 0x01, 0x01, 0x05, 0x48, 0x03, 0x82,
    0x84, 0x01, 0x01, 0x01, 0x84, 0x84, 0x82, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x0A, 0x1B, 0x8F, 0x03, 0x01,
    0x01, 0x05, 0x4A, 0x01, 0x8A, 0x01, 0x01, 0x01, 0x05, 0x48, 0x83, 0x82, 0x04, 0x01, 0x01, 0x01, 0x04, 0x04, 0x02, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x8A, 0x1B, 0x8F, 0x03, 0x01, 0x01, 0x05, 0x4A, 0x01, 0x8A, 0x01, 0x01, 0x01, 0x05, 0x48, 0x83, 0x02, 0x04, 0x01, 0x01, 0x01, 0x04, 0x04,
    0x02, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x8A, 0x9B, 0x8F, 0x03, 0x01, 0x01, 0x05, 0x4A, 0x01, 0x8A, 0x01,
    0x01, 0x01, 0x05, 0x48, 0x03, 0x42, 0x04, 0x01, 0x01, 0x01, 0x04, 0x04, 0x42, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x07, 0x17, 0x41, 0xA8, 0x32, 0x30,
};

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_4IN2_V2_Reset( void )
{
    EPD_Digital_Write( EPD_RST_PIN, 1 );
    HAL_Delay( 100 );
    EPD_Digital_Write( EPD_RST_PIN, 0 );
    HAL_Delay( 2 );
    EPD_Digital_Write( EPD_RST_PIN, 1 );
    HAL_Delay( 100 );
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_4IN2_V2_SendCommand( uint8_t Reg )
{
    EPD_Digital_Write( EPD_DC_PIN, 0 );
    EPD_Digital_Write( EPD_CS_PIN, 0 );
    EPD_SPI_WriteByte( Reg );
    EPD_Digital_Write( EPD_CS_PIN, 1 );
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_4IN2_V2_SendData( uint8_t Data )
{
    EPD_Digital_Write( EPD_DC_PIN, 1 );
    EPD_Digital_Write( EPD_CS_PIN, 0 );
    EPD_SPI_WriteByte( Data );
    EPD_Digital_Write( EPD_CS_PIN, 1 );
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
void EPD_4IN2_V2_ReadBusy( void )
{
    printf( "e-Paper busy\r\n" );
    while ( EPD_Digital_Read( EPD_BUSY_PIN ) == 1 ) {  // LOW: idle, HIGH: busy
        HAL_Delay( 10 );
    }
    printf( "e-Paper busy release\r\n" );
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_4IN2_V2_TurnOnDisplay( void )
{
    EPD_4IN2_V2_SendCommand( 0x22 );
    EPD_4IN2_V2_SendData( 0xF7 );
    EPD_4IN2_V2_SendCommand( 0x20 );
    EPD_4IN2_V2_ReadBusy();
}

static void EPD_4IN2_V2_TurnOnDisplay_Fast( void )
{
    EPD_4IN2_V2_SendCommand( 0x22 );
    EPD_4IN2_V2_SendData( 0xC7 );
    EPD_4IN2_V2_SendCommand( 0x20 );
    EPD_4IN2_V2_ReadBusy();
}

static void EPD_4IN2_V2_TurnOnDisplay_Partial( void )
{
    EPD_4IN2_V2_SendCommand( 0x22 );
    EPD_4IN2_V2_SendData( 0xFF );
    EPD_4IN2_V2_SendCommand( 0x20 );
    EPD_4IN2_V2_ReadBusy();
}

static void EPD_4IN2_V2_TurnOnDisplay_4Gray( void )
{
    EPD_4IN2_V2_SendCommand( 0x22 );
    EPD_4IN2_V2_SendData( 0xCF );
    EPD_4IN2_V2_SendCommand( 0x20 );
    EPD_4IN2_V2_ReadBusy();
}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
static void EPD_4IN2_V2_SetWindows( uint32_t Xstart, uint32_t Ystart, uint32_t Xend, uint32_t Yend )
{
    EPD_4IN2_V2_SendCommand( 0x44 );  // SET_RAM_X_ADDRESS_START_END_POSITION
    EPD_4IN2_V2_SendData( ( Xstart >> 3 ) & 0xFF );
    EPD_4IN2_V2_SendData( ( Xend >> 3 ) & 0xFF );

    EPD_4IN2_V2_SendCommand( 0x45 );  // SET_RAM_Y_ADDRESS_START_END_POSITION
    EPD_4IN2_V2_SendData( Ystart & 0xFF );
    EPD_4IN2_V2_SendData( ( Ystart >> 8 ) & 0xFF );
    EPD_4IN2_V2_SendData( Yend & 0xFF );
    EPD_4IN2_V2_SendData( ( Yend >> 8 ) & 0xFF );
}

/******************************************************************************
function :	Set Cursor
parameter:
******************************************************************************/
static void EPD_4IN2_V2_SetCursor( uint32_t Xstart, uint32_t Ystart )
{
    EPD_4IN2_V2_SendCommand( 0x4E );  // SET_RAM_X_ADDRESS_COUNTER
    EPD_4IN2_V2_SendData( Xstart & 0xFF );

    EPD_4IN2_V2_SendCommand( 0x4F );  // SET_RAM_Y_ADDRESS_COUNTER
    EPD_4IN2_V2_SendData( Ystart & 0xFF );
    EPD_4IN2_V2_SendData( ( Ystart >> 8 ) & 0xFF );
}

// LUT download
static void EPD_4IN2_V2_4Gray_lut( void )
{
    unsigned char i;

    // WS byte 0~152, the content of VS[nX-LUTm], TP[nX], RP[n], SR[nXY], FR[n] and XON[nXY]
    EPD_4IN2_V2_SendCommand( 0x32 );
    for ( i = 0; i < 227; i++ ) {
        EPD_4IN2_V2_SendData( LUT_ALL[ i ] );
    }
    // WS byte 153, the content of Option for LUT end
    EPD_4IN2_V2_SendCommand( 0x3F );
    EPD_4IN2_V2_SendData( LUT_ALL[ i++ ] );

    // WS byte 154, the content of gate leve
    EPD_4IN2_V2_SendCommand( 0x03 );
    EPD_4IN2_V2_SendData( LUT_ALL[ i++ ] );  // VGH

    // WS byte 155~157, the content of source level
    EPD_4IN2_V2_SendCommand( 0x04 );
    EPD_4IN2_V2_SendData( LUT_ALL[ i++ ] );  // VSH1
    EPD_4IN2_V2_SendData( LUT_ALL[ i++ ] );  // VSH2
    EPD_4IN2_V2_SendData( LUT_ALL[ i++ ] );  // VSL

    // WS byte 158, the content of VCOM level
    EPD_4IN2_V2_SendCommand( 0x2c );
    EPD_4IN2_V2_SendData( LUT_ALL[ i++ ] );  // VCOM
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_4IN2_V2_Init( void )
{
    EPD_Init();

    EPD_4IN2_V2_Reset();

    EPD_4IN2_V2_ReadBusy();
    EPD_4IN2_V2_SendCommand( 0x12 );  // soft  reset
    EPD_4IN2_V2_ReadBusy();

    EPD_4IN2_V2_SendCommand( 0x01 );  // Driver output control
    EPD_4IN2_V2_SendData( ( EPD_4IN2_V2_HEIGHT - 1 ) % 256 );
    EPD_4IN2_V2_SendData( ( EPD_4IN2_V2_HEIGHT - 1 ) / 256 );
    EPD_4IN2_V2_SendData( 0x00 );

    EPD_4IN2_V2_SendCommand( 0x21 );  //  Display update control
    EPD_4IN2_V2_SendData( 0x80 );
    EPD_4IN2_V2_SendData( 0x00 );

    EPD_4IN2_V2_SendCommand( 0x3C );  // BorderWavefrom
    EPD_4IN2_V2_SendData( 0x05 );

    EPD_4IN2_V2_SendCommand( 0x11 );  // data  entry  mode
    EPD_4IN2_V2_SendData( 0x03 );     // X-mode

    EPD_4IN2_V2_SetWindows( 0, 0, EPD_4IN2_V2_WIDTH - 1, EPD_4IN2_V2_HEIGHT - 1 );

    EPD_4IN2_V2_SetCursor( 0, 0 );

    EPD_4IN2_V2_ReadBusy();
}

/******************************************************************************
function :	Initialize Fast the e-Paper register
parameter:
******************************************************************************/
void EPD_4IN2_V2_Init_Fast( uint8_t Mode )
{
    EPD_Init();

    EPD_4IN2_V2_Reset();

    EPD_4IN2_V2_ReadBusy();
    EPD_4IN2_V2_SendCommand( 0x12 );  // soft  reset
    EPD_4IN2_V2_ReadBusy();

    EPD_4IN2_V2_SendCommand( 0x21 );
    EPD_4IN2_V2_SendData( 0x80 );
    EPD_4IN2_V2_SendData( 0x00 );

    EPD_4IN2_V2_SendCommand( 0x3C );
    EPD_4IN2_V2_SendData( 0x05 );

    if ( Mode == Seconds_1_5S ) {
        // 1.5s
        EPD_4IN2_V2_SendCommand( 0x1A );  // Write to temperature register
        EPD_4IN2_V2_SendData( 0x6E );
    }
    else if ( Mode == Seconds_1S ) {
        // 1s
        EPD_4IN2_V2_SendCommand( 0x1A );  // Write to temperature register
        EPD_4IN2_V2_SendData( 0x5A );
    }

    EPD_4IN2_V2_SendCommand( 0x22 );  // Load temperature value
    EPD_4IN2_V2_SendData( 0x91 );
    EPD_4IN2_V2_SendCommand( 0x20 );
    EPD_4IN2_V2_ReadBusy();

    EPD_4IN2_V2_SendCommand( 0x11 );  // data  entry  mode
    EPD_4IN2_V2_SendData( 0x03 );     // X-mode

    EPD_4IN2_V2_SetWindows( 0, 0, EPD_4IN2_V2_WIDTH - 1, EPD_4IN2_V2_HEIGHT - 1 );

    EPD_4IN2_V2_SetCursor( 0, 0 );

    EPD_4IN2_V2_ReadBusy();
}

void EPD_4IN2_V2_Init_4Gray( void )
{
    EPD_Init();

    EPD_4IN2_V2_Reset();

    EPD_4IN2_V2_SendCommand( 0x12 );  // SWRESET
    EPD_4IN2_V2_ReadBusy();

    EPD_4IN2_V2_SendCommand( 0x21 );
    EPD_4IN2_V2_SendData( 0x00 );
    EPD_4IN2_V2_SendData( 0x00 );

    EPD_4IN2_V2_SendCommand( 0x3C );
    EPD_4IN2_V2_SendData( 0x03 );

    EPD_4IN2_V2_SendCommand( 0x0C );  // BTST
    EPD_4IN2_V2_SendData( 0x8B );     // 8B
    EPD_4IN2_V2_SendData( 0x9C );     // 9C
    EPD_4IN2_V2_SendData( 0xA4 );     // 96 A4
    EPD_4IN2_V2_SendData( 0x0F );     // 0F

    EPD_4IN2_V2_SendCommand(0x01);   // 驱动输出控制      drive output control
    EPD_4IN2_V2_SendData(0x2B); //  Y 的低字节
    EPD_4IN2_V2_SendData(0x01); //  Y 的高字节
    EPD_4IN2_V2_SendData(0x00);

    EPD_4IN2_V2_4Gray_lut();  // LUT

    EPD_4IN2_V2_SendCommand( 0x11 );  // data  entry  mode
    EPD_4IN2_V2_SendData( 0x03 );     // X-mode

    EPD_4IN2_V2_SetWindows( 0, 0, EPD_4IN2_V2_WIDTH - 1, EPD_4IN2_V2_HEIGHT - 1 );

    EPD_4IN2_V2_SetCursor( 0, 0 );
}
/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_4IN2_V2_Clear( void )
{
    uint32_t Width, Height;
    Width  = ( EPD_4IN2_V2_WIDTH % 8 == 0 ) ? ( EPD_4IN2_V2_WIDTH / 8 ) : ( EPD_4IN2_V2_WIDTH / 8 + 1 );
    Height = EPD_4IN2_V2_HEIGHT;

    EPD_4IN2_V2_SendCommand( 0x24 );
    for ( uint32_t j = 0; j < Height; j++ ) {
        for ( uint32_t i = 0; i < Width; i++ ) {
            EPD_4IN2_V2_SendData( 0xFF );
        }
    }

    EPD_4IN2_V2_SendCommand( 0x26 );
    for ( uint32_t j = 0; j < Height; j++ ) {
        for ( uint32_t i = 0; i < Width; i++ ) {
            EPD_4IN2_V2_SendData( 0xFF );
        }
    }
    EPD_4IN2_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_4IN2_V2_Display( const uint8_t* blackimage, const uint8_t* ryimage )
{
    uint32_t Width, Height;
    Width  = ( EPD_4IN2_V2_WIDTH % 8 == 0 ) ? ( EPD_4IN2_V2_WIDTH / 8 ) : ( EPD_4IN2_V2_WIDTH / 8 + 1 );
    Height = EPD_4IN2_V2_HEIGHT;

    EPD_4IN2_V2_SendCommand( 0x24 );
    for ( uint32_t j = 0; j < Height; j++ ) {
        for ( uint32_t i = 0; i < Width; i++ ) {
            EPD_4IN2_V2_SendData( blackimage[ i + j * Width ] );
        }
    }

    EPD_4IN2_V2_SendCommand( 0x26 );
    for ( uint32_t j = 0; j < Height; j++ ) {
        for ( uint32_t i = 0; i < Width; i++ ) {
            EPD_4IN2_V2_SendData( ryimage[ i + j * Width ] );
        }
    }
    EPD_4IN2_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and fast displays
parameter:
******************************************************************************/
void EPD_4IN2_V2_Display_Fast( uint8_t* Image )
{
    uint32_t Width, Height;
    Width  = ( EPD_4IN2_V2_WIDTH % 8 == 0 ) ? ( EPD_4IN2_V2_WIDTH / 8 ) : ( EPD_4IN2_V2_WIDTH / 8 + 1 );
    Height = EPD_4IN2_V2_HEIGHT;

    EPD_4IN2_V2_SendCommand( 0x24 );
    for ( uint32_t j = 0; j < Height; j++ ) {
        for ( uint32_t i = 0; i < Width; i++ ) {
            EPD_4IN2_V2_SendData( Image[ i + j * Width ] );
        }
    }

    EPD_4IN2_V2_SendCommand( 0x26 );
    for ( uint32_t j = 0; j < Height; j++ ) {
        for ( uint32_t i = 0; i < Width; i++ ) {
            EPD_4IN2_V2_SendData( Image[ i + j * Width ] );
        }
    }
    EPD_4IN2_V2_TurnOnDisplay_Fast();
}

void EPD_4IN2_V2_Display_4Gray( uint8_t* Image )
{
    uint64_t i, j, k, m;
    uint8_t temp1, temp2, temp3;
    /****Color display description****
          white  gray2  gray1  black
    0x10|  01     01     00     00
    0x13|  01     00     01     00
    *********************************/
    EPD_4IN2_V2_SendCommand( 0x24 );
    // EPD_4IN2_HEIGHT
    // EPD_4IN2_WIDTH
    for ( m = 0; m < EPD_4IN2_V2_HEIGHT; m++ )
        for ( i = 0; i < EPD_4IN2_V2_WIDTH / 8; i++ ) {
            temp3 = 0;
            for ( j = 0; j < 2; j++ ) {

                temp1 = Image[ ( m * ( EPD_4IN2_V2_WIDTH / 8 ) + i ) * 2 + j ];
                for ( k = 0; k < 2; k++ ) {
                    temp2 = temp1 & 0xC0;
                    if ( temp2 == 0xC0 )
                        temp3 |= 0x01;  // white
                    else if ( temp2 == 0x00 )
                        temp3 |= 0x00;  // black
                    else if ( temp2 == 0x80 )
                        temp3 |= 0x00;  // gray1
                    else                // 0x40
                        temp3 |= 0x01;  // gray2
                    temp3 <<= 1;

                    temp1 <<= 2;
                    temp2 = temp1 & 0xC0;
                    if ( temp2 == 0xC0 )  // white
                        temp3 |= 0x01;
                    else if ( temp2 == 0x00 )  // black
                        temp3 |= 0x00;
                    else if ( temp2 == 0x80 )
                        temp3 |= 0x00;  // gray1
                    else                // 0x40
                        temp3 |= 0x01;  // gray2
                    if ( j != 1 || k != 1 )
                        temp3 <<= 1;

                    temp1 <<= 2;
                }
            }
            EPD_4IN2_V2_SendData( temp3 );
        }
    // new  data
    EPD_4IN2_V2_SendCommand( 0x26 );
    for ( m = 0; m < EPD_4IN2_V2_HEIGHT; m++ )
        for ( i = 0; i < EPD_4IN2_V2_WIDTH / 8; i++ ) {
            temp3 = 0;
            for ( j = 0; j < 2; j++ ) {
                temp1 = Image[ ( m * ( EPD_4IN2_V2_WIDTH / 8 ) + i ) * 2 + j ];
                for ( k = 0; k < 2; k++ ) {
                    temp2 = temp1 & 0xC0;
                    if ( temp2 == 0xC0 )
                        temp3 |= 0x01;  // white
                    else if ( temp2 == 0x00 )
                        temp3 |= 0x00;  // black
                    else if ( temp2 == 0x80 )
                        temp3 |= 0x01;  // gray1
                    else                // 0x40
                        temp3 |= 0x00;  // gray2
                    temp3 <<= 1;

                    temp1 <<= 2;
                    temp2 = temp1 & 0xC0;
                    if ( temp2 == 0xC0 )  // white
                        temp3 |= 0x01;
                    else if ( temp2 == 0x00 )  // black
                        temp3 |= 0x00;
                    else if ( temp2 == 0x80 )
                        temp3 |= 0x01;  // gray1
                    else                // 0x40
                        temp3 |= 0x00;  // gray2
                    if ( j != 1 || k != 1 )
                        temp3 <<= 1;

                    temp1 <<= 2;
                }
            }
            EPD_4IN2_V2_SendData( temp3 );
        }
    EPD_4IN2_V2_TurnOnDisplay_4Gray();
}

// Send partial data for partial refresh
void EPD_4IN2_V2_PartialDisplay( uint8_t* Image, uint32_t x, uint32_t y, uint32_t w, uint32_t l )
{
    uint32_t Width, Height;
    Width  = ( w % 8 == 0 ) ? ( w / 8 ) : ( w / 8 + 1 );
    Height = l;

    EPD_4IN2_V2_SendCommand( 0x3C );  // BorderWavefrom,
    EPD_4IN2_V2_SendData( 0x80 );

    EPD_4IN2_V2_SendCommand( 0x21 );
    EPD_4IN2_V2_SendData( 0x00 );
    EPD_4IN2_V2_SendData( 0x00 );

    EPD_4IN2_V2_SendCommand( 0x3C );
    EPD_4IN2_V2_SendData( 0x80 );

    EPD_4IN2_V2_SetWindows( x, y, x + w - 1, y + l - 1 );
    EPD_4IN2_V2_SetCursor( x, y );

    EPD_4IN2_V2_SendCommand( 0x24 );
    for ( uint32_t j = 0; j < Height; j++ ) {
        for ( uint32_t i = 0; i < Width; i++ ) {
            EPD_4IN2_V2_SendData( Image[ i + j * Width ] );
        }
    }

    EPD_4IN2_V2_TurnOnDisplay_Partial();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_4IN2_V2_Sleep( void )
{
    EPD_4IN2_V2_SendCommand( 0x10 );  // DEEP_SLEEP
    EPD_4IN2_V2_SendData( 0x01 );
    HAL_Delay( 200 );
}