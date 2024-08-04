#include "sys.h"
#include "delay.h"
#include "usart.h"

int main(void)
{
    uint8_t data;

    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    USART_Init(115200);                  /* ����1��ʼ��  115200 */

    while(1)
    {
        data = USART1_Receive_Byte();
        USART1_Send_Byte(data);
    }
}
