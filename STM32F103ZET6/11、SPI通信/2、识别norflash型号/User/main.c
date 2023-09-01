#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "norflash.h"

int main(void)
{
    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    USART_Init(115200);                  /* ���ڳ�ʼ��  115200 */
    NorFlash_Init();                     /* W25Q128��ʼ�� */

    NorFlash_Read_ID();


    while(1)
    {
    }
}
