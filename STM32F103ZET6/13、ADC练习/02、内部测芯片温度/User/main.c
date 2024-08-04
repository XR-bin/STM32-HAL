#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"

int main(void)
{
    double data1=0;
    double data2=0;

    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    USART_Init(115200);                  /* ����1��ʼ�� 115200 */
    ADC1_In16_Init();                    /* ADC1ͨ��16��ʼ�� */

    while(1)
    {
        /* ÿ�յ�����1�����ݾ�ת��һ��ADC���� */
        data1=0, data2=0;
        USART1_Receive_Byte();
        data1 = ADC1_Chip_Temperature1();
        data2 = ADC1_Chip_Temperature2();

        printf("���β�����%f\r\n", data1);
        printf("��������%f\r\n", data2);
    }
}
