#include "sys.h"
#include "delay.h"
#include "iic.h"
#include "oled.h"


int main(void)
{
    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    OLED_Init();              /* OLED����ʼ�� */

    OLED_ShowString16(0, 0, "123asdf");

    while(1)
    {
    }
}
