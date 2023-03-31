#include "./SYSTEM/inc/sys.h"
#include "./SYSTEM/inc/usart.h"
#include "./SYSTEM/inc/delay.h"

int main(void)
{
    uint8_t len;

    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟为72Mhz */
    delay_init(72);                         /* 初始化延时函数 */
    Usart1_Init(115200);                     /* 串口初始化为115200 */
    
    printf("请输入一个英文字符：\r\n\r\n");
    
    while(1)
    {
        if (g_usart_rx_sta & 0x8000)        /* 接收到了数据? */
        {
            len = g_usart_rx_sta & 0x3fff;  /* 得到此次接收到的数据长度 */
            printf("\r\n您发送的消息为:\r\n");

            HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_usart_rx_buf, len, 1000);    /* 发送接收到的数据 */
            while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC) != SET);           /* 等待发送结束 */
            printf("\r\n\r\n");             /* 插入换行 */
            g_usart_rx_sta = 0;
        }
        else
        {
            delay_ms(10);
        }
    }
}
