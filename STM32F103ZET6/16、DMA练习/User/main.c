#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "dma.h"

uint8_t data[] = 
{
    0X97,0X01,0XB7,0X01,0XB7,0X01,0XB7,0X01,0X97,0X01,0XB7,0X01,0XB7,0X01,0XB7,0X01,
    0XD8,0X01,0XB7,0X01,0XB8,0X01,0XB8,0X01,0XD8,0X01,0XD8,0X01,0XD8,0X01,0XD8,0X01,
    0XD8,0X01,0XD8,0X01,0XD8,0X01,0XD8,0X01,0XF8,0X01,0XF9,0X01,0XF9,0X01,0XF8,0X01,
    0XF8,0X01,0XF9,0X01,0X19,0X02,0X19,0X02,0X19,0X02,0X19,0X02,0X19,0X02,0X19,0X02,
    0X19,0X02,0X39,0X02,0X39,0X02,0X39,0X02,0X39,0X02,0X39,0X02,0X39,0X02,0X39,0X02,
    0X19,0X02,0X39,0X02,0X59,0X02,0X39,0X02,0X39,0X02,0X5A,0X02,0X5A,0X02,0X39,0X02,
    0X5A,0X02,0X5A,0X02,0X5A,0X02,0X5A,0X02,0X5A,0X02,0X5A,0X02,0X7A,0X0A,0X7A,0X02,
    0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,
    0X9A,0X02,0X9A,0X02,0X9A,0X02,0X9A,0X02,0X9A,0X02,0X9A,0X02,0X9A,0X02,0XBB,0X02,
    0XBA,0X02,0XBB,0X02,0XBB,0X02,0XBB,0X02,0XDB,0X02,0XDB,0X02,0XDB,0X02,0XDA,0X02,
    0XDB,0X02,0XDB,0X02,0XDB,0X02,0XDB,0X02,0XFB,0X02,0XFB,0X02,0XDB,0X02,0XBA,0X02,
    0XDB,0X02,0XDB,0X02,0XFB,0X02,0XFB,0X02,0XFB,0X02,0XFB,0X02,0XFB,0X02,0XFB,0X02,
    0XDB,0X02,0XFB,0X02,0XFB,0X02,0XDB,0X02,0XFB,0X02,0X1B,0X03,0X1B,0X03,0XFB,0X02,
    0X1B,0X03,0X1B,0X03,0X1B,0X03,0X1B,0X03,0X1B,0X03,0X3C,0X03,0X3C,0X03,0X5C,0X03,
    0X3C,0X03,0X5C,0X03,0X5C,0X03,0X3C,0X03,0X3C,0X03,0X3C,0X03,0X3C,0X03,0X3C,0X03,
    0X3C,0X03,0X5C,0X03,0X5D,0X03,0X5D,0X03,0X5D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,
    0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,
    0X5D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,
    0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,
    0X97,0X01,0XB7,0X01,0XB7,0X01,0XB7,0X01,0X97,0X01,0XB7,0X01,0XB7,0X01,0XB7,0X01,
    0XD8,0X01,0XB7,0X01,0XB8,0X01,0XB8,0X01,0XD8,0X01,0XD8,0X01,0XD8,0X01,0XD8,0X01,
    0XD8,0X01,0XD8,0X01,0XD8,0X01,0XD8,0X01,0XF8,0X01,0XF9,0X01,0XF9,0X01,0XF8,0X01,
    0XF8,0X01,0XF9,0X01,0X19,0X02,0X19,0X02,0X19,0X02,0X19,0X02,0X19,0X02,0X19,0X02,
    0X19,0X02,0X39,0X02,0X39,0X02,0X39,0X02,0X39,0X02,0X39,0X02,0X39,0X02,0X39,0X02,
    0X19,0X02,0X39,0X02,0X59,0X02,0X39,0X02,0X39,0X02,0X5A,0X02,0X5A,0X02,0X39,0X02,
    0X5A,0X02,0X5A,0X02,0X5A,0X02,0X5A,0X02,0X5A,0X02,0X5A,0X02,0X7A,0X0A,0X7A,0X02,
    0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,
    0X9A,0X02,0X9A,0X02,0X9A,0X02,0X9A,0X02,0X9A,0X02,0X9A,0X02,0X9A,0X02,0XBB,0X02,
    0XBA,0X02,0XBB,0X02,0XBB,0X02,0XBB,0X02,0XDB,0X02,0XDB,0X02,0XDB,0X02,0XDA,0X02,
    0XDB,0X02,0XDB,0X02,0XDB,0X02,0XDB,0X02,0XFB,0X02,0XFB,0X02,0XDB,0X02,0XBA,0X02,
    0XDB,0X02,0XDB,0X02,0XFB,0X02,0XFB,0X02,0XFB,0X02,0XFB,0X02,0XFB,0X02,0XFB,0X02,
    0XDB,0X02,0XFB,0X02,0XFB,0X02,0XDB,0X02,0XFB,0X02,0X1B,0X03,0X1B,0X03,0XFB,0X02,
    0X1B,0X03,0X1B,0X03,0X1B,0X03,0X1B,0X03,0X1B,0X03,0X3C,0X03,0X3C,0X03,0X5C,0X03,
    0X3C,0X03,0X5C,0X03,0X5C,0X03,0X3C,0X03,0X3C,0X03,0X3C,0X03,0X3C,0X03,0X3C,0X03,
    0X3C,0X03,0X5C,0X03,0X5D,0X03,0X5D,0X03,0X5D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,
    0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,
    0X5D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,
    0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,
    0X97,0X01,0XB7,0X01,0XB7,0X01,0XB7,0X01,0X97,0X01,0XB7,0X01,0XB7,0X01,0XB7,0X01,
    0XD8,0X01,0XB7,0X01,0XB8,0X01,0XB8,0X01,0XD8,0X01,0XD8,0X01,0XD8,0X01,0XD8,0X01,
    0XD8,0X01,0XD8,0X01,0XD8,0X01,0XD8,0X01,0XF8,0X01,0XF9,0X01,0XF9,0X01,0XF8,0X01,
    0XF8,0X01,0XF9,0X01,0X19,0X02,0X19,0X02,0X19,0X02,0X19,0X02,0X19,0X02,0X19,0X02,
    0X19,0X02,0X39,0X02,0X39,0X02,0X39,0X02,0X39,0X02,0X39,0X02,0X39,0X02,0X39,0X02,
    0X19,0X02,0X39,0X02,0X59,0X02,0X39,0X02,0X39,0X02,0X5A,0X02,0X5A,0X02,0X39,0X02,
    0X5A,0X02,0X5A,0X02,0X5A,0X02,0X5A,0X02,0X5A,0X02,0X5A,0X02,0X7A,0X0A,0X7A,0X02,
    0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,
    0X9A,0X02,0X9A,0X02,0X9A,0X02,0X9A,0X02,0X9A,0X02,0X9A,0X02,0X9A,0X02,0XBB,0X02,
    0XBA,0X02,0XBB,0X02,0XBB,0X02,0XBB,0X02,0XDB,0X02,0XDB,0X02,0XDB,0X02,0XDA,0X02,
    0XDB,0X02,0XDB,0X02,0XDB,0X02,0XDB,0X02,0XFB,0X02,0XFB,0X02,0XDB,0X02,0XBA,0X02,
    0XDB,0X02,0XDB,0X02,0XFB,0X02,0XFB,0X02,0XFB,0X02,0XFB,0X02,0XFB,0X02,0XFB,0X02,
    0XDB,0X02,0XFB,0X02,0XFB,0X02,0XDB,0X02,0XFB,0X02,0X1B,0X03,0X1B,0X03,0XFB,0X02,
    0X1B,0X03,0X1B,0X03,0X1B,0X03,0X1B,0X03,0X1B,0X03,0X3C,0X03,0X3C,0X03,0X5C,0X03,
    0X3C,0X03,0X5C,0X03,0X5C,0X03,0X3C,0X03,0X3C,0X03,0X3C,0X03,0X3C,0X03,0X3C,0X03,
    0X3C,0X03,0X5C,0X03,0X5D,0X03,0X5D,0X03,0X5D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,
    0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,
    0X5D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,
    0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,
    0X97,0X01,0XB7,0X01,0XB7,0X01,0XB7,0X01,0X97,0X01,0XB7,0X01,0XB7,0X01,0XB7,0X01,
    0XD8,0X01,0XB7,0X01,0XB8,0X01,0XB8,0X01,0XD8,0X01,0XD8,0X01,0XD8,0X01,0XD8,0X01,
    0XD8,0X01,0XD8,0X01,0XD8,0X01,0XD8,0X01,0XF8,0X01,0XF9,0X01,0XF9,0X01,0XF8,0X01,
    0XF8,0X01,0XF9,0X01,0X19,0X02,0X19,0X02,0X19,0X02,0X19,0X02,0X19,0X02,0X19,0X02,
    0X19,0X02,0X39,0X02,0X39,0X02,0X39,0X02,0X39,0X02,0X39,0X02,0X39,0X02,0X39,0X02,
    0X19,0X02,0X39,0X02,0X59,0X02,0X39,0X02,0X39,0X02,0X5A,0X02,0X5A,0X02,0X39,0X02,
    0X5A,0X02,0X5A,0X02,0X5A,0X02,0X5A,0X02,0X5A,0X02,0X5A,0X02,0X7A,0X0A,0X7A,0X02,
    0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,0X7A,0X02,
    0X9A,0X02,0X9A,0X02,0X9A,0X02,0X9A,0X02,0X9A,0X02,0X9A,0X02,0X9A,0X02,0XBB,0X02,
    0XBA,0X02,0XBB,0X02,0XBB,0X02,0XBB,0X02,0XDB,0X02,0XDB,0X02,0XDB,0X02,0XDA,0X02,
    0XDB,0X02,0XDB,0X02,0XDB,0X02,0XDB,0X02,0XFB,0X02,0XFB,0X02,0XDB,0X02,0XBA,0X02,
    0XDB,0X02,0XDB,0X02,0XFB,0X02,0XFB,0X02,0XFB,0X02,0XFB,0X02,0XFB,0X02,0XFB,0X02,
    0XDB,0X02,0XFB,0X02,0XFB,0X02,0XDB,0X02,0XFB,0X02,0X1B,0X03,0X1B,0X03,0XFB,0X02,
    0X1B,0X03,0X1B,0X03,0X1B,0X03,0X1B,0X03,0X1B,0X03,0X3C,0X03,0X3C,0X03,0X5C,0X03,
    0X3C,0X03,0X5C,0X03,0X5C,0X03,0X3C,0X03,0X3C,0X03,0X3C,0X03,0X3C,0X03,0X3C,0X03,
    0X3C,0X03,0X5C,0X03,0X5D,0X03,0X5D,0X03,0X5D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,
    0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,
    0X5D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,
    0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,
    0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,0X7D,0X0B,
};

int main(void)
{
    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    SysTick_Init();                      /* 滴答定时器初始 */
    USART_Init(9600);                    /* 串口1初始化 9600 */
    LED_Init();                          /* LED灯初始化 */
    DMA1_Usart1_Init();                  /* DMA1初始化 */

    HAL_UART_Transmit_DMA(&g_uart1_handle, data, sizeof(data));   /* 开启DMA1 */

    LED1_ON;
    delay_ms(2000);
    LED1_OFF;

    while(1)
    {
    }
}
