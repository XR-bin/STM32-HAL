#include "./BSP/inc/exti.h"

/**********************************************************
*@funcName ：Extix_Init
*@brief    ：外部中断初始化程序
*@param    ：void(无)
*@retval   ：void(无)
*@details  ：
*             KEY0 --- PE4 --- EXTI4
*             KEY1 --- PE3 --- EXTI3
************************************************************/
void Extix_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    KEY_GPIO_CLK_ENABLE();                                  /* KEY0和KEY1时钟使能 */
    
    /* KEY0引脚配置 */
    gpio_init_struct.Pin = KEY0_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* 下升沿触发 */
    gpio_init_struct.Pull = GPIO_PULLUP;                     /* 下拉 */
    HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &gpio_init_struct);    /* KEY0配置为下降沿触发中断 */
    /* KEY0引脚配置 */
    gpio_init_struct.Pin = KEY1_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_RISING;             /* 上升沿触发 */
    HAL_GPIO_Init(KEY1_INT_GPIO_PORT, &gpio_init_struct);    /* KEY1配置为下降沿触发中断 */
    
    /* KEY0中断配置 */
    HAL_NVIC_SetPriority(KEY0_INT_IRQn, 0, 2);               /* 抢占0，子优先级2 */
    HAL_NVIC_EnableIRQ(KEY0_INT_IRQn);                       /* 使能中断线4 */
    /* KEY1中断配置 */
    HAL_NVIC_SetPriority(KEY1_INT_IRQn, 1, 2);               /* 抢占1，子优先级2 */
    HAL_NVIC_EnableIRQ(KEY1_INT_IRQn);                       /* 使能中断线3 */
}



/**********************************************************
*@funcName ：KEY0_INT_IRQHandler(EXTI4_IRQHandler)
*@brief    ：KEY0外部中断服务程序
*@param    ：void(无)
*@retval   ：void(无)
*@details  ：
*             KEY0 --- PE4 --- EXTI4
************************************************************/
void KEY0_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY0_INT_GPIO_PIN);         /* 调用中断处理公用函数 清除KEY0所在中断线 的中断标志位 */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY0_INT_GPIO_PIN);         /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
}



/**********************************************************
*@funcName ：KEY1_INT_IRQHandler(EXTI3_IRQHandler)
*@brief    ：KEY0外部中断服务程序
*@param    ：void(无)
*@retval   ：void(无)
*@details  ：
*             KEY1 --- PE3 --- EXTI3
************************************************************/
void KEY1_INT_IRQHandler(void)
{ 
    HAL_GPIO_EXTI_IRQHandler(KEY1_INT_GPIO_PIN);         /* 调用中断处理公用函数 清除KEY1所在中断线 的中断标志位 */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_INT_GPIO_PIN);         /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
}



/**********************************************************
*@funcName ：HAL_GPIO_EXTI_Callback
*@brief    ：EXTI中断处理回调函数
*@param    ：GPIO_Pin---中断引脚号
*@retval   ：void(无)
*@details  ：
*            HAL_GPIO_EXTI_Callback回调钩子函数会在HAL库的外部中
*        断处理公用函数(HAL_GPIO_EXTI_IRQHandler())中被调用。
*             KEY0 --- PE4 --- EXTI4
*             KEY1 --- PE3 --- EXTI3
************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(20);      /* 消抖 */
    
    switch(GPIO_Pin)
    {
        case KEY0_INT_GPIO_PIN: 
            if(KEY0 == 0)
            {
                LED0_TOGGLE();  /* LED0 状态取反 */ 
                LED1_TOGGLE();  /* LED1 状态取反 */ 
            }
            break;
            
        case KEY1_INT_GPIO_PIN:
            if (KEY1 == 0)
            {
                LED0_TOGGLE();  /* LED0 状态取反 */ 
            }
            break;
            
        default: break;
    }
}

