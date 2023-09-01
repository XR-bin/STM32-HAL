#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"

int main(void)
{
    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);         /* 2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();                      /* 滴答定时器初始 */
    USART_Init(115200);                  /* 串口1初始化 115200 */
    ADC3_In6_Init();                     /* ADC3通道6初始化 */

    while(1)
    {
        /* 每收到串口1的数据就转换一次ADC数据 */
        USART1_Receive_Byte();
        /* 开启规则通道转换开关 */
        HAL_ADC_Start_IT(&g_adc3_handle);
    }
}
