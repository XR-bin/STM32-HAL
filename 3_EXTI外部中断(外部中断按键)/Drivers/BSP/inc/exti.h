#ifndef _EXTI_H
#define _EXTI_H

    /****************   外部头文件声明   ****************/
    #include "./SYSTEM/inc/sys.h"
    #include "./SYSTEM/inc/delay.h"
    #include "./BSP/inc/led.h"
    
    
    
    /********************   宏定义   ********************/
    /* LED相关时钟使能 */
    #define KEY_GPIO_CLK_ENABLE()          do{__HAL_RCC_GPIOE_CLK_ENABLE();}while(0)
    
    /* KEY0引脚定义 */
    #define KEY0_INT_GPIO_PORT              GPIOE
    #define KEY0_INT_GPIO_PIN               GPIO_PIN_4
    /* KEY1引脚定义 */
    #define KEY1_INT_GPIO_PORT              GPIOE
    #define KEY1_INT_GPIO_PIN               GPIO_PIN_3
    
    /* 中断编号 & 中断服务函数 */
    #define KEY0_INT_IRQn                   EXTI4_IRQn
    #define KEY0_INT_IRQHandler             EXTI4_IRQHandler
    #define KEY1_INT_IRQn                   EXTI3_IRQn
    #define KEY1_INT_IRQHandler             EXTI3_IRQHandler
    
    /* 读取按键电平 */
    #define KEY0        HAL_GPIO_ReadPin(KEY0_INT_GPIO_PORT, KEY0_INT_GPIO_PIN)     /* 读取KEY0引脚 */
    #define KEY1        HAL_GPIO_ReadPin(KEY1_INT_GPIO_PORT, KEY1_INT_GPIO_PIN)     /* 读取KEY1引脚 */
    
    
    
    /******************   函数外部声明   ******************/
    void Extix_Init(void);

#endif

