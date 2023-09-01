#include "sys.h"
#include "delay.h"
#include "timer.h"
#include "pwm.h"

int main(void)
{
    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);         /* 2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();                      /* 滴答定时器初始 */
    TIM6_Init(500,7200);             /* 500ms 定时中断 */
    TIM3_PWM_Init(1000, 72, 500);    /* 50%占空比 */

    while(1)
    {
    }
}
