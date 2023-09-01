#include "sys.h"
#include "delay.h"
#include "led.h"
#include "exti.h"

int main(void)
{
    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);         /* 2�ŷ��鷽�� 2 λ��ռ���ȼ��� 2 λ��Ӧ���ȼ� */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    LED_Init();                          /* LED������ʼ�� */
    EXTI4_Init();                        /* �ⲿ�ж�4��ʼ�� */

    while(1)
    {
    }
}
