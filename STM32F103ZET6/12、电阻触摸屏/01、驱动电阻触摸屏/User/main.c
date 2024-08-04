#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "touch.h"

int main(void)
{
    uint16_t x=0,y=0;

    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    SysTick_Init();                      /* 滴答定时器初始 */
    USART_Init(115200);                  /* 串口初始化 115200 */
    Touch_IO_Init();                     /* 电阻触摸屏引脚初始化 */

    while(1)
    {
        if(T_PEN == 0)
        {
            x = Touch_Data(TOUCH_X);          /* 获取触点x坐标 */
            y = Touch_Data(TOUCH_Y);          /* 获取触点y坐标 */
            printf("x:%4d   y:%4d\r\n",x,y);
        }
    }
}
