#pragma once
#include "sys.h"
#include "error.h"

error_t usart_write( const u8* data, u32 length );