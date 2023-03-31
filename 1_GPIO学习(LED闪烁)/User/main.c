#include "./SYSTEM/inc/sys.h"
#include "./SYSTEM/inc/delay.h"
#include "./BSP/inc/led.h"

int main(void)
{
    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* ����ʱ��,72M */
    delay_init(72);                             /* ��ʼ����ʱ���� */
    LED_Init();
    
    while(1)
    {
        LED0(0);
        LED1(1);
        delay_ms(500);
        LED0(1);
        LED1(0);
        delay_ms(500);
    }
}
