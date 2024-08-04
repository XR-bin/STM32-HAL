#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "touch.h"

int main(void)
{
    TOUCH_XY tmp;

    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    SysTick_Init();                      /* 滴答定时器初始 */
    USART_Init(115200);                  /* 串口1初始化 115200 */
    LCD_ILI9341_Init();                  /* ILI9341的LCD屏初始化 */
    tp_dev.init();                       /* 触摸屏初始化 */
    tp_dev.adjust();                     /* 屏幕校准 */

    while(1)
    {
        if(T_PEN == 0)
        {
            tmp = tp_dev.scan(1);
            printf("x:%d   y:%d\r\n",tmp.x, tmp.y);
        }
    }
}
