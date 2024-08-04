#ifndef __EXTI_H
#define __EXTI_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   宏定义   ********************/
    /* 引脚 和 中断编号 & 中断服务函数 定义 */ 
    #define KEY0_INT_GPIO_PORT              GPIOE
    #define KEY0_INT_GPIO_PIN               GPIO_PIN_4
    #define KEY0_INT_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */
    #define KEY0_INT_IRQn                   EXTI4_IRQn
    #define KEY0_INT_IRQHandler             EXTI4_IRQHandler



    /****************    函数外部声明   *****************/
    void EXTI4_Init(void);      /* 外部中断4初始化 */

#endif









