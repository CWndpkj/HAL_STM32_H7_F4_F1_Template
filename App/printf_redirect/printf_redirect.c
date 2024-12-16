/**
 * @file printf_readirect.c
 * @author pkjinfinity (pkjinfinity@outlook.com)
 * @brief 将printf输出重定向到串口输出，实现debug日志打印
 * @version 0.1
 * @date 2024-03-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "sys.h"
#include "usart_helper.h"
//仅对arm-none-eabi-gcc适配

extern error_t usart_debug_write(const u8*data,u32 length);
//重写 _write()方法
int _write( int fd, char* pBuffer, int size )
{
    usart_debug_write( ( const u8* )pBuffer, size );
    return size;
}