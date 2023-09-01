#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "rtc.h"

int main(void)
{
    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);         /* 2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();                      /* 滴答定时器初始 */
    USART_Init(115200);                  /* 串口1初始化 115200 */
    RTC_Set_Alarm(2023, 8, 1, 12, 0, 1); /* 设置闹钟 */
    RTC_Init();                          /* 初始化RTC */

    while(1)
    {
    }
}
