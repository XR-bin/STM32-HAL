#include "exti.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\exti.c
*              .\Drivers\BSP\inc\exti.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-06-16
* @brief     ���ⲿ�жϴ���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��EXTI4_Init
* @brief    ���ⲿ�жϳ�ʼ��
* @param    ��void
* @retval   ��void
* @details  ��
*            KEY0--------PE4 �͵�ƽ���£��ߵ�ƽ̧��
* @fn       ��
************************************************************/
void EXTI4_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;                       /* GPIO���ýṹ�� */

    KEY0_INT_GPIO_CLK_ENABLE();                              /* KEY0ʱ��ʹ�� */
    
    gpio_init_struct.Pin  = KEY0_INT_GPIO_PIN;               /* KEY0���� */
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* �����ش��� */
    gpio_init_struct.Pull = GPIO_PULLUP;                     /* ���� */
    HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &gpio_init_struct);    /* KEY0����Ϊ�½��ش����ж� */

    HAL_NVIC_SetPriority(KEY0_INT_IRQn, 0, 0);               /* ��ռ0�������ȼ�0 */
    HAL_NVIC_EnableIRQ(KEY0_INT_IRQn);                       /* ʹ���ж���4 */

    __HAL_GPIO_EXTI_CLEAR_IT(KEY0_INT_GPIO_PIN);             /* ����ⲿ�ж�4��־ */
}



/**********************************************************
* @funcName ��KEY0_INT_IRQHandler
* @brief    ��KEY0�ⲿ�жϷ�����
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void KEY0_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY0_INT_GPIO_PIN);         /* �����жϴ����ú��� ���KEY0�����ж��� ���жϱ�־λ */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY0_INT_GPIO_PIN);         /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}



/**********************************************************
* @funcName ��HAL_GPIO_EXTI_Callback
* @brief    ���ⲿ�жϻص�����
* @param    ��uint16_t GPIO_Pin
* @retval   ��void
* @details  ��
*            KEY0--------PE4 �͵�ƽ���£��ߵ�ƽ̧��
* @fn       ��
************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(20);      /* ���� */

    switch(GPIO_Pin)
    {
        case KEY0_INT_GPIO_PIN:
            if((GPIOE->IDR & (1<<4)) == 0) GPIOB->ODR ^= (1<<5);     /* LED0��ת */
    }
}












