#include "sys.h"
#include "delay.h"
#include "usart.h"

/***
* 注意：
*          HAL库没有提供IDLE(空闲)中断回调函数，高版本的HAL库有IDLE相关函
*      数，但没有空闲中断回调函数，低版本的HAL库更没有，所以这些空闲中断回
*      调函数得自己手动添加。
*
*         在stm32f1xx_hal_uart.c和stm32f1xx_hal_uart.h进行添加相关代码，
*      添加的代码我有专门标注出来。
*/

int main(void)
{
    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);         /* 2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();                      /* 滴答定时器初始 */
    USART_Init(115200);                  /* 串口1初始化  115200 带中断 */

    while(1)
    {
    }
}
