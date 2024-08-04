#include "sys.h"
#include "delay.h"
#include "key.h"
#include "pwm.h"

int main(void)
{
    uint8_t key;

    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    KEY_Init();                          /* ������ʼ�� */
    TIM3_PWM_Init(1000, 72, 500);        /* 50%ռ�ձ� */

    while(1)
    {
        key = KEY_Scan();
        if(key == 1)
        {
            if(TIM3->CCR2 == 0) TIM3->CCR2 = 0;
            else                TIM3->CCR2 -= 100;
        }
        else if(key == 2)
        {
            if(TIM3->CCR2 == 1000) TIM3->CCR2 = 1000;
            else                   TIM3->CCR2 += 100;
        }
    }
}
