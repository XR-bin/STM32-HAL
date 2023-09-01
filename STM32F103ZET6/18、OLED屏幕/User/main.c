#include "sys.h"
#include "delay.h"
#include "iic.h"
#include "oled.h"


int main(void)
{
    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    SysTick_Init();                      /* 滴答定时器初始 */
    OLED_Init();              /* OLED屏初始化 */

    OLED_ShowString16(0, 0, "123asdf");

    while(1)
    {
    }
}
