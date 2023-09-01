#ifndef __KEY_H
#define __KEY_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   宏定义   ********************/
    /* 引脚定义 */
    /* KEY0 */
    #define KEY0_GPIO_PORT                  GPIOE
    #define KEY0_GPIO_PIN                   GPIO_PIN_4
    #define KEY0_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()   /* PE口时钟使能 */

    /* KEY1 */
    #define KEY1_GPIO_PORT                  GPIOE
    #define KEY1_GPIO_PIN                   GPIO_PIN_3
    #define KEY1_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOE_CLK_ENABLE()   /* PE口时钟使能 */

    /* KEY_UP */
    #define KEYUP_GPIO_PORT                 GPIOA
    #define KEYUP_GPIO_PIN                  GPIO_PIN_0
    #define KEYUP_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()   /* PA口时钟使能 */

    /* 获取按键状态 */
    #define KEY0    HAL_GPIO_ReadPin(KEY0_GPIO_PORT, KEY0_GPIO_PIN)       /* 低电平按下 */
    #define KEY1    HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN)       /* 低电平按下 */
    #define KEY_UP  HAL_GPIO_ReadPin(KEYUP_GPIO_PORT, KEYUP_GPIO_PIN)     /* 高电平按下 */



    /****************    函数外部声明   *****************/
    void KEY_Init(void);
    uint8_t KEY_Scan(void);

#endif








