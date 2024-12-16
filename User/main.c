#include "FreeRTOS.h"
#include "delay.h"
#include "demos/lv_demos.h"
#include "key.h"
#include "lcd.h"
#include "led.h"
#include "log.h"
#include "lv_port_disp.h"
#include "lvgl.h"
#include "stepper_control.h"
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "task.h"
#include "usart_helper.h"

const char* TAG = "main";

TaskHandle_t hblink_task, hlv_timer_task, hlv_refresh_task, hstart_task, hdetect_key_task, hstepper_move_task;
static uint8_t key_state;

static void blink_task()
{
    while ( 1 ) {
        LOGD( TAG, "Toggle LED" );
        LED_Toggle( LED1 );
        vTaskDelay( pdMS_TO_TICKS( 200 ) );
    }
}
static void detect_key_task()
{
    while ( 1 ) {
        key_state = key_get_state();
        vTaskDelay( pdMS_TO_TICKS( 50 ) );
    }
}
static void stepper_move_task()
{
    while ( 1 ) {
        if ( key_state & 0x1 ) {
            Linear_Interpolation( 200, 0, 1000 );
            LOGD( TAG, "MOVE right" );
        }
        else if ( key_state & ( 0x1 << 5 ) ) {
            Linear_Interpolation( 0, 200, 1000 );
            LOGD( TAG, "MOVE down" );
        }
        else if ( key_state & ( 0x1 << 2 ) ) {
            Linear_Interpolation( -200, 0, 1000 );
            LOGD( TAG, "MOVE left" );
        }
        else if ( key_state & ( 0x1 << 1 ) ) {
            Linear_Interpolation( 0, -200, 1000 );
            LOGD( TAG, "MOVE up" );
        }
        else if ( key_state & ( 0x1 << 3 ) ) {
            Circular_InterPolation( 15000, 0, 0, -15000, 1500, CW );
            while ( circular_para.motionstatus )
                ;
            Circular_InterPolation( 0, -15000, -15000, 0,1500, CW );
            while ( circular_para.motionstatus )
                ;
            Circular_InterPolation( -15000, 0, 0, 15000, 1500, CW );
            while ( circular_para.motionstatus )
                ;
            Circular_InterPolation( 0, 15000, 15000, 0, 1500, CW );
            while ( circular_para.motionstatus )
                ;
        }
        else if ( key_state & 0x1 << 4 ) {
            // Linear_Interpolation( 10000, 15000, 2000 );
            Circular_InterPolation( 15000, 0, 0, 15000, 1500, CCW );
            while ( circular_para.motionstatus )
                ;
            Circular_InterPolation( 0, 15000, -15000, 0,1500, CCW );
            while ( circular_para.motionstatus )
                ;
            Circular_InterPolation( -15000, 0, 0, -15000,1500, CCW );
            while ( circular_para.motionstatus )
                ;
            Circular_InterPolation( 0, -15000, 15000, 0, 1500, CCW );
            while ( circular_para.motionstatus )
                ;
        }
        vTaskDelay( 5 );
    }
}
static void lv_timer_task()
{
    while ( 1 ) {
        lv_timer_handler();
        vTaskDelay( pdMS_TO_TICKS( 10 ) );
    }
}
static void lv_refresh_task()
{
    lv_lock();
    // lv_obj_t* label = lv_label_create( lv_scr_act() );
    // lv_label_set_text( label, "count:" );
    // lv_obj_set_style_text_color( label, lv_palette_main( LV_PALETTE_BLUE ), 0 );
    // lv_obj_set_pos( label, 10, 10 );
    char* info[] = { "benchmark" };
    lv_demos_create( info, 1 );
    lv_unlock();
    // int count = 0;
    while ( 1 ) {
        // lv_lock();
        // lv_label_set_text_fmt( label, "count:%d", count );
        // lv_unlock();
        // count++;
        vTaskDelay( pdMS_TO_TICKS( 1000 ) );
        printf( "task1 \r\n" );
    }
}
static void start_task()
{
    taskENTER_CRITICAL();
    // lv_init();
    // lv_port_disp_init();
    // lv_os_init();
    LED_Init();
    key_init();
    vTaskDelete( hstart_task );
    taskEXIT_CRITICAL();
}

void vApplicationTickHook()
{
    lv_tick_inc( 1 );
}

int main()
{
    sys_stm32_clock_init( 336, 8, 2, 7 );
    delay_init( 168 );
    usart_debug_init();
    stepper_Init();
    Linear_Interpolation( 0, 2000, 8000 );

    // Circular_InterPolation( 0,-2000, 0, 2000, 8000,CW );

    // Linear_Interpolation( 300, 300, 8000 );
    // Linear_Interpolation( 10000, 0, 8000 );
    // Linear_Interpolation( 0, 0, 8000 );
    xTaskCreate( start_task, "start_task", 1024, NULL, 10, &hstart_task );
    xTaskCreate( blink_task, "blink_task", 2048, NULL, 5, &hblink_task );
    xTaskCreate( detect_key_task, "detect_key_task", 2048, NULL, 5, &hdetect_key_task );
    xTaskCreate( stepper_move_task, "stepper_move_task", 2048, NULL, 5, &hstepper_move_task );
    // xTaskCreate( lv_timer_task, "lv_timer_task", 2048, NULL, 6, &hlv_timer_task );
    // xTaskCreate( lv_refresh_task, "lv_refresh_task", 2048, NULL, 7, &hlv_refresh_task );
    vTaskStartScheduler();
    return 0;
}
