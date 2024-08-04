#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"

int main(void)
{
    uint32_t rgb=0;

    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    SysTick_Init();                      /* 滴答定时器初始 */
    USART_Init(115200);                  /* 串口1初始化   115200 */
    LCD_ILI9341_Init();                  /* ILI9341的LCD屏初始化 */

    delay_ms(50);

    LCD_ILI9341_ClearXY(100, 100, 100, 100, RED);    /* 画一个红色窗体 */
    rgb = LCD_ReadPoint(101, 101);                   /* 取红色窗体里的一个像素点的颜色数据 */
    printf("rgb：%d    %x", rgb, rgb);

    while(1)
    {
    }
}
