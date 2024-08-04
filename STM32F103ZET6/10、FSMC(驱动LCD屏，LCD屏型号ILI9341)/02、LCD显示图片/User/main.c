#include "sys.h"
#include "delay.h"
#include "lcd.h"
#include "tu1.h"
#include "tu2.h"

/**
* ע�⣺������������˴�����SRAM�ռ䣬�����¼F1�ͺŵ�STM32����������
*       ��SRAM����ˣ��������õ���STM32F103ZET6�Ĵ�����STM32���ܹ��ɹ����С�
*/

int main(void)
{
    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    LCD_ILI9341_Init();                  /* ILI9341��LCD����ʼ�� */

    /**
    * ע�⣺tu1��tu2��ͼƬ��ʽ�ǲ���һ����
    *       tu1����ɫ�����ǵ�λ��ǰ����λ�ں����� uint16����ɫ����Ϊ 0xF001��תΪuint8�����Ա 0x01 0xF0
    *       tu2����ɫ�����Ǹ�λ��ǰ����λ�ں����� uint16����ɫ����Ϊ 0xF001��תΪuint8�����Ա 0xF0 0x01
    *
    * LCD_Picture1��LCD_Picture3��������tu1��ʽ�ģ�LCD_Picture2����������tu2��ʽ��
    * LCD_Picture1��LCD_Picture3��ʵ�ַ�ʽ��һ��
    */
    LCD_Picture1(0, 0, (uint8_t*)gImage_tu1);
    delay_ms(8000);
    LCD_Picture2(0, 0, (uint8_t*)gImage_tu2);
    delay_ms(8000);
    LCD_Picture3(0, 0, (uint8_t*)gImage_tu1);

    while(1)
    {
    }
}
