/**
 * @file log.h
 * @author pkjinfinity (pkjinfinity@outlook.com)
 * @brief 日志输出的简单实现，对日志的输出分为三个等级：info,warning,error
 *         对应的打印颜色为：绿色，黄色，红色
 *         日志输出示例：LOGI("info","this is a info type log,num:%d,string:%s",123,"hello world");
 * @version 0.1
 * @date 2024-03-25
 *
 * @copyright Copyright (c) 2024
 * @note 需要先实现printf的重定向
 */

#pragma once
#include <stdio.h>

void LOGD( const char* tag, const char* format, ... );
void LOGI( const char* tag, const char* format, ... );
void LOGW( const char* tag, const char* format, ... );
void LOGE( const char* tag, const char* format, ... );