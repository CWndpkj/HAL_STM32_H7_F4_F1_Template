#include "usmart_config.h"
#include "usmart.h"
#include "usmart_str.h"
#include"log.h"
/******************************************************************************************/
/* 用户配置区
 * 这下面要包含所用到的函数所申明的头文件(用户自己添加)
 */

#include "delay.h"
#include "sys.h"
#include "u8g2.h"

extern u8g2_t u8g2;
/* 函数名列表初始化(用户自己添加)
 * 用户直接在这里输入要执行的函数名及其查找串
 */

void oled_display()
{
    u8g2_SendBuffer( &u8g2 );
}
void oled_clear()
{
    u8g2_ClearDisplay( &u8g2 );
}
struct _m_usmart_nametab usmart_nametab[] = {
#if USMART_USE_WRFUNS == 1 /* 如果使能了读写操作 */
    { ( void* )read_addr, "uint32_t read_addr(uint32_t addr)" },
    { ( void* )write_addr, "void write_addr(uint32_t addr, uint32_t val)" },
#endif
    { ( void* )oled_display, "void oled_display()" },
    { ( void* )oled_clear, "void oled_clear()" }
};

void rcv_complete_cb( uint8_t* data )
{
    LOGI("usmart", "rcv:%s\r\n", data );
}
Data_rcv_handler_t data_rcv_handler = {
    .last_transfer_timestamp = 0,
    .rcv_buffer_0            = NULL,  // 将在usmart_rcv_data中初始化
    .rcv_buffer_1            = NULL,
    .use_muti_buffer         = 1,
    .rcv_complete_cb         = rcv_complete_cb,
};

/******************************************************************************************/

/* 函数控制管理器初始化
 * 得到各个受控函数的名字
 * 得到函数总数量
 */
struct _m_usmart_dev usmart_dev = {
    .funs        = usmart_nametab,
    .init        = usmart_init,
    .cmd_rec     = usmart_cmd_rec,
    .exe         = usmart_exe,
    .scan        = usmart_scan,
    .fnum        = sizeof( usmart_nametab ) / sizeof( struct _m_usmart_nametab ), /* 函数数量 */
    .pnum        = 0,                                                             // 参数数量
    .id          = 0,                                                             // 函数ID
    .sptype      = 0,                                                             // 参数显示类型,0,10进制;1,16进制
    .parmtype    = 0,                                                             // 参数类型.bitx:,0,数字;1,字符串
    .plentbl     = { 0 },                                                         // 每个参数的长度暂存表,需要MAX_PARM个0初始化
    .parm        = { 0 },                                                         // 函数的参数,需要PARM_LEN个0初始化
    .runtimeflag = 1,  // 0,不统计函数执行时间;1,统计函数执行时间,注意:此功能必须在USMART_ENTIMX_SCAN使能的时候,才有用
    .runtime     = 0,  // 函数执行时间
};
