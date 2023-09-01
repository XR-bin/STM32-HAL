#include "sys.h"
#include "delay.h"
#include "led.h"
#include "timer.h"

int main(void)
{
    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);         /* 2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();                      /* 滴答定时器初始 */
    LED_Init();                          /* LED初始化 */
    TIM6_Init(4999, 7200);               /* 定时器6初始化  500ms定时 */

    while(1)
    {
        LED0_ON;
        delay_ms(500);
        LED0_OFF;
        delay_ms(500);
    }
}
