#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "iwdg.h"

int main(void)
{
    uint8_t key = 0;

    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    SysTick_Init();                      /* ��ʱ��ʼ�� */
    LED_Init();                          /* LED��ʼ�� */
    KEY_Init();                          /* KEY��ʼ�� */
    IWDG_Init(4, 1875);                  /* Ԥ��Ƶ��Ϊ64,����ֵΪ1875,���ʱ��Ϊ3s */

    delay_ms(1000);
    LED0_ON;

    while(1)
    {
        key = KEY_Scan();

        if(key) 
        {
            IWDG_Feed();
        }
    }
}

