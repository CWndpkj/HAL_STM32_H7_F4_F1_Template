

#include "log.h"
#include <stdarg.h>
void LOGD( const char* tag, const char* format, ... )
{
    printf( "\033[30m[%s]:", tag );
    va_list args;
    va_start( args, format );
    vprintf( format, args );
    va_end( args );
    printf( "\033[0m\r\n" );
}
void LOGI( const char* tag, const char* format, ... )
{
    printf( "\033[32m[%s]:", tag );
    va_list args;
    va_start( args, format );
    vprintf( format, args );
    va_end( args );
    printf( "\033[0m\r\n" );
}
void LOGW( const char* tag, const char* format, ... )
{
    printf( "\033[33m[%s]:", tag );
    va_list args;
    va_start( args, format );
    vprintf( format, args );
    va_end( args );
    printf( "\033[0m\r\n" );
}
void LOGE( const char* tag, const char* format, ... )
{
    printf( "\033[31m[%s]:", tag );
    va_list args;
    va_start( args, format );
    vprintf( format, args );
    va_end( args );
    printf( "\033[0m\r\n" );
}