#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"

int main(void)
{
    uint8_t key = 0;
    uint8_t flay = 1;
    uint8_t i = 0;
    uint32_t cont = 0;

    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    SysTick_Init();                      /* ��ʱ��ʼ�� */
    LED_Init();                          /* LED��ʼ�� */
    KEY_Init();                          /* KEY��ʼ�� */

    while(1)
    {
        key = KEY_Scan();
        switch(key)
        {
            case 1:LED0_ON;LED1_OFF;flay = 0;break;
            case 2:LED1_ON;LED0_OFF;flay = 0;break;
            case 3:flay = 1;LED0_OFF;LED1_OFF;break;
        }
        if(flay)
        {
            if(i)
            {
                LED0_ON;
                LED1_OFF;
            }
            else 
            {
                LED0_OFF;
                LED1_ON;
            }
            cont++;
            if(cont ==60000)
            {
                i=!i;
                cont = 0;
            }
        }
    }
}

