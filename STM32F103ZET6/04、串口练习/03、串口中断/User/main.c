#include "sys.h"
#include "delay.h"
#include "usart.h"

/***
* ע�⣺
*          HAL��û���ṩIDLE(����)�жϻص��������߰汾��HAL����IDLE��غ�
*      ������û�п����жϻص��������Ͱ汾��HAL���û�У�������Щ�����жϻ�
*      ���������Լ��ֶ���ӡ�
*
*         ��stm32f1xx_hal_uart.c��stm32f1xx_hal_uart.h���������ش��룬
*      ��ӵĴ�������ר�ű�ע������
*/

int main(void)
{
    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);         /* 2�ŷ��鷽�� 2 λ��ռ���ȼ��� 2 λ��Ӧ���ȼ� */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    USART_Init(115200);                  /* ����1��ʼ��  115200 ���ж� */

    while(1)
    {
    }
}
