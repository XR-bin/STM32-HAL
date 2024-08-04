#include "sys.h"
#include "delay.h"
#include "led.h"

int main(void)
{
    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    LED_Init();                          /* LED������ʼ�� */

    while(1)
    {
        LED0_ON;
        LED1_ON;
        delay_ms(600);
        LED0_OFF;
        LED1_OFF;
        delay_ms(600);
    }
}
