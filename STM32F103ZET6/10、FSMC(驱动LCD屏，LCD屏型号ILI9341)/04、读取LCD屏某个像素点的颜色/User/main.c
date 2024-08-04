#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"

int main(void)
{
    uint32_t rgb=0;

    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    USART_Init(115200);                  /* ����1��ʼ��   115200 */
    LCD_ILI9341_Init();                  /* ILI9341��LCD����ʼ�� */

    delay_ms(50);

    LCD_ILI9341_ClearXY(100, 100, 100, 100, RED);    /* ��һ����ɫ���� */
    rgb = LCD_ReadPoint(101, 101);                   /* ȡ��ɫ�������һ�����ص����ɫ���� */
    printf("rgb��%d    %x", rgb, rgb);

    while(1)
    {
    }
}
