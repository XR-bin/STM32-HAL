#include "./SYSTEM/inc/sys.h"
#include "./SYSTEM/inc/usart.h"
#include "./SYSTEM/inc/delay.h"

int main(void)
{
    uint8_t len;

    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* ����ʱ��Ϊ72Mhz */
    delay_init(72);                         /* ��ʼ����ʱ���� */
    Usart1_Init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    
    printf("������һ��Ӣ���ַ���\r\n\r\n");
    
    while(1)
    {
        if (g_usart_rx_sta & 0x8000)        /* ���յ�������? */
        {
            len = g_usart_rx_sta & 0x3fff;  /* �õ��˴ν��յ������ݳ��� */
            printf("\r\n�����͵���ϢΪ:\r\n");

            HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_usart_rx_buf, len, 1000);    /* ���ͽ��յ������� */
            while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC) != SET);           /* �ȴ����ͽ��� */
            printf("\r\n\r\n");             /* ���뻻�� */
            g_usart_rx_sta = 0;
        }
        else
        {
            delay_ms(10);
        }
    }
}
