#ifndef __ADC_H
#define __ADC_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "usart.h"



    /********************   宏定义   ********************/
    /* ADC3引脚 */
    #define ADC3_CHY_GPIO_PORT                  GPIOA
    #define ADC3_CHY_GPIO_PIN                   GPIO_PIN_1 
    #define ADC3_CHY_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE()  /* PA口时钟使能 */
    /* ADC_x */
    #define ADC_ADCX                            ADC3 
    #define ADC3_CHY                            ADC_CHANNEL_6                 /* 通道Y,  0 <= Y <= 17 */ 
    #define ADC3_CHY_CLK_ENABLE()               __HAL_RCC_ADC3_CLK_ENABLE()   /* ADC1 时钟使能 */



    /****************    变量外部声明   *****************/
    extern ADC_HandleTypeDef g_adc3_handle;         /* ADC句柄 */



    /****************    函数外部声明   *****************/
    void ADC3_In6_Init(void);     /* ADC3初始化(带中断) */

#endif
