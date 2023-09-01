#ifndef __BEEP_H
#define __BEEP_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"



    /********************   宏定义   ********************/
    /* BEEP引脚定义 */
    #define BEEP_GPIO_PORT                  GPIOB
    #define BEEP_GPIO_PIN                   GPIO_PIN_8
    #define BEEP_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOB_CLK_ENABLE()

    /* BEEP端口控制 */
    #define BEEP(x)   do{ x ? \
                          HAL_GPIO_WritePin(BEEP_GPIO_PORT, BEEP_GPIO_PIN, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(BEEP_GPIO_PORT, BEEP_GPIO_PIN, GPIO_PIN_RESET); \
                      }while(0)

    /* BEEP取反定义 */
    #define BEEP_TOGGLE()   do{ HAL_GPIO_TogglePin(BEEP_GPIO_PORT, BEEP_GPIO_PIN); }while(0)        /* 翻转BEEP */

    /* BEEP开关 */
    #define BEEP_ON  BEEP(1)    /* 开蜂鸣器 */
    #define BEEP_OFF BEEP(0)    /* 关蜂鸣器 */



    /****************    函数外部声明   *****************/
    void BEEP_Init(void);   /* BEEP初始化 */

#endif


