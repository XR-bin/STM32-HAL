#ifndef __TOUCH_H
#define __TOUCH_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"



    /****************     结构体定义     ****************/
    /* 用于电阻触摸屏x和y坐标读取 */
    typedef struct touch
    {
        uint16_t x;     /* 触点x坐标 */
        uint16_t y;     /* 触点y坐标 */
    }TOUCH_XY;

    /* 触摸屏控制器 */
    typedef struct
    {
        void (*init)(void);      /* 初始化触摸屏控制器 */
        void (*adjust)(void);       /* 触摸屏校准 */
        TOUCH_XY (*scan)(uint8_t);   /* 扫描触摸屏.0,屏幕扫描;1,物理坐标; */

        uint8_t comX;    /* 获取x坐标数据指令 */
        uint8_t comY;    /* 获取y坐标数据指令 */

        /* 5点校准触摸屏校准参数(电容屏不需要校准) */
        float xfac;                 /* LCD和触摸屏x坐标的比例系数 */
        float yfac;                 /* LCD和触摸屏y坐标的比例系数 */
        short xc;                   /* LCD和触摸屏x坐标的偏移量 */
        short yc;                   /* LCD和触摸屏x坐标的偏移量 */

        /* 新增的参数,当触摸屏的左右上下完全颠倒时需要用到.
         * b0:0, 竖屏(适合左右为X坐标,上下为Y坐标的TP)
         *    1, 横屏(适合左右为Y坐标,上下为X坐标的TP)
         * b1~6: 保留.
         * b7:0, 电阻屏
         *    1, 电容屏
         */
        uint8_t touchtype;
    } _m_tp_dev;



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
    
    /* 电阻触摸驱动芯片 数据采集 滤波用参数 */
    #define TP_READ_TIMES   5       /* 读取次数 */
    #define TP_LOST_VAL     1       /* 丢弃值(要丢弃的最大最小值个数) */
    
    /* 连续两次读取X,Y坐标的数据误差最大允许值 */
    #define TP_ERR_RANGE    50         /* 误差范围 */

    /* 校准误差允许范围 */
    #define CORRECT_RANGE_MAX    1.03    /* 误差范围 */
    #define CORRECT_RANGE_MIN    0.97    /* 误差范围 */



    /****************    变量外部声明   *****************/
    extern _m_tp_dev tp_dev;        /* 触屏控制器在touch.c里面定义 */



    /****************    函数外部声明   *****************/
    void Touch_IO_Init(void);                /* 电阻触摸屏引脚初始化 */
    uint16_t Touch_Data(uint8_t command);    /* 向电阻触摸屏发送数据并接收电阻触摸屏发来的数据 */
    TOUCH_XY Touch_Smoothing(void);          /* 电阻触摸屏数据滤波函数 */
    void LCD_Calibration_Cross(uint16_t x, uint16_t y, uint16_t color); /* 用于校准的十字提示函数 */
    void LCD_Touch_Calibration(void);        /* LCD与触摸屏建立联系，屏幕校准 */
    uint8_t Touch_Read_XY(TOUCH_XY* coord);  /* 读取电阻触摸屏触摸物理坐标 */
    TOUCH_XY Touch_Scan(uint8_t mode);       /* 读取电阻触摸屏触摸坐标(物理坐标或屏幕坐标) */
    void Touch_Init(void);                   /* 触摸屏初始化 */

#endif







