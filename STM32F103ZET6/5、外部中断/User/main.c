#include "sys.h"
#include "delay.h"
#include "led.h"
#include "exti.h"

int main(void)
{
    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);         /* 2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();                      /* 滴答定时器初始 */
    LED_Init();                          /* LED驱动初始化 */
    EXTI4_Init();                        /* 外部中断4初始化 */

    while(1)
    {
    }
}
