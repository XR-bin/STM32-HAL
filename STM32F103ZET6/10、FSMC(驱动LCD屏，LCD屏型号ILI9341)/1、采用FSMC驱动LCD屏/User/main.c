#include "sys.h"
#include "delay.h"
#include "lcd.h"

int main(void)
{
    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    LCD_ILI9341_Init();                  /* ILI9341��LCD����ʼ�� */

    //    LCD_Draw_Circle(100, 100, 50, RED);           /* ����Բ */
    //    LCD_Fill_Circle(100, 100, 50, BLUE);          /* ʵ��Բ */
    //    LCD_Draw_Rectangle(50, 50, 100, 100, BLACK);  /* ���ľ��� */
    //    LCD_Fill(50, 50, 100, 100, YELLOW);           /* ʵ�ľ��� */
    //    LCD_ILI9341_ClearXY(50, 50, 100, 100, GREEN); /* ʵ�ľ��� */
    //    LCD_Draw_Line(50, 50, 100, 100, MAGENTA);     /* ��б�� */
    //    LCD_Draw_Hline(50, 50, 100, MAGENTA);         /* ˮƽֱ�� */

    //    LCD_String16(50, 50, "123asd,./", RED);       /* ��ʾ16*16�ַ��� */
    //    LCD_String24(50, 80, "123asd,./", BLUE);      /* ��ʾ24*24�ַ��� */
    //    LCD_String32(50, 120, "123asd,./", GREEN);    /* ��ʾ32*32�ַ��� */

    /* ע�⣺�����������ĺ�����ģ�Ҿ�ȡ���������� */
    LCD_HanziStr32(50, 120, "��˼��", DARKBLUE);    /* ��ʾ32*32���� */

    while(1)
    {
    }
}
