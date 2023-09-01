#include "sys.h"
#include "delay.h"
#include "led.h"

int main(void)
{
    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    SysTick_Init();                      /* 滴答定时器初始 */
    LED_Init();                          /* LED驱动初始化 */

    while(1)
    {
        LED0_ON;
        LED1_ON;
        delay_ms(600);
        LED0_OFF;
        LED1_OFF;
        delay_ms(600);
    }
}
