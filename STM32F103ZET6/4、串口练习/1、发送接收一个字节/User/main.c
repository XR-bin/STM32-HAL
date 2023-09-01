#include "sys.h"
#include "delay.h"
#include "usart.h"

int main(void)
{
    uint8_t data;

    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    SysTick_Init();                      /* 滴答定时器初始 */
    USART_Init(115200);                  /* 串口1初始化  115200 */

    while(1)
    {
        data = USART1_Receive_Byte();
        USART1_Send_Byte(data);
    }
}
