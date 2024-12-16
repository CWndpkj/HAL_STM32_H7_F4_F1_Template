/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include "lcd.h"
#include <stdbool.h>

/*********************
 *      DEFINES
 *********************/
#define MY_DISP_HOR_RES 480

#define MY_DISP_VER_RES 272

#define BYTE_PER_PIXEL ( LV_COLOR_FORMAT_GET_SIZE( LV_COLOR_FORMAT_RGB565 ) ) /*will be 2 for RGB565 */

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init( void );

static void disp_flush( lv_display_t* disp, const lv_area_t* area, uint8_t* px_map );

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init( void )
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*------------------------------------
     * Create a display and set a flush_cb
     * -----------------------------------*/
    lv_display_t* disp = lv_display_create( MY_DISP_HOR_RES, MY_DISP_VER_RES );
    lv_display_set_flush_cb( disp, disp_flush );

    /* Example 1
     * One buffer for partial rendering*/
    LV_ATTRIBUTE_MEM_ALIGN
    static uint8_t buf_1_1[ MY_DISP_HOR_RES * 40 * BYTE_PER_PIXEL ]; /*A buffer for 10 rows*/
    lv_display_set_buffers( disp, buf_1_1, NULL, sizeof( buf_1_1 ), LV_DISPLAY_RENDER_MODE_PARTIAL );

    /* Example 2
     * Two buffers for partial rendering
     * In flush_cb DMA or similar hardware should be used to update the display in the background.*/
    // LV_ATTRIBUTE_MEM_ALIGN
    // static uint8_t buf_2_1[ MY_DISP_HOR_RES * BYTE_PER_PIXEL * 50 ];

    // LV_ATTRIBUTE_MEM_ALIGN
    // static uint8_t buf_2_2[ MY_DISP_HOR_RES * BYTE_PER_PIXEL * 50 ];
    // lv_display_set_buffers( disp, buf_2_1, buf_2_2, sizeof( buf_2_1 ), LV_DISPLAY_RENDER_MODE_PARTIAL );

    /* Example 3
     * Two buffers screen sized buffer for double buffering.
     * Both LV_DISPLAY_RENDER_MODE_DIRECT and LV_DISPLAY_RENDER_MODE_FULL works, see their comments*/
    // LV_ATTRIBUTE_MEM_ALIGN
    // static uint8_t buf_3_1[ MY_DISP_HOR_RES * MY_DISP_VER_RES * BYTE_PER_PIXEL ];

    // LV_ATTRIBUTE_MEM_ALIGN
    // static uint8_t buf_3_2[ MY_DISP_HOR_RES * MY_DISP_VER_RES * BYTE_PER_PIXEL ];
    // lv_display_set_buffers( disp, buf_3_1, buf_3_2, sizeof( buf_3_1 ), LV_DISPLAY_RENDER_MODE_DIRECT );
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init( void )
{
    /*You code here*/
    lcd_init();
    lcd_fill( 0, 0, MY_DISP_HOR_RES, MY_DISP_VER_RES, 0x0 );
    lcd_display_on();
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update( void )
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update( void )
{
    disp_flush_enabled = false;
}

/*Flush the content of the internal buffer the specific area on the display.
 *`px_map` contains the rendered image as raw pixel map and it should be copied to `area` on the display.
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_display_flush_ready()' has to be called when it's finished.*/
static void disp_flush( lv_display_t* disp_drv, const lv_area_t* area, uint8_t* px_map )
{
    if ( disp_flush_enabled ) {
        uint16_t* buf16 = ( uint16_t* )px_map; /* Let's say it's a 16 bit (RGB565) display */
        lcd_fill_rect_with_buffer( area->x1, area->y1, area->x2, area->y2, buf16 );
        // lcd_color_fill(area->x1, area->y1, area->x2, area->y2, buf16);
    }
    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_display_flush_ready( disp_drv );
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
