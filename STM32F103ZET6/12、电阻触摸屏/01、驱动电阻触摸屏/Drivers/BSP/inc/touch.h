#ifndef __TOUCH_H
#define __TOUCH_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"



    /********************   宏定义   ********************/
    /* 电阻触摸屏驱动IC T_PEN/T_CS/T_MISO/T_MOSI/T_SCK 引脚 定义 */
    /* T_PEN引脚 */
    #define T_PEN_GPIO_PORT                 GPIOF
    #define T_PEN_GPIO_PIN                  GPIO_PIN_10
    #define T_PEN_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOF_CLK_ENABLE()    /* 所在IO口时钟使能 */
    /* T_CS引脚 */
    #define T_CS_GPIO_PORT                  GPIOF
    #define T_CS_GPIO_PIN                   GPIO_PIN_11
    #define T_CS_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOF_CLK_ENABLE()    /* 所在IO口时钟使能 */
    /* T_MISO引脚 */
    #define T_MISO_GPIO_PORT                GPIOB
    #define T_MISO_GPIO_PIN                 GPIO_PIN_2
    #define T_MISO_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()    /* 所在IO口时钟使能 */
    /* T_MOSI引脚 */
    #define T_MOSI_GPIO_PORT                GPIOF
    #define T_MOSI_GPIO_PIN                 GPIO_PIN_9
    #define T_MOSI_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOF_CLK_ENABLE()    /* 所在IO口时钟使能 */
    /* T_SCK引脚 */
    #define T_SCK_GPIO_PORT                 GPIOB
    #define T_SCK_GPIO_PIN                  GPIO_PIN_1
    #define T_SCK_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()    /* 所在IO口时钟使能 */

    /* 片选线 */
    #define T_CS_H    HAL_GPIO_WritePin(T_CS_GPIO_PORT, T_CS_GPIO_PIN, GPIO_PIN_SET)   /* 片选失能 */
    #define T_CS_L    HAL_GPIO_WritePin(T_CS_GPIO_PORT, T_CS_GPIO_PIN, GPIO_PIN_RESET) /* 片选使能 */
    /* 时钟线 */
    #define T_SCK_H   HAL_GPIO_WritePin(T_SCK_GPIO_PORT, T_SCK_GPIO_PIN, GPIO_PIN_SET)
    #define T_SCK_L   HAL_GPIO_WritePin(T_SCK_GPIO_PORT, T_SCK_GPIO_PIN, GPIO_PIN_RESET)
    /* 输出线 */
    #define T_MOSI_H  HAL_GPIO_WritePin(T_MOSI_GPIO_PORT, T_MOSI_GPIO_PIN, GPIO_PIN_SET)
    #define T_MOSI_L  HAL_GPIO_WritePin(T_MOSI_GPIO_PORT, T_MOSI_GPIO_PIN, GPIO_PIN_RESET)
    /* 输入线 */
    #define T_MISO    HAL_GPIO_ReadPin(T_MISO_GPIO_PORT, T_MISO_GPIO_PIN)
    /* 笔接触线 */
    #define T_PEN     HAL_GPIO_ReadPin(T_PEN_GPIO_PORT, T_PEN_GPIO_PIN)

    /* 指令 */
    #define TOUCH_X   0xD0    //获取x坐标数据
    #define TOUCH_Y   0x90    //获取y坐标数据



    /****************    函数外部声明   *****************/
    void Touch_IO_Init(void);                /* 电阻触摸屏引脚初始化 */
    uint16_t Touch_Data(uint8_t command);    /* 向电阻触摸屏发送数据并接收电阻触摸屏发来的数据 */

#endif







