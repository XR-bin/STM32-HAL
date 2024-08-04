#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "w25q128.h"

int main(void)
{
    uint8_t str[] = "qwe123456789zxc";
    uint8_t arr[30] = {0};

    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    USART_Init(115200);                  /* ���ڳ�ʼ��  115200 */
    W25Q128_Init();                      /* W25Q128��ʼ�� */

    W25Q128_Write(0x00, sizeof(str), str);
    W25Q128_Read_Bytes(0x00, sizeof(str), arr);

    printf("str��%s\r\n", str);
    printf("arr��%s\r\n", arr);

    while(1)
    {
    }
}
