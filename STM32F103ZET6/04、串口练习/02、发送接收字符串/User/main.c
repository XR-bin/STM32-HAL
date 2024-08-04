#include "sys.h"
#include "delay.h"
#include "usart.h"

int main(void)
{
    uint8_t str[200];

    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    SysTick_Init();                      /* 滴答定时器初始 */
    USART_Init(115200);                  /* 串口1初始化  115200 */

    while(1)
    {
        USART1_Receive_Str(str);
        USART1_Send_Str(str);
        printf("\r\n");
        printf("%s\r\n",str);
    }
}
