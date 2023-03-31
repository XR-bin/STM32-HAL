#include "./BSP/inc/led.h"

/**********************************************************
*@funcName ：LED_Init
*@brief    ：使能相关时钟，并初始化LED的相关IO
*@param    ：void(无)
*@retval   ：void(无)
*@details  ：
*             LED0 --- PB5
*             LED1 --- PE5
************************************************************/
void LED_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    LED_GPIO_CLK_ENABLE();                                  /* LED0和LED1时钟使能 */
    
    /* LED0引脚配置 */
    gpio_init_struct.Pin = LED0_GPIO_PIN;                   /* LED0引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(LED0_GPIO_PORT, &gpio_init_struct);       /* 初始化LED0引脚 */
    
    /* LED1引脚配置 */
    gpio_init_struct.Pin = LED1_GPIO_PIN;                   /* LED1引脚 */
    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct);       /* 初始化LED1引脚 */
    
    LED0(1);                                                /* 关闭 LED0 */
    LED1(1);                                                /* 关闭 LED1 */
}
