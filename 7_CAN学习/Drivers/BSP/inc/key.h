#ifndef _KEY_H
#define _KEY_H

    /****************   外部头文件声明   ****************/
    #include "./SYSTEM/inc/sys.h"
    #include "./SYSTEM/inc/delay.h"
    
    
    
    /********************   宏定义   ********************/
    /* KEY相关时钟使能 */
    #define KEY_GPIO_CLK_ENABLE()          do{\
                                               __HAL_RCC_GPIOA_CLK_ENABLE();\
                                               __HAL_RCC_GPIOE_CLK_ENABLE();\
                                           }while(0)
                                           
    /* KEY0引脚定义 */
    #define KEY0_GPIO_PORT                  GPIOE
    #define KEY0_GPIO_PIN                   GPIO_PIN_4
    /* KEY1引脚定义 */
    #define KEY1_GPIO_PORT                  GPIOE
    #define KEY1_GPIO_PIN                   GPIO_PIN_3
    /* KEY_UP引脚定义 */
    #define KEYUP_GPIO_PORT                 GPIOA
    #define KEYUP_GPIO_PIN                  GPIO_PIN_0
    
    /* 读取按键电平 */
    #define KEY0        HAL_GPIO_ReadPin(KEY0_GPIO_PORT, KEY0_GPIO_PIN)     /* 读取KEY0引脚 */
    #define KEY1        HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN)     /* 读取KEY1引脚 */
    #define KEY_UP      HAL_GPIO_ReadPin(KEYUP_GPIO_PORT, KEYUP_GPIO_PIN)   /* 读取KEY1引脚 */
    
    /* 按键标识 */
    #define KEY0_PRES    1              /* KEY0按下 */
    #define KEY1_PRES    2              /* KEY1按下 */
    #define WKUP_PRES    3              /* KEY_UP按下(即WK_UP) */
    
    
    
    /****************    函数外部声明   ****************/
    void Key_Init(void);
    uint8_t Key_Scan(uint8_t mode);

#endif

