#define USMART_USE_HELP 1 /* 使用帮助，该值设为0，可以节省近700个字节，但是将导致无法显示帮助信息。 */

#define USMART_USE_WRFUNS 1 /* 使用读写函数,使能这里,可以读取任何地址的值,还可以写寄存器的值. */

#define USMART_ENABLE_DATA_RCV 1  // 使能长数据接收模式

#define USMART_PRINTF(...) LOGI("usmart",__VA_ARGS__) /* 定义printf输出 */

#define MAX_FNAME_LEN 30 /* 函数名最大长度，应该设置为不小于最长函数名的长度。 */
#define MAX_PARM 10      /* 最大为10个参数 ,修改此参数,必须修改usmart_exe与之对应. */
#define PARM_LEN 200     /* 所有参数之和的长度不超过PARM_LEN个字节,注意串口接收部分要与之对应(不小于PARM_LEN) */

#define USMART_ENTIMX_SCAN 1
// 使用TIM的定时中断来扫描SCAN函数,如果设置为0,需要自己实现隔一段时间扫描一次scan函数.
// 注意:如果要用runtime统计功能,必须设置USMART_ENTIMX_SCAN为1!!!!
