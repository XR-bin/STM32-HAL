#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"

int main(void)
{
    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);         /* 2�ŷ��鷽�� 2 λ��ռ���ȼ��� 2 λ��Ӧ���ȼ� */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    USART_Init(115200);                  /* ����1��ʼ�� 115200 */
    ADC3_In6_Init();                     /* ADC3ͨ��6��ʼ�� */

    while(1)
    {
        /* ÿ�յ�����1�����ݾ�ת��һ��ADC���� */
        USART1_Receive_Byte();
        /* ��������ͨ��ת������ */
        HAL_ADC_Start_IT(&g_adc3_handle);
    }
}
