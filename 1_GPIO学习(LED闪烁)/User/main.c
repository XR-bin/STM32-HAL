#include "./SYSTEM/inc/sys.h"
#include "./SYSTEM/inc/delay.h"
#include "./BSP/inc/led.h"

int main(void)
{
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* 设置时钟,72M */
    delay_init(72);                             /* 初始化延时函数 */
    LED_Init();
    
    while(1)
    {
        LED0(0);
        LED1(1);
        delay_ms(500);
        LED0(1);
        LED1(0);
        delay_ms(500);
    }
}
