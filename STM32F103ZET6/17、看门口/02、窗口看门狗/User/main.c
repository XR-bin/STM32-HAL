#include "sys.h"
#include "delay.h"
#include "led.h"
#include "wwdg.h"

int main(void)
{
    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    SysTick_Init();                      /* ��ʱ��ʼ�� */
    LED_Init();                          /* LED��ʼ�� */
    LED0_ON;                             /* LED0�� */
    delay_ms(300);                       /* ����300ms */
    WWDG_Init(0x7f, 0x5f, 3);            /* ������ֵΪ7f,���ڼĴ���Ϊ5f,��Ƶ��Ϊ8 */

    while(1)
    {
        LED0_OFF;                        /* LED0�� */
    }
}

