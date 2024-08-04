#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "norflash.h"

int main(void)
{
    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    SysTick_Init();                      /* 滴答定时器初始 */
    USART_Init(115200);                  /* 串口初始化  115200 */
    NorFlash_Init();                     /* W25Q128初始化 */

    NorFlash_Read_ID();


    while(1)
    {
    }
}
