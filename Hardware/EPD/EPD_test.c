/*****************************************************************************
* | File      	:   EPD_2in13bc_test.c
* | Author      :   Waveshare team
* | Function    :   2.13inch B&C e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-13
* | Info        :
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
#include "EPD_Test.h"
#include "EPD_4in2/EPD_4in2_V2.h"
#include "EPD_defs.h"

int EPD_test( void )
{
    printf( "EPD_2IN13BC_test Demo\r\n" );
    EPD_4IN2_V2_Init_4Gray();
    printf( "e-Paper Init and Clear...\r\n" );
    EPD_4IN2_V2_Clear();
    HAL_Delay( 500 );

    // Create a new image cache named IMAGE_BW and fill it with white
    uint8_t *BlackImage, *RYImage;  // Red or Yellow
    uint16_t Imagesize = ( ( EPD_WIDTH % 8 == 0 ) ? ( EPD_WIDTH / 8 ) : ( EPD_WIDTH / 8 + 1 ) ) * EPD_HEIGHT;
    if ( ( BlackImage = ( uint8_t* )malloc( Imagesize ) ) == NULL ) {
        printf( "Failed to apply for black memory...\r\n" );
        return -1;
    }
    if ( ( RYImage = ( uint8_t* )malloc( Imagesize ) ) == NULL ) {
        printf( "Failed to apply for red memory...\r\n" );
        return -1;
    }
    printf( "NewImage:BlackImage and RYImage\r\n" );
    Paint_NewImage( BlackImage, EPD_WIDTH, EPD_HEIGHT, 270, WHITE );
    Paint_NewImage( RYImage, EPD_WIDTH, EPD_HEIGHT, 270, WHITE );

    // Select Image
    Paint_SelectImage( BlackImage );
    Paint_Clear( WHITE );
    Paint_SelectImage( RYImage );
    Paint_Clear( WHITE );

#if 1  // show image for array
    printf( "show image for array\r\n" );
    //EPD_4IN2_V2_Display( gImage_test1, gImage_4in2bc_ry );
    EPD_4IN2_V2_Display_4Gray( gImage_test1 );
    HAL_Delay( 8000 );
#endif

#if 1  // Drawing on the image
       /*Horizontal screen*/
    // 1.Draw black image
    printf( "Draw black image\r\n" );
    Paint_SelectImage( BlackImage );
    Paint_Clear( WHITE );
    Paint_DrawPoint( 5, 70, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT );
    Paint_DrawPoint( 5, 80, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT );
    Paint_DrawLine( 20, 70, 50, 100, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID );
    Paint_DrawLine( 50, 70, 20, 100, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID );
    Paint_DrawRectangle( 60, 70, 90, 100, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL );
    Paint_DrawCircle( 125, 85, 15, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY );
    Paint_DrawString_CN( 5, 15, "ÄãºÃabc", &Font12CN, WHITE, BLACK );

    // 2.Draw red image
    printf( "Draw red image\r\n" );
    Paint_SelectImage( RYImage );
    Paint_Clear( WHITE );
    Paint_DrawPoint( 5, 90, RED, DOT_PIXEL_3X3, DOT_STYLE_DFT );
    Paint_DrawPoint( 5, 100, RED, DOT_PIXEL_4X4, DOT_STYLE_DFT );
    Paint_DrawLine( 125, 70, 125, 100, RED, DOT_PIXEL_1X1, LINE_STYLE_DOTTED );
    Paint_DrawLine( 110, 85, 140, 85, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID );
    Paint_DrawRectangle( 20, 70, 50, 100, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY );
    Paint_DrawCircle( 165, 85, 15, RED, DOT_PIXEL_1X1, DRAW_FILL_FULL );
    Paint_DrawString_EN( 50, 50, "waveshare Electronics", &Font12, BLACK, WHITE );
    Paint_DrawNum( 5, 100, 987654321, &Font16, WHITE, RED );

    printf( "EPD_Display\r\n" );
    // EPD_4IN2_V2_Display( BlackImage, RYImage );
    HAL_Delay( 8000 );
#endif

    // printf( "Clear...\r\n" );
    // //EPD_Clear();

    // printf( "Goto Sleep...\r\n" );
    // EPD_Sleep();
    // free( BlackImage );
    // free( RYImage );
    // BlackImage = NULL;
    // RYImage    = NULL;

    // // close 5V
    // printf( "close 5V, Module enters 0 power consumption ...\r\n" );
    // EPD_Exit();

    return 0;
}
