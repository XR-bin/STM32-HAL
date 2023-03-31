#include "./BSP/inc/exti.h"

/**********************************************************
*@funcName ��Extix_Init
*@brief    ���ⲿ�жϳ�ʼ������
*@param    ��void(��)
*@retval   ��void(��)
*@details  ��
*             KEY0 --- PE4 --- EXTI4
*             KEY1 --- PE3 --- EXTI3
************************************************************/
void Extix_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    KEY_GPIO_CLK_ENABLE();                                  /* KEY0��KEY1ʱ��ʹ�� */
    
    /* KEY0�������� */
    gpio_init_struct.Pin = KEY0_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* �����ش��� */
    gpio_init_struct.Pull = GPIO_PULLUP;                     /* ���� */
    HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &gpio_init_struct);    /* KEY0����Ϊ�½��ش����ж� */
    /* KEY0�������� */
    gpio_init_struct.Pin = KEY1_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_RISING;             /* �����ش��� */
    HAL_GPIO_Init(KEY1_INT_GPIO_PORT, &gpio_init_struct);    /* KEY1����Ϊ�½��ش����ж� */
    
    /* KEY0�ж����� */
    HAL_NVIC_SetPriority(KEY0_INT_IRQn, 0, 2);               /* ��ռ0�������ȼ�2 */
    HAL_NVIC_EnableIRQ(KEY0_INT_IRQn);                       /* ʹ���ж���4 */
    /* KEY1�ж����� */
    HAL_NVIC_SetPriority(KEY1_INT_IRQn, 1, 2);               /* ��ռ1�������ȼ�2 */
    HAL_NVIC_EnableIRQ(KEY1_INT_IRQn);                       /* ʹ���ж���3 */
}



/**********************************************************
*@funcName ��KEY0_INT_IRQHandler(EXTI4_IRQHandler)
*@brief    ��KEY0�ⲿ�жϷ������
*@param    ��void(��)
*@retval   ��void(��)
*@details  ��
*             KEY0 --- PE4 --- EXTI4
************************************************************/
void KEY0_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY0_INT_GPIO_PIN);         /* �����жϴ����ú��� ���KEY0�����ж��� ���жϱ�־λ */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY0_INT_GPIO_PIN);         /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}



/**********************************************************
*@funcName ��KEY1_INT_IRQHandler(EXTI3_IRQHandler)
*@brief    ��KEY0�ⲿ�жϷ������
*@param    ��void(��)
*@retval   ��void(��)
*@details  ��
*             KEY1 --- PE3 --- EXTI3
************************************************************/
void KEY1_INT_IRQHandler(void)
{ 
    HAL_GPIO_EXTI_IRQHandler(KEY1_INT_GPIO_PIN);         /* �����жϴ����ú��� ���KEY1�����ж��� ���жϱ�־λ */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_INT_GPIO_PIN);         /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
}



/**********************************************************
*@funcName ��HAL_GPIO_EXTI_Callback
*@brief    ��EXTI�жϴ���ص�����
*@param    ��GPIO_Pin---�ж����ź�
*@retval   ��void(��)
*@details  ��
*            HAL_GPIO_EXTI_Callback�ص����Ӻ�������HAL����ⲿ��
*        �ϴ����ú���(HAL_GPIO_EXTI_IRQHandler())�б����á�
*             KEY0 --- PE4 --- EXTI4
*             KEY1 --- PE3 --- EXTI3
************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(20);      /* ���� */
    
    switch(GPIO_Pin)
    {
        case KEY0_INT_GPIO_PIN: 
            if(KEY0 == 0)
            {
                LED0_TOGGLE();  /* LED0 ״̬ȡ�� */ 
                LED1_TOGGLE();  /* LED1 ״̬ȡ�� */ 
            }
            break;
            
        case KEY1_INT_GPIO_PIN:
            if (KEY1 == 0)
            {
                LED0_TOGGLE();  /* LED0 ״̬ȡ�� */ 
            }
            break;
            
        default: break;
    }
}

