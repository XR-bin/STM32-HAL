#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"

int main(void)
{
    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    SysTick_Init();                      /* 滴答定时器初始 */
    USART_Init(115200);                  /* 串口初始化 115200 */
    LCD_ILI9341_Init();                  /* ILI9341的LCD屏初始化 */

    delay_ms(50);

    /* 能识别的型号：ST7789、ILI9341、NT35310、NT35510、SSD1963 */
    LCD_Recognition_Id();

    while(1)
    {
    }
}
