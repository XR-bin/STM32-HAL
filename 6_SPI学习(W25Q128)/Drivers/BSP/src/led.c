#include "./BSP/inc/led.h"

/**********************************************************
*@funcName ��LED_Init
*@brief    ��ʹ�����ʱ�ӣ�����ʼ��LED�����IO
*@param    ��void(��)
*@retval   ��void(��)
*@details  ��
*             LED0 --- PB5
*             LED1 --- PE5
************************************************************/
void LED_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    LED_GPIO_CLK_ENABLE();                                  /* LED0��LED1ʱ��ʹ�� */
    
    /* LED0�������� */
    gpio_init_struct.Pin = LED0_GPIO_PIN;                   /* LED0���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(LED0_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��LED0���� */
    
    /* LED1�������� */
    gpio_init_struct.Pin = LED1_GPIO_PIN;                   /* LED1���� */
    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��LED1���� */
    
    LED0(1);                                                /* �ر� LED0 */
    LED1(1);                                                /* �ر� LED1 */
}
