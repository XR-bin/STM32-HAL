#include "./SYSTEM/inc/sys.h"
#include "./SYSTEM/inc/delay.h"
#include "./BSP/inc/led.h"
#include "./BSP/inc/exti.h"

int main(void)
{
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* 设置时钟,72M */
    delay_init(72);                             /* 初始化延时函数 */
    LED_Init();
    Extix_Init();
    
    while(1)
    {
    }
}
