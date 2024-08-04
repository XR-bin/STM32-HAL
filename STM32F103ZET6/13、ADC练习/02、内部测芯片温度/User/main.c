#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"

int main(void)
{
    double data1=0;
    double data2=0;

    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    SysTick_Init();                      /* 滴答定时器初始 */
    USART_Init(115200);                  /* 串口1初始化 115200 */
    ADC1_In16_Init();                    /* ADC1通道16初始化 */

    while(1)
    {
        /* 每收到串口1的数据就转换一次ADC数据 */
        data1=0, data2=0;
        USART1_Receive_Byte();
        data1 = ADC1_Chip_Temperature1();
        data2 = ADC1_Chip_Temperature2();

        printf("单次采样：%f\r\n", data1);
        printf("过采样：%f\r\n", data2);
    }
}
