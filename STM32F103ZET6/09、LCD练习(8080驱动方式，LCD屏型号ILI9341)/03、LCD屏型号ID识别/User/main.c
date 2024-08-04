#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"

int main(void)
{
    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    USART_Init(115200);                  /* ���ڳ�ʼ�� 115200 */
    LCD_ILI9341_Init();                  /* ILI9341��LCD����ʼ�� */

    delay_ms(50);

    /* ��ʶ����ͺţ�ST7789��ILI9341��NT35310��NT35510��SSD1963 */
    LCD_Recognition_Id();

    while(1)
    {
    }
}
