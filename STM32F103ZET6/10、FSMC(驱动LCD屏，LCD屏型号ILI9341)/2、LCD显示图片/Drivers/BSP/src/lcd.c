#include "lcd.h"
#include "fsmc.h"



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
* @funcName ��LCD_WriteCMD
* @brief    ����LCD�����������
* @param    ��uint8_t cmd(LCDָ��/�Ĵ������/��ַ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_WriteCMD(uint16_t cmd)
{
    LCD->LCD_CMD = cmd;
}



/**********************************************************
* @funcName ��LCD_WriteData
* @brief    ����LCD����������
* @param    ��uint16_t data(����)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_WriteData(uint16_t data)
{
    LCD->LCD_DAT = data;
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
    LCD->LCD_CMD = lcddev.setxcmd;  /* ��������(x����) */
    LCD->LCD_DAT = x >> 8;          /* д����ʼx���� */
    LCD->LCD_DAT = x & 0XFF;

    LCD->LCD_CMD = lcddev.setycmd;  /* ��������(y����) */
    LCD->LCD_DAT = y >> 8;          /* д����ʼy���� */
    LCD->LCD_DAT = y & 0XFF;
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
    LCD->LCD_CMD = lcddev.wramcmd;
    
    /* ��ʼ��ɫ */
    while(i)
    {
        LCD->LCD_DAT = color;     /* ������ɫ���� */
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

    LCD->LCD_CMD = dirreg;
    LCD->LCD_DAT = regval;

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

    LCD->LCD_CMD = lcddev.setxcmd;             /* ��������(x����) */
    LCD->LCD_DAT = 0;                          /* д����ʼx���� */
    LCD->LCD_DAT = 0;
    LCD->LCD_DAT = (lcddev.width - 1) >> 8;    /* д�����x���� */
    LCD->LCD_DAT = (lcddev.width - 1) & 0XFF;

    LCD->LCD_CMD = lcddev.setycmd;             /* ��������(y����) */
    LCD->LCD_DAT = 0;                          /* д����ʼy���� */
    LCD->LCD_DAT = 0;
    LCD->LCD_DAT = (lcddev.height - 1) >> 8;   /* д�����y���� */
    LCD->LCD_DAT = (lcddev.height - 1) & 0XFF;
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
    /* FSMC��ʼ�� */
    FSMC_LCD_Init();

    delay1_ms(50);
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
    LCD->LCD_CMD = lcddev.wramcmd;

    /* ��ʼ��ɫ */
    LCD->LCD_DAT = color;
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
* @funcName ��LCD_Picture1
* @brief    ��LCD��Ļ��ʾһ��ͼƬ(ʹ�����λ��ǰ��ͼƬģ����ǣ�����tu1.c)
* @param    ��uint16_t x      (�����ʼλ�õ�x���꣬������)
* @param    ��uint16_t y      (�����ʼλ�õ�y���꣬������)
* @param    ��uint8_t *buf    (ͼƬ����)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_Picture1(uint16_t x, uint16_t y, uint8_t *buf)
{
    uint32_t i;

    picture *p  = (picture *)buf;
    uint16_t *k;

    /**
    * ע�⣺
    *   ǿ��ת�������и������ע�⣺ǿת�ǽ���һλ����λ��ǰһλ����λ
    *       ���磺uint8_t str[2]={0x22,0x32} ------ǿת------>   uint16_t a[0] = 0x3222
    *       ���磺uint16_t *a = 0x1234       ------ǿת------>   uint8_t *a ��ӡ������˳���� 0x34  0x12
    **/
    k = (uint16_t *)(buf+8);
    i = p->w * p->h;

    /* ȷ��x���� */
    LCD_WriteCMD(lcddev.setxcmd);      /* ��������(x����) */
    LCD_WriteData(x >> 8);             /* д����ʼx���� */
    LCD_WriteData(x);
    LCD_WriteData((x+(p->w)-1) >> 8);  /* д�����x���� */
    LCD_WriteData(x+(p->w)-1);

    /*ȷ��y����*/    
    LCD_WriteCMD(lcddev.setycmd);      /* ��������(y����) */
    LCD_WriteData(y>>8);               /* д����ʼy���� */
    LCD_WriteData(y);
    LCD_WriteData((y+(p->h)-1) >> 8);  /* д�����y���� */
    LCD_WriteData(y+(p->h)-1);

    /* ȷ����ɫ */
    /* ��������(�����ɫ) */
    LCD_WriteCMD(lcddev.wramcmd);
    /* ������ɫ���� */
    while(i)
    {
        LCD_WriteData(*k);     /* ������ɫ���� */
        k++;
        i--;
    }
}



/**********************************************************
* @funcName ��LCD_Picture2
* @brief    ��LCD��Ļ��ʾһ��ͼƬ(ʹ�����λ��ǰ��ͼƬģ����ǣ�����tu2.c)
* @param    ��uint16_t x      (�����ʼλ�õ�x���꣬������)
* @param    ��uint16_t y      (�����ʼλ�õ�y���꣬������)
* @param    ��uint8_t *buf    (ͼƬ����)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_Picture2(uint16_t x, uint16_t y, uint8_t *buf)
{
    uint32_t i;
    uint32_t k = 0;
    picture *p = (picture *)buf;

    uint16_t w = p->w;
    uint16_t h = p->h;

    w=((w & 0x00FF) << 8) | (w >> 8);
    h=((h & 0x00FF) << 8) | (h >> 8);

    i = w * h;

    buf = buf+8;

    /* ȷ��x���� */
    LCD_WriteCMD(lcddev.setxcmd);      /* ��������(x����) */
    LCD_WriteData(x >> 8);             /* д����ʼx���� */
    LCD_WriteData(x);
    LCD_WriteData((x+(w)-1) >> 8);     /* д�����x���� */
    LCD_WriteData(x+(w)-1);

    /*ȷ��y����*/    
    LCD_WriteCMD(lcddev.setycmd);      /* ��������(y����) */
    LCD_WriteData(y>>8);               /* д����ʼy���� */
    LCD_WriteData(y);
    LCD_WriteData((y+(h)-1) >> 8);     /* д�����y���� */
    LCD_WriteData(y+(h)-1);

    /* ȷ����ɫ */
    /* ��������(�����ɫ) */
    LCD_WriteCMD(lcddev.wramcmd);
    /* ������ɫ���� */
    while(i)
    {
        LCD_WriteData((buf[k*2]<<8)|(buf[k*2+1]));     /* ������ɫ���� */
        k++;
        i--;
    }
}



/**********************************************************
* @funcName ��LCD_Picture3
* @brief    ��LCD��Ļ��ʾһ��ͼƬ(ʹ�����λ��ǰ��ͼƬģ����ǣ�����tu1.c)
* @param    ��uint16_t x      (�����ʼλ�õ�x���꣬������)
* @param    ��uint16_t y      (�����ʼλ�õ�y���꣬������)
* @param    ��uint8_t *buf    (ͼƬ����)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_Picture3(uint16_t x, uint16_t y, uint8_t *buf)
{
    picture *p = (picture *)buf;
    uint16_t *k;

    /**
    * ע�⣺
    *   ǿ��ת�������и������ע�⣺ǿת�ǽ���һλ����λ��ǰһλ����λ
    *       ���磺uint8_t str[2]={0x22,0x32} ------ǿת------>   uint16_t a[0] = 0x3222
    *       ���磺uint16_t *a = 0x1234       ------ǿת------>   uint8_t *a ��ӡ������˳���� 0x34  0x12
    **/
    k = (uint16_t *)(buf+8);

    LCD_Color_Fill(x, y, x+p->w-1, x+p->h-1, k);
}





