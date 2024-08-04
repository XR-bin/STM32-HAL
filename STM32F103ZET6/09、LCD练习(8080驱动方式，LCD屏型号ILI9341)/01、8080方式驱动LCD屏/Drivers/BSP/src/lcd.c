#include "lcd.h"
#include "font.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\lcd.c
*              .\Drivers\BSP\inc\lcd.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-01
* @brief     ��LCD��(ILI9341)��������
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/* ����LCD��Ҫ���� */
_lcd_dev lcddev;

/**********************************************************
* @funcName ��LCD_IO_Init
* @brief    ����LCD��Ӧ��GPIO�ڽ��г�ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
*     �����ߣ� ͨ�����   ����   �ٶ�50M
*           DO0---PD14    DO1---PD15    DO2---PD0    DO3---PD1
*           DO4---PE7     DO5---PE8     DO6---PE9    DO7---PE10
*           DO8---PE11    DO9---PE12    DO10---PE13  DO11---PE14
*           DO12---PE15   DO13---PD8    DO14---PD9   DO15---PD10
*
*     �����ߣ� ͨ�����   ����   �ٶ�50M
*           WR---PD5    RD---PD4    CS---PG12
*           RS/DC(����/����)---PG0
*           BL�����⣩--PB0
*     ����
*         PB��0
*         PD��0-1   4-5   8-10   14-15  
*         PE��7-15
*         PG��0   12
* @fn       ��
************************************************************/
void LCD_IO_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;                      /* GPIO���ýṹ�� */

    /* ʱ��ʹ��   GPIOB   GPIOD   GPIOE  GPIOG */
    __HAL_RCC_GPIOB_CLK_ENABLE();                           /* ʹ��PB�˿�ʱ�� */
    __HAL_RCC_GPIOD_CLK_ENABLE();                           /* ʹ��PD�˿�ʱ�� */
    __HAL_RCC_GPIOE_CLK_ENABLE();                           /* ʹ��PE�˿�ʱ�� */
    __HAL_RCC_GPIOG_CLK_ENABLE();                           /* ʹ��PG�˿�ʱ�� */

    /* PB0 */
    gpio_init_struct.Pin   = GPIO_PIN_0;                    /* PB0 �˿����� */
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_PP;           /* ������� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);                /* ��ʼ��LED0���� */

    /* PD0/1/4/5/8/9/10/14/15 */
    gpio_init_struct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;     /* PD0/1/4/5/8/9/10/14/15 �˿����� */
    HAL_GPIO_Init(GPIOD, &gpio_init_struct);                /* �����趨������ʼ��PD0/1/4/5/8/9/10/14/15 */

    /* PE7-15 */
    gpio_init_struct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;  /* PE7-15 �˿����� */
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);                /* �����趨������ʼ��PE7-15 */

    /* PG0/12 */
    gpio_init_struct.Pin = GPIO_PIN_0|GPIO_PIN_12;          /* PG0/12 �˿����� */
    HAL_GPIO_Init(GPIOG, &gpio_init_struct);                /* �����趨������ʼ��PG0/12 */

    LCD_CS_H;      /* Ƭѡ���� */
    LCD_WR_H;      /* дʹ������ */
    LCD_RD_H;      /* ��ʹ������ */
    LCD_DC_H;      /* ��������ѡ�������� */
    LCD_BL_L;      /* ��������(�ر�) */
}



/**********************************************************
* @funcName ��LCD_WriteCMD
* @brief    ����LCD�����������
* @param    ��uint8_t cmd(LCDָ��/�Ĵ������/��ַ)
* @retval   ��void
* @details  ��
* @fn       ��
*         LCD_CS_H(Ƭѡʧ��)   LCD_CS_L(Ƭѡʹ��)
*         LCD_WR_H(дʧ��)     LCD_WR_L(дʹ��)
*         LCD_RD_H(��ʧ��)     LCD_RD_L(��ʹ��)
*         LCD_DC_H(�շ�����)   LCD_DC_L(�շ�����)
*         LCD_BL_H(��������)   LCD_BL_L(�رձ���)
************************************************************/
void LCD_WriteCMD(uint8_t cmd)
{
    LCD_RD_H;           /* ��ʹ������ */
    LCD_DC_L;           /* ��������ѡ�������� */
    LCD_CS_L;           /* Ƭѡ���� */
    LCD_WR_L;           /* дʹ������ */

    /* ׼��Ҫ���͵����� */
    if(cmd & 0x01) LCD_DO_0(1);    else LCD_DO_0(0);
    if(cmd & 0x02) LCD_DO_1(1);    else LCD_DO_1(0);
    if(cmd & 0x04) LCD_DO_2(1);    else LCD_DO_2(0);
    if(cmd & 0x08) LCD_DO_3(1);    else LCD_DO_3(0);
    if(cmd & 0x10) LCD_DO_4(1);    else LCD_DO_4(0);
    if(cmd & 0x20) LCD_DO_5(1);    else LCD_DO_5(0);
    if(cmd & 0x40) LCD_DO_6(1);    else LCD_DO_6(0);
    if(cmd & 0x80) LCD_DO_7(1);    else LCD_DO_7(0);

    LCD_WR_H;    /* дʹ������ */
    LCD_CS_H;    /* Ƭѡ���� */
}



/**********************************************************
* @funcName ��LCD_WriteData
* @brief    ����LCD����������
* @param    ��uint16_t data(����)
* @retval   ��void
* @details  ��
* @fn       ��
*         LCD_CS_H(Ƭѡʧ��)   LCD_CS_L(Ƭѡʹ��)
*         LCD_WR_H(дʧ��)     LCD_WR_L(дʹ��)
*         LCD_RD_H(��ʧ��)     LCD_RD_L(��ʹ��)
*         LCD_DC_H(�շ�����)   LCD_DC_L(�շ�����)
*         LCD_BL_H(��������)   LCD_BL_L(�رձ���)
************************************************************/
void LCD_WriteData(uint16_t data)
{
    LCD_RD_H;              /* ��ʹ������ */
    LCD_DC_H;              /* ��������ѡ�������� */
    LCD_CS_L;              /* Ƭѡ���� */
    LCD_WR_L;              /* дʹ������ */

    /* ׼��Ҫ���͵����� */
    if(data & 0x0001)  LCD_DO_0(1);    else LCD_DO_0(0);
    if(data & 0x0002)  LCD_DO_1(1);    else LCD_DO_1(0);
    if(data & 0x0004)  LCD_DO_2(1);    else LCD_DO_2(0);
    if(data & 0x0008)  LCD_DO_3(1);    else LCD_DO_3(0);
    if(data & 0x0010)  LCD_DO_4(1);    else LCD_DO_4(0);
    if(data & 0x0020)  LCD_DO_5(1);    else LCD_DO_5(0);
    if(data & 0x0040)  LCD_DO_6(1);    else LCD_DO_6(0);
    if(data & 0x0080)  LCD_DO_7(1);    else LCD_DO_7(0);
    if(data & 0x0100)  LCD_DO_8(1);    else LCD_DO_8(0);
    if(data & 0x0200)  LCD_DO_9(1);    else LCD_DO_9(0);
    if(data & 0x0400)  LCD_DO_10(1);   else LCD_DO_10(0);
    if(data & 0x0800)  LCD_DO_11(1);   else LCD_DO_11(0);
    if(data & 0x1000)  LCD_DO_12(1);   else LCD_DO_12(0);
    if(data & 0x2000)  LCD_DO_13(1);   else LCD_DO_13(0);
    if(data & 0x4000)  LCD_DO_14(1);   else LCD_DO_14(0);
    if(data & 0x8000)  LCD_DO_15(1);   else LCD_DO_15(0);

    LCD_WR_H;    /* дʹ������ */
    LCD_CS_H;    /* Ƭѡ���� */
}



/**********************************************************
* @funcName ��LCD_ILI9341_SetCursor
* @brief    �����ù��λ��(��RGB����Ч)
* @param    ��uint16_t x(x���꣬������)
* @param    ��uint16_t y(y���꣬������)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_ILI9341_SetCursor(uint16_t x, uint16_t y)
{
    LCD_WriteCMD(lcddev.setxcmd);  /* ��������(x����) */
    LCD_WriteData(x >> 8);         /* д����ʼx���� */
    LCD_WriteData(x & 0XFF);

    LCD_WriteCMD(lcddev.setycmd);  /* ��������(y����) */
    LCD_WriteData(y >> 8);         /* д����ʼy���� */
    LCD_WriteData(y & 0XFF);
}



/**********************************************************
* @funcName ��LCD_Clear
* @brief    ��LCD��Ļ���ĳһ����ɫ����������
* @param    ��uint16_t color(��ɫ����)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_Clear(uint16_t color)
{
    uint32_t i;

    i = lcddev.width * lcddev.height;

    LCD_ILI9341_SetCursor(0x00, 0x00);      /* ���ù�� */

    /* ��������(��ɫ) */
    LCD_WriteCMD(lcddev.wramcmd);
    /* ��ʼ��ɫ */
    while(i)
    {
        LCD_WriteData(color);
        i--;
    }
}



/**********************************************************
* @funcName ��LCD_ILI9341_ScanDir
* @brief    ������LCD(ILI9341)���Զ�ɨ�跽��
* @param    ��uint8_t dir(ɨ�跽��)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_ILI9341_ScanDir(uint8_t dir)
{
    uint16_t regval=0;
    uint8_t dirreg=0;
    uint16_t temp;

    /**
    * ����ʱ����1963���ı�ɨ�跽������IC�ı�ɨ�跽��
    * ����ʱ��1963�ı䷽������IC���ı�ɨ�跽��
    *
    * ע�⣺���������õ���Ļ��ILI9341
    **/
    if(lcddev.dir==1)            /* ����ʱ���� */
    {
        switch(dir)              /* ɨ�跽��ת�� */
        {
            case 0:dir=6;break;
            case 1:dir=7;break;
            case 2:dir=4;break;
            case 3:dir=5;break;
            case 4:dir=1;break;
            case 5:dir=0;break;
            case 6:dir=3;break;
            case 7:dir=2;break;
        }
    }

    /* ����ɨ�跽ʽ ���� 0X36/0X3600 �Ĵ��� bit 5,6,7 λ��ֵ */
    switch (dir)
    {
        /* ������,���ϵ��� */
        case L2R_U2D:
            regval |= (0 << 7) | (0 << 6) | (0 << 5);
            break;

        /* ������,���µ��� */
        case L2R_D2U:
            regval |= (1 << 7) | (0 << 6) | (0 << 5);
            break;

        /* ���ҵ���,���ϵ��� */
        case R2L_U2D:
            regval |= (0 << 7) | (1 << 6) | (0 << 5);
            break;

        /* ���ҵ���,���µ��� */
        case R2L_D2U:
            regval |= (1 << 7) | (1 << 6) | (0 << 5);
            break;

        /* ���ϵ���,������ */
        case U2D_L2R:
            regval |= (0 << 7) | (0 << 6) | (1 << 5);
            break;

        /* ���ϵ���,���ҵ��� */
        case U2D_R2L:
            regval |= (0 << 7) | (1 << 6) | (1 << 5);
            break;

        /* ���µ���,������ */
        case D2U_L2R:
            regval |= (1 << 7) | (0 << 6) | (1 << 5);
            break;

        /* ���µ���,���ҵ��� */
        case D2U_R2L:
            regval |= (1 << 7) | (1 << 6) | (1 << 5);
            break;
    }

    dirreg = 0X36;    /* �Ծ��󲿷�����IC, ��0X36�Ĵ������� */
    regval |= 0X08;   /* ILI9341Ҫ����BGRλ */

    LCD_WriteCMD(dirreg);
    LCD_WriteData(regval);

    /* ����Ļ��������� */
    if(regval&0X20)
    {
        if (lcddev.width < lcddev.height)   /* ����X,Y */
        {
            temp = lcddev.width;
            lcddev.width = lcddev.height;
            lcddev.height = temp;
        }
    }
    else  
    {
        if (lcddev.width > lcddev.height)   /* ����X,Y */
        {
            temp = lcddev.width;
            lcddev.width = lcddev.height;
            lcddev.height = temp;
        }
    }

    LCD_WriteCMD(lcddev.setxcmd);              /* ��������(x����) */
    LCD_WriteData(0);                          /* д����ʼx���� */
    LCD_WriteData(0);
    LCD_WriteData((lcddev.width - 1) >> 8);    /* д�����x���� */
    LCD_WriteData((lcddev.width - 1) & 0XFF);

    LCD_WriteCMD(lcddev.setycmd);              /* ��������(y����) */
    LCD_WriteData(0);                          /* д����ʼy���� */
    LCD_WriteData(0);
    LCD_WriteData((lcddev.height - 1) >> 8);   /* д�����y���� */
    LCD_WriteData((lcddev.height - 1) & 0XFF);
}



/**********************************************************
* @funcName ��LCD_ILI9341_DisplayDir
* @brief    ������LCD������
* @param    ��uint8_t dir
*   @arg    ��0 --- ����   1 --- ����
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_ILI9341_DisplayDir(uint8_t dir)
{
    if(dir==0)
    {
        lcddev.dir = 0;             /* ���� */
        lcddev.width = LCD_WIDTH;   /* ��Ļ�� */
        lcddev.height = LCD_HEIGHT; /* ��Ļ�� */
    }
    else
    {
        lcddev.dir = 1;             /* ���� */
        lcddev.width = LCD_HEIGHT;  /* ��Ļ�� */
        lcddev.height = LCD_WIDTH;  /* ��Ļ�� */
    }

    lcddev.wramcmd=0X2C;    /* ��ɫָ�� */
    lcddev.setxcmd=0X2A;    /* x����ָ�� */
    lcddev.setycmd=0X2B;    /* y����ָ�� */

    LCD_ILI9341_ScanDir(DFT_SCAN_DIR);  /* Ĭ��ɨ�跽�� */
}


 
/**********************************************************
* @funcName ��LCD_ILI9341_Init
* @brief    ��LCD��Ļ��ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_ILI9341_Init(void)
{
    /* IO�ڳ�ʼ�� */
    LCD_IO_Init();

    delay1_ms(10);
    lcddev.id = LED_ID;    /* ��Ļ�ͺ� */

    /* LCD���Ҵ��� */
    LCD_WriteCMD(0xCF);
    LCD_WriteData(0x00);
    LCD_WriteData(0xC1);
    LCD_WriteData(0X30);
    LCD_WriteCMD(0xED);
    LCD_WriteData(0x64);
    LCD_WriteData(0x03);
    LCD_WriteData(0X12);
    LCD_WriteData(0X81);
    LCD_WriteCMD(0xE8);
    LCD_WriteData(0x85);
    LCD_WriteData(0x10);
    LCD_WriteData(0x7A);
    LCD_WriteCMD(0xCB);
    LCD_WriteData(0x39);
    LCD_WriteData(0x2C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x34);
    LCD_WriteData(0x02);
    LCD_WriteCMD(0xF7);
    LCD_WriteData(0x20);
    LCD_WriteCMD(0xEA);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteCMD(0xC0);        /* Power control */
    LCD_WriteData(0x1B);       /* VRH[5:0] */
    LCD_WriteCMD(0xC1);        /* Power control */
    LCD_WriteData(0x01);       /* SAP[2:0];BT[3:0] */
    LCD_WriteCMD(0xC5);        /* VCM control */
    LCD_WriteData(0x30);       /* 3F */
    LCD_WriteData(0x30);       /* 3C */
    LCD_WriteCMD(0xC7);        /* VCM control2 */
    LCD_WriteData(0XB7);
    LCD_WriteCMD(0x36);        /* Memory Access Control */
    LCD_WriteData(0x48);
    LCD_WriteCMD(0x3A);
    LCD_WriteData(0x55);
    LCD_WriteCMD(0xB1);
    LCD_WriteData(0x00);
    LCD_WriteData(0x1A);
    LCD_WriteCMD(0xB6);        /*  Display Function Control */
    LCD_WriteData(0x0A);
    LCD_WriteData(0xA2);
    LCD_WriteCMD(0xF2);        /*  3Gamma Function Disable */
    LCD_WriteData(0x00);
    LCD_WriteCMD(0x26);        /* Gamma curve selected */
    LCD_WriteData(0x01);
    LCD_WriteCMD(0xE0);        /* Set Gamma */
    LCD_WriteData(0x0F);
    LCD_WriteData(0x2A);
    LCD_WriteData(0x28);
    LCD_WriteData(0x08);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x08);
    LCD_WriteData(0x54);
    LCD_WriteData(0XA9);
    LCD_WriteData(0x43);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x0F);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteCMD(0XE1);        /* Set Gamma */
    LCD_WriteData(0x00);
    LCD_WriteData(0x15);
    LCD_WriteData(0x17);
    LCD_WriteData(0x07);
    LCD_WriteData(0x11);
    LCD_WriteData(0x06);
    LCD_WriteData(0x2B);
    LCD_WriteData(0x56);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x05);
    LCD_WriteData(0x10);
    LCD_WriteData(0x0F);
    LCD_WriteData(0x3F);
    LCD_WriteData(0x3F);
    LCD_WriteData(0x0F);
    LCD_WriteCMD(0x2B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x01);
    LCD_WriteData(0x3f);
    LCD_WriteCMD(0x2A);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0xef);
    LCD_WriteCMD(0x11);        /* Exit Sleep */
    delay_ms(120);
    LCD_WriteCMD(0x29);        /* display on */

    /* �û��Լ���д */
    LCD_ILI9341_DisplayDir(0);    /* ������ĻΪ���� */
    LCD_BL_H;                     /* ����ƴ� */
    LCD_Clear(WHITE);             /* �������� */
}



/**********************************************************
* @funcName ��LCD_ILI9341_ClearXY
* @brief    ��LCD��Ļ��ĳ����������ĳ����ɫ
* @param    ��uint16_t x      (�����ʼλ�õ�x���꣬������)
* @param    ��uint16_t y      (�����ʼλ�õ�y���꣬������)
* @param    ��uint16_t w      (���εĿ�)
* @param    ��uint16_t h      (���εĸ�)
* @param    ��uint16_t color (���ε����ɫ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_ILI9341_ClearXY(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    uint32_t i;
    i = w * h;

    LCD_WriteCMD(lcddev.setxcmd);     /* ��������(x����) */
    /* д��x�������� */
    LCD_WriteData(x>>8);              /* ��ʼx���� */
    LCD_WriteData(x);
    LCD_WriteData((x+w) >> 8);        /* ĩβx���� */
    LCD_WriteData(x+w);

    LCD_WriteCMD(lcddev.setycmd);     /* ��������(y����) */
    /* д��y�������� */
    LCD_WriteData(y>>8);              /* ��ʼy���� */
    LCD_WriteData(y);
    LCD_WriteData((y+h) >> 8);        /* ĩβy���� */
    LCD_WriteData(y+h);

    LCD_WriteCMD(lcddev.wramcmd);     /* ��������(�����ɫ) */
    /* ��ʼ��ɫ */
    while(i)
    {
        LCD_WriteData(color);         /* ������ɫ���� */
        i--;
    }
}



/**********************************************************
* @funcName ��LCD_Point
* @brief    ��LCD��Ļ���㺯��
* @param    ��uint16_t x      (�����ʼλ�õ�x���꣬������)
* @param    ��uint16_t y      (�����ʼλ�õ�y���꣬������)
* @param    ��uint16_t color (���ε����ɫ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_Point(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_ILI9341_SetCursor(x, y);     /* ���ù�� */

    /* ��������(�����ɫ) */
    LCD_WriteCMD(lcddev.wramcmd);

    /* ��ʼ��ɫ */
    LCD_WriteData(color);
}



/**********************************************************
* @funcName ��LCD_Fill
* @brief    ����ָ����������䵥����ɫ
* @param    ��uint16_t x0      (�������Ͻǵ�x���꣬������)
* @param    ��uint16_t y0      (�������Ͻǵ�y���꣬������)
* @param    ��uint16_t x1      (�������½ǵ�x���꣬������)
* @param    ��uint16_t y1      (�������½ǵ�y���꣬������)
* @param    ��uint16_t color   (���ε����ɫ)
* @retval   ��void
* @details  ��
*             ���������СΪ:(x1 - x0 + 1) * (y1 - y0 + 1)
* @fn       ��
************************************************************/
void LCD_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    uint16_t i, j;
    uint16_t xlen = x1 - x0 +1;

    for (i = y0; i <= y1; i++)
    {
        LCD_ILI9341_SetCursor(x0, i);      /* ���ù��λ�� */

        /* ��������(�����ɫ) */
        LCD_WriteCMD(lcddev.wramcmd);

        for (j = 0; j < xlen; j++)
        {
            LCD_WriteData(color);          /* ������ɫ���� */
        }
    }
}



/**********************************************************
* @funcName ��LCD_Color_Fill
* @brief    ����ָ�����������ָ����ɫ��(�������ڻ�ͼ)
* @param    ��uint16_t x0      (�������Ͻǵ�x���꣬������)
* @param    ��uint16_t y0      (�������Ͻǵ�y���꣬������)
* @param    ��uint16_t x1      (�������½ǵ�x���꣬������)
* @param    ��uint16_t y1      (�������½ǵ�y���꣬������)
* @param    ��uint16_t *color  (Ҫ������ɫ�����׵�ַ)
* @retval   ��void
* @details  ��
*             ���������СΪ:(x1 - x0 + 1) * (y1 - y0 + 1)
* @fn       ��
************************************************************/
void LCD_Color_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t *color)
{
    uint16_t height, width;
    uint16_t i, j;
    width = x1 - x0 + 1;            /* �õ����Ŀ�� */
    height = y1 - y0 + 1;           /* �߶� */

    for (i = 0; i < height; i++)
    {
        LCD_ILI9341_SetCursor(x0, y0+i);      /* ���ù��λ�� */

        /* ��������(�����ɫ) */
        LCD_WriteCMD(lcddev.wramcmd);

        for (j = 0; j < width; j++)
        {
            LCD_WriteData(color[i*width + j]);   /* ������ɫ���� */
        }
    }
}



/**********************************************************
* @funcName ��LCD_Draw_Line
* @brief    ����һ��ֱ��(������ˮƽ�ߣ�Ҳ������б��)
* @param    ��uint16_t x0      (��ǰ�˵�x���꣬������)
* @param    ��uint16_t y0      (��ǰ�˵�y���꣬������)
* @param    ��uint16_t x1      (�ߺ�˵�x���꣬������)
* @param    ��uint16_t y1      (�ߺ�˵�y���꣬������)
* @param    ��uint16_t color   (�ߵ���ɫ)
* @retval   ��void
* @details  ��
*             ����ȷ��һ��ֱ��
* @fn       ��
************************************************************/
void LCD_Draw_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    delta_x = x1 - x0;           /* ������������ */
    delta_y = y1 - y0;
    row = x0;
    col = y0;

    if (delta_x > 0)incx = 1;   /* ���õ������� */
    else if (delta_x == 0)incx = 0; /* ��ֱ�� */
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0; /* ˮƽ�� */
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if ( delta_x > delta_y)distance = delta_x;  /* ѡȡ�������������� */
    else distance = delta_y;

    for (t = 0; t <= distance + 1; t++ )   /* ������� */
    {
        LCD_Point(row, col, color);        /* ���� */
        xerr += delta_x ;
        yerr += delta_y ;

        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}



/**********************************************************
* @funcName ��LCD_Draw_Hline
* @brief    ����һ��ˮƽֱ��
* @param    ��uint16_t x     (��ǰ�˵�x���꣬������)
* @param    ��uint16_t y     (��ǰ�˵�y���꣬������)
* @param    ��uint16_t len   (�ߵĳ���)
* @param    ��uint16_t color (�ߵ���ɫ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_Draw_Hline(uint16_t x, uint16_t y, uint16_t len, uint16_t color)
{
    if ((len == 0) || (x > lcddev.width) || (y > lcddev.height))return;

    LCD_Fill(x, y, x + len - 1, y, color);
}




/**********************************************************
* @funcName ��LCD_Draw_Rectangle
* @brief    �������ľ���
* @param    ��uint16_t x0      (�������Ͻǵ�x���꣬������)
* @param    ��uint16_t y0      (�������Ͻǵ�y���꣬������)
* @param    ��uint16_t x1      (�������½ǵ�x���꣬������)
* @param    ��uint16_t y1      (�������½ǵ�y���꣬������)
* @param    ��uint16_t color   (���ε����ɫ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_Draw_Rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    LCD_Draw_Line(x0, y0, x1, y0, color);
    LCD_Draw_Line(x0, y0, x0, y1, color);
    LCD_Draw_Line(x0, y1, x1, y1, color);
    LCD_Draw_Line(x1, y0, x1, y1, color);
}



/**********************************************************
* @funcName ��LCD_Draw_Circle
* @brief    ��LCD��Ļ������Բ
* @param    ��uint16_t x      (Բ�ĵ�x���꣬������)
* @param    ��uint16_t y      (Բ�ĵ�y���꣬������)
* @param    ��uint8_t r       (Բ�ĵİ뾶)
* @param    ��uint16_t color (�ߵ���ɫ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_Draw_Circle(uint16_t x, uint16_t y, uint8_t r, uint16_t color)
{
    int a=0, b=r;
    int di = 3 - (r << 1);       /* �ж��¸���λ�õı�־ */

    while (a <= b)
    {
        LCD_Point(x + a, y - b, color);  /* 5 */
        LCD_Point(x + b, y - a, color);  /* 0 */
        LCD_Point(x + b, y + a, color);  /* 4 */
        LCD_Point(x + a, y + b, color);  /* 6 */
        LCD_Point(x - a, y + b, color);  /* 1 */
        LCD_Point(x - b, y + a, color);
        LCD_Point(x - a, y - b, color);  /* 2 */
        LCD_Point(x - b, y - a, color);  /* 7 */
        a++;

        /* ʹ��Bresenham�㷨��Բ */
        if (di < 0)
        {
            di += 4 * a + 6;
        }
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}



/**********************************************************
* @funcName ��LCD_Fill_Circle
* @brief    ��LCD��Ļ��ʵ��Բ
* @param    ��uint16_t x      (Բ�ĵ�x���꣬������)
* @param    ��uint16_t y      (Բ�ĵ�y���꣬������)
* @param    ��uint16_t r      (Բ�ĵİ뾶)
* @param    ��uint16_t color  (�ߵ���ɫ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_Fill_Circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    uint32_t i;
    uint32_t imax = ((uint32_t)r * 707) / 1000 + 1;
    uint32_t sqmax = (uint32_t)r * (uint32_t)r + (uint32_t)r / 2;
    uint32_t xr = r;

    LCD_Draw_Hline(x - r, y, 2 * r, color);

    for (i = 1; i <= imax; i++)
    {
        if ((i * i + xr * xr) > sqmax)
        {
            /* draw lines from outside */
            if (xr > imax)
            {
                LCD_Draw_Hline (x - i + 1, y + xr, 2 * (i - 1), color);
                LCD_Draw_Hline (x - i + 1, y - xr, 2 * (i - 1), color);
            }

            xr--;
        }

        /* draw lines from inside (center) */
        LCD_Draw_Hline(x - xr, y + i, 2 * xr, color);
        LCD_Draw_Hline(x - xr, y - i, 2 * xr, color);
    }
}



/**********************************************************
* @funcName ��LCD_Character16
* @brief    ��LCD��Ļ��ʾһ��16*16�ַ�
* @param    ��uint16_t x      (�ַ����Ͻǵ�x���꣬������)
* @param    ��uint16_t y      (�ַ����Ͻǵ�y���꣬������)
* @param    ��uint8_t ch      (Ҫ��ʾ���ַ�)
* @param    ��uint16_t color  (�ַ���ɫ����ɫ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_Character16(uint16_t x, uint16_t y, uint8_t ch, uint16_t color)
{
    uint8_t n;
    uint8_t i,j;
    uint8_t temp;

    /*�����Ҫ��ʾ���ַ���ո��ַ���ƫ��*/
    n = ch - ' ';

    /*��ʾ*/
    for(i=0;i<16;i++)
    {
        temp = model16[n*16+i];
        for(j=0;j<8;j++)
        {
            if(temp & (0x80 >> j))
            {
                LCD_Point(x+j,y+i,color);
            }
        }
    }
}



/**********************************************************
* @funcName ��LCD_Character24
* @brief    ��LCD��Ļ��ʾһ��24*24�ַ�
* @param    ��uint16_t x      (�ַ����Ͻǵ�x���꣬������)
* @param    ��uint16_t y      (�ַ����Ͻǵ�y���꣬������)
* @param    ��uint8_t ch      (Ҫ��ʾ���ַ�)
* @param    ��uint16_t color  (�ַ���ɫ����ɫ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_Character24(uint16_t x, uint16_t y, uint8_t ch, uint16_t color)
{
    uint8_t n;
    uint8_t i,j;
    uint16_t temp;
    uint8_t temp1;
    uint8_t temp2;

    /* �����Ҫ��ʾ���ַ���ո��ַ���ƫ�� */
    n = ch - ' ';

    /* ��ʾ */
    for(i=0;i<24;i++)
    {
        temp1 = model24[n*48+i*2];
        temp2 = model24[n*48+i*2+1];
        temp = (temp1<<4) | (temp2>>4);

        for(j=0;j<12;j++)
        {
            if(temp & (0x800 >> j))
            {
                LCD_Point(x+j,y+i,color);
            }
        }
    }
}



/**********************************************************
* @funcName ��LCD_Character32
* @brief    ��LCD��Ļ��ʾһ��32*32�ַ�
* @param    ��uint16_t x      (�ַ����Ͻǵ�x���꣬������)
* @param    ��uint16_t y      (�ַ����Ͻǵ�y���꣬������)
* @param    ��uint8_t  ch     (Ҫ��ʾ���ַ�)
* @param    ��uint16_t color  (�ַ���ɫ����ɫ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_Character32(uint16_t x, uint16_t y, uint8_t ch, uint16_t color)
{
    uint8_t n;
    uint8_t i,j;
    uint16_t temp;
    uint8_t temp1;
    uint8_t temp2;

    /* �����Ҫ��ʾ���ַ���ո��ַ���ƫ�� */
    n = ch - ' ';

    /* ��ʾ  */
    for(i=0;i<32;i++)
    {
        temp1 = model32[n*64+i*2];
        temp2 = model32[n*64+i*2+1];
        temp = (temp1<<8) | temp2;

        for(j=0;j<16;j++)
        {
            if(temp & (0x8000 >> j))
            {
                LCD_Point(x+j,y+i,color);
            }
        }
    }
}



/**********************************************************
* @funcName ��LCD_String16
* @brief    ��LCD��Ļ��ʾһ��16*16�ַ���
* @param    ��uint16_t x      (�ַ����Ͻǵ�x���꣬������)
* @param    ��uint16_t y      (�ַ����Ͻǵ�y���꣬������)
* @param    ��uint8_t  *str   (Ҫ��ʾ���ַ���)
* @param    ��uint16_t color  (�ַ���ɫ����ɫ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_String16(uint16_t x,uint16_t y,uint8_t *str,uint16_t color)
{
    while(*str != '\0')
    {
        LCD_Character16(x,y,*str,color);
        x += 8;
        str++;
        if(x+8 > lcddev.width)
        {
            x = 0;
            y += 16;
        }
    }
}



/**********************************************************
* @funcName ��LCD_String24
* @brief    ��LCD��Ļ��ʾһ��24*24�ַ���
* @param    ��uint16_t x      (�ַ����Ͻǵ�x���꣬������)
* @param    ��uint16_t y      (�ַ����Ͻǵ�y���꣬������)
* @param    ��uint8_t  *str   (Ҫ��ʾ���ַ���)
* @param    ��uint16_t color  (�ַ���ɫ����ɫ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_String24(uint16_t x, uint16_t y, uint8_t *str, uint16_t color)
{
    while(*str != '\0')
    {
        LCD_Character24(x,y,*str,color);
        x += 12;
        str++;
        if(x+12 > lcddev.width)
        {
            x = 0;
            y += 24;
        }
    }
}



/**********************************************************
* @funcName ��LCD_String32
* @brief    ��LCD��Ļ��ʾһ��32*32�ַ���
* @param    ��uint16_t x      (�ַ����Ͻǵ�x���꣬������)
* @param    ��uint16_t y      (�ַ����Ͻǵ�y���꣬������)
* @param    ��uint8_t  *str   (Ҫ��ʾ���ַ���)
* @param    ��uint16_t color  (�ַ���ɫ����ɫ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_String32(uint16_t x,uint16_t y,uint8_t *str,uint16_t color)
{
    while(*str != '\0')
    {
        LCD_Character32(x,y,*str,color);
        x += 16;
        str++;
        if(x+16 > lcddev.width)
        {
            x = 0;
            y += 32;
        }
    }
}



/**********************************************************
* @funcName ��LCD_Hanzi32
* @brief    ��LCD��Ļ��ʾһ��32*32����
* @param    ��uint16_t x      (�ַ����Ͻǵ�x���꣬������)
* @param    ��uint16_t y      (�ַ����Ͻǵ�y���꣬������)
* @param    ��uint8_t  *ch    (Ҫ��ʾ�ĺ���)
* @param    ��uint16_t color  (������ɫ����ɫ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_LCD_Hanzi32(uint16_t x,uint16_t y,uint8_t *ch,uint16_t color)
{
    uint8_t n = 0;
    uint8_t i,j,k;
    uint32_t temp=0;

    while(1)
    {
        if((*ch == query[n*2])&&(*(ch+1)==query[n*2+1]))
        {
            break;
        }
        n++;
    }

    for(i=0;i<32;i++)
    {
        for(k=0;k<4;k++)
        {
            temp<<=8;
            temp |= hanzi[16*8*n+k+i*4];
        }
        for(j=0;j<32;j++)
        {
            if(temp & (0x80000000 >> j))
            {
                LCD_Point(x+j,y+i,color);
            }
        }
    }
}



/**********************************************************
* @funcName ��LCD_HanziStr32
* @brief    ��LCD��Ļ��ʾһ��32*32�����ַ���
* @param    ��uint16_t x      (�ַ����Ͻǵ�x���꣬������)
* @param    ��uint16_t y      (�ַ����Ͻǵ�y���꣬������)
* @param    ��uint8_t  *str   (Ҫ��ʾ�ĺ�������)
* @param    ��uint16_t color  (������ɫ����ɫ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_HanziStr32(uint16_t x,uint16_t y,uint8_t *str,uint16_t color)
{
    while(*str != '\0')
    {
        LCD_LCD_Hanzi32(x,y,str,color);
        x += 32;
        str+=2;
        if(x+16 > lcddev.width)
        {
            x = 0;
            y += 32;
        }
    }
}



