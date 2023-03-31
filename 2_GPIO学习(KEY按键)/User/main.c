#include "./SYSTEM/inc/sys.h"
#include "./SYSTEM/inc/delay.h"
#include "./BSP/inc/led.h"
#include "./BSP/inc/key.h"

int main(void)
{
    uint8_t key=0;

    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* ����ʱ��,72M */
    delay_init(72);                             /* ��ʼ����ʱ���� */
    LED_Init();
    Key_Init();
    
    while(1)
    {
        key = Key_Scan(0);
        
        if(key != 0)
        {
            switch(key)
            {
                case KEY0_PRES: 
                    LED0_TOGGLE();
                    break;
                    
                case KEY1_PRES: 
                    LED1_TOGGLE();
                    break;
                    
                case WKUP_PRES:
                    LED0_TOGGLE();
                    LED1_TOGGLE();
                    break;
                    
                default: break;
            }
        }
        else
        {
            delay_ms(20);
        }
    }
}
