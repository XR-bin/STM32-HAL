#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "touch.h"

int main(void)
{
    uint16_t x=0,y=0;

    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    USART_Init(115200);                  /* ���ڳ�ʼ�� 115200 */
    Touch_IO_Init();                     /* ���败�������ų�ʼ�� */

    while(1)
    {
        if(T_PEN == 0)
        {
            x = Touch_Data(TOUCH_X);          /* ��ȡ����x���� */
            y = Touch_Data(TOUCH_Y);          /* ��ȡ����y���� */
            printf("x:%4d   y:%4d\r\n",x,y);
        }
    }
}
