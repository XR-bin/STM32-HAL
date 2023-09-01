#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "rtc.h"

int main(void)
{
    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);         /* 2�ŷ��鷽�� 2 λ��ռ���ȼ��� 2 λ��Ӧ���ȼ� */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    USART_Init(115200);                  /* ����1��ʼ�� 115200 */
    RTC_Set_Alarm(2023, 8, 1, 12, 0, 1); /* �������� */
    RTC_Init();                          /* ��ʼ��RTC */

    while(1)
    {
    }
}
