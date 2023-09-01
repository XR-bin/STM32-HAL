#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"

int main(void)
{
    uint16_t data1=0;
    uint16_t data2=0;

    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    USART_Init(115200);                  /* ����1��ʼ�� 115200 */
    ADC3_In6_Init();                     /* ADC3ͨ��6��ʼ�� */

    while(1)
    {
        /* ÿ�յ�����1�����ݾ�ת��һ��ADC���� */
        data1=0, data2=0;
        USART1_Receive_Byte();
        data1 = ADC3_In6_Data();
        data2 = Oversampling();

        printf("���β�����%d\r\n", data1);
        printf("��������%d\r\n", data2);
    }
}
