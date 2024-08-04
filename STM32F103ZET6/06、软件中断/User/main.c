#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "exti.h"

int main(void)
{
    uint8_t key_flay;

    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);         /* 2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();                      /* 滴答定时器初始 */
    LED_Init();                          /* LED初始化 */
    KEY_Init();                          /* KEY初始化 */
    EXTI5_Swier_Init();                  /* 软件外部中断初始化 */

    while(1)
    {
        key_flay = KEY_Scan();
        if(key_flay == 2)
        {
            EXTI->SWIER |= (1<<5);     /* 触发软件中断 */
        }
    }
}
