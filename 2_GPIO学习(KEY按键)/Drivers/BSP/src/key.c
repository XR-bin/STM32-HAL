#include "./BSP/inc/key.h"

/**********************************************************
*@funcName ：Key_Init
*@brief    ：使能相关时钟，并初始化KEY的相关IO
*@param    ：void(无)
*@retval   ：void(无)
*@details  ：
*             KEY0   --- PE4
*             KEY1   --- PE3
*             KEY_UP --- PA0
************************************************************/
void Key_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    KEY_GPIO_CLK_ENABLE();                                  /* KEY0和KEY1、KEY_UP时钟使能 */
    
    /* KEY0引脚配置 */
    gpio_init_struct.Pin = KEY0_GPIO_PIN;                   /* KEY0引脚 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                /* 输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    HAL_GPIO_Init(KEY0_GPIO_PORT, &gpio_init_struct);       /* 初始化KEY0引脚 */
    /* KEY1引脚配置 */
    gpio_init_struct.Pin = KEY1_GPIO_PIN;                   /* KEY1引脚 */
    HAL_GPIO_Init(KEY1_GPIO_PORT, &gpio_init_struct);       /* 初始化KEY1引脚 */
    /* KEY_UP引脚配置 */
    gpio_init_struct.Pin = KEYUP_GPIO_PIN;                   /* KEY_UP引脚 */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                   /* 下拉 */
    HAL_GPIO_Init(KEYUP_GPIO_PORT, &gpio_init_struct);       /* 初始化KEY_UP引脚 */
}



/***************************************************************************
*@funcName ：Key_Scan
*@brief    ：按键扫描函数
*@param    ：mode(是否支持连续按)
*   @arg  mode == 0,  不支持连续按(当按键按下不放时, 只有第一次调用会返回键值,
*                     必须松开以后, 再次按下才会返回其他键值)
*   @arg  mode == 1,  支持连续按(当按键按下不放时, 每次调用该函数都会返回键值)
*@retval   ：键值, 定义如下:
*            KEY0_PRES, 1, KEY0按下
*            KEY1_PRES, 2, KEY1按下
*            WKUP_PRES, 3, WKUP按下
*@details  ：
*             KEY0   --- PE4
*             KEY1   --- PE3
*             KEY_UP --- PA0
****************************************************************************/
uint8_t Key_Scan(uint8_t mode)
{
    static uint8_t key_up = 1;  /* 按键按松开标志 */
    uint8_t keyval = 0;

    if (mode) key_up = 1;       /* 支持连按 */

    if (key_up && (KEY0 == 0 || KEY1 == 0 || KEY_UP == 1))  /* 按键松开标志为1, 且有任意一个按键按下了 */
    {
        delay_ms(10);           /* 去抖动 */
        key_up = 0;

        if (KEY0 == 0)  keyval = KEY0_PRES;

        if (KEY1 == 0)  keyval = KEY1_PRES;

        if (KEY_UP == 1) keyval = WKUP_PRES;
    }
    else if (KEY0 == 1 && KEY1 == 1 && KEY_UP == 0) /* 没有任何按键按下, 标记按键松开 */
    {
        key_up = 1;
    }

    return keyval;              /* 返回键值 */
}

