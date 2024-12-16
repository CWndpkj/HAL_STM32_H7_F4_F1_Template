#pragma once
#include "sys.h"

void key_init();

// 用于轮询模式获取按键的状态
uint8_t key_get_state();