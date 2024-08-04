#include "touch.h"
#include "lcd.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\touch.c
*              .\Drivers\BSP\inc\touch.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-03
* @brief     �����败�����������úͲ�������
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



_m_tp_dev tp_dev =
{
    Touch_Init,               /* ��������ʼ�� */
    LCD_Touch_Calibration,    /* ������У׼ */
    Touch_Scan,               /* ��ȡ�������� */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};




/**********************************************************
* @funcName ��Touch_IO_Init
* @brief    ���Ե��败����оƬ��Ӧ��GPIO�ڽ��г�ʼ������
* @param    ��void
* @retval   ��void
* @details  ��
*            CS     PF11  �������
*            SCK    PB1   �������
*            PEN    PF10  ͨ������
*            MISO   PB2   ͨ������
*            MOSI   PF9   �������
* @fn       ��
************************************************************/
void Touch_IO_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;    /* GPIO���ýṹ�� */

    /* ʱ��ʹ�� */
    T_PEN_GPIO_CLK_ENABLE();    /* T_PEN��ʱ��ʹ�� */
    T_CS_GPIO_CLK_ENABLE();     /* T_CS��ʱ��ʹ�� */
    T_MISO_GPIO_CLK_ENABLE();   /* T_MISO��ʱ��ʹ�� */
    T_MOSI_GPIO_CLK_ENABLE();   /* T_MOSI��ʱ��ʹ�� */
    T_SCK_GPIO_CLK_ENABLE();    /* T_SCK��ʱ��ʹ�� */

    /* GPIOx��ʼ������ */
    /* GPIOx�˿����� */
    /* T_PEN */
    gpio_init_struct.Pin   = T_PEN_GPIO_PIN;                 /* PF10 �˿����� */
    gpio_init_struct.Mode  = GPIO_MODE_INPUT;                /* ���� */
    gpio_init_struct.Pull  = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;           /* ���� */
    HAL_GPIO_Init(T_PEN_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��T_PEN���� */
    /* T_MISO */
    gpio_init_struct.Pin = T_MISO_GPIO_PIN;                  /* PB2 */
    HAL_GPIO_Init(T_MISO_GPIO_PORT, &gpio_init_struct);      /* ��ʼ��T_MISO���� */
    /* T_MOSI */
    gpio_init_struct.Pin = T_MOSI_GPIO_PIN;                  /* PF9 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;             /* ������� */
    HAL_GPIO_Init(T_MOSI_GPIO_PORT, &gpio_init_struct);      /* ��ʼ��T_MOSI���� */
    /* SCK */
    gpio_init_struct.Pin = T_SCK_GPIO_PIN;                   /* PB1 */
    HAL_GPIO_Init(T_SCK_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��T_CLK���� */
    /* CS */
    gpio_init_struct.Pin = T_CS_GPIO_PIN;                    /* F11 */
    HAL_GPIO_Init(T_CS_GPIO_PORT, &gpio_init_struct);        /* ��ʼ��T_CS���� */

    /* ��ʼ��״̬���� */
    T_CS_H;
    T_SCK_L;
    T_MOSI_L;
}



/**********************************************************
* @funcName ��Touch_Data
* @brief    ���Ե��败����оƬ��Ӧ��GPIO�ڽ��г�ʼ������
* @param    ��uint8_t command (ָ��)
* @retval   ��uint16_t
* @details  ��
*            Ƭѡ��        T_CS_H       T_CS_L  
*            ʱ����        T_SCK_H      T_SCK_L  
*            ���������    T_MOSI_H     T_MOSI_L
*            ����������    T_MISO
*            �ʽӴ���      T_PEN
* @fn       ��
************************************************************/
uint16_t Touch_Data(uint8_t command)
{
    uint8_t i;
    uint16_t data = 0;

    T_MOSI_L;                        /* ���������� */
    T_CS_L;                          /* Ƭѡʹ�� */

    for(i=0;i<8;i++)
    {
        T_SCK_L;                     /* ʱ�������� */

        /* �ж�ָ��λ��ƽ */
        if(command & 0x80)T_MOSI_H;
        else T_MOSI_L;

        command <<= 1;               /* ָ���������� */

        T_SCK_H;                     /* ʱ�������� */
        delay_us(1);                 /* ��ʱ�����ڴӻ���ȡ���� */
    }
    T_SCK_L;                         /* ʱ�������� */
    T_MOSI_L;                        /* ���������� */
    delay_us(6);                     /* ��ʱ6us�����ڵȴ�ADC������ת���� */

    /* �ճ�һ�������� */
    T_SCK_L;
    delay_us(1);
    T_SCK_H;
    delay_us(1);

    for(i=0;i<12;i++)
    {
        data <<= 1;                  /* ָ���������� */
        T_SCK_L;                     /* ʱ�������� */
        delay_us(1);                 /* ��ʱ���������ݽ��� */
        T_SCK_H;                     /* ʱ�������� */
        if(T_MISO) data |= 0x1;      /* ��ȡ����λ��ƽ */
        delay_us(1);                 /* ��ʱ���������ݽ��� */
    }

    T_CS_H;                          /* Ƭѡ���� */
    T_SCK_L;                         /* ʱ�������� */

    return data;
}



/**********************************************************
* @funcName ��Touch_Smoothing
* @brief    �����败���������˲�����
* @param    ��void
* @retval   ��TOUCH_XY
* @details  ��
* @fn       ��
************************************************************/
TOUCH_XY Touch_Smoothing(void)
{
    uint8_t i,j;
    uint16_t touch_x[TP_READ_TIMES];
    uint16_t touch_y[TP_READ_TIMES];
    uint16_t sum_x = 0,sum_y = 0;
    TOUCH_XY mean;

    /* ��ȡTP_READ_TIMES���������� */
    for(i=0;i<TP_READ_TIMES;i++)
    {
        touch_x[i] = Touch_Data(tp_dev.comX);
        touch_y[i] = Touch_Data(tp_dev.comY);
    }

    /* �����ݽ�����������(ð������) */
    for(i=1; i<TP_READ_TIMES; i++)
    {
        for(j=0; j<TP_READ_TIMES-i; j++)
        {
            /* x�������� */
            if(touch_x[j] > touch_x[j+1])
            {
                touch_x[j] ^= touch_x[j+1];
                touch_x[j+1] ^= touch_x[j];
                touch_x[j] ^= touch_x[j+1];
            }

            /* y�������� */
            if(touch_y[j]>touch_y[j+1])
            {
                touch_y[j] ^= touch_y[j+1];
                touch_y[j+1] ^= touch_y[j];
                touch_y[j] ^= touch_y[j+1];
            }
        }
    }

    /* ȥ��TP_LOST_VAL��������С��ȡƽ��ֵ */
    for(i=TP_LOST_VAL; i<TP_READ_TIMES - TP_LOST_VAL; i++)
    {
        sum_x += touch_x[i];
        sum_y += touch_y[i];
    }

    mean.x = sum_x / (TP_READ_TIMES - TP_LOST_VAL);
    mean.y = sum_y / (TP_READ_TIMES - TP_LOST_VAL);

    return mean;
}



/**********************************************************
* @funcName ������У׼��ʮ����ʾ����
* @brief    ��LCD_Calibration_Cross
* @param    ��uint16_t x     (ʮ�ֵ����ĵ�x����)
* @param    ��uint16_t y     (ʮ�ֵ����ĵ�y����)
* @param    ��uint16_t color (ʮ�ֵ���ɫ)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_Calibration_Cross(uint16_t x, uint16_t y, uint16_t color)
{
    uint16_t i;

    for(i=x-10;i<x+10;i++)
    {
        LCD_Point(i,y,color);
    }
    for(i=y-10;i<y+10;i++)
    {
        LCD_Point(x,i,color);
    }
}



/**********************************************************
* @funcName ��LCD�봥����������ϵ����ĻУ׼
* @brief    ��LCD_Touch_Calibration
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void LCD_Touch_Calibration(void)
{
    uint8_t i;
    TOUCH_XY touch_xy[4];
    /* {20,20} {220, 20} {20,300} {220,300} */
    TOUCH_XY lcd_xy[4] = {\
                          {20,20},\
                          {lcddev.width-20,20},\
                          {20, lcddev.height-20},\
                          {lcddev.width-20,lcddev.height-20}\
                         };/* ����У׼��LCD���������*/
    double temp1,temp2;

THIS:  
    /***************��ȡУ׼��������***************/
    /* ͨ���ض���LCD���������ȡ������Ĵ��������� */
    /* ��������������ʱ���� */
    /* LCD��0��У׼���������(20,20) */
    /* LCD��1��У׼���������(220,20) */
    /* LCD��2��У׼���������(20,300) */
    /* LCD��3��У׼���������(220,300) */
    for(i=0;i<4;i++)
    {
        LCD_Calibration_Cross(lcd_xy[i].x,lcd_xy[i].y,RED);      /* ����i������ʮ�� */
        while(T_PEN);                                            /* �ȴ����������� */
        delay_ms(10);                                            /* ��ʱ10ms�������¶��� */
        touch_xy[i] = Touch_Smoothing();                         /* ��ȡ���������� */
        while(!T_PEN);                                           /* �ȴ�̧�� */
        LCD_Calibration_Cross(lcd_xy[i].x,lcd_xy[i].y,WHITE);    /* ��������ʮ�� */
        delay_ms(150);                                           /* ��ʱ100ms������̧�𶶶� */
    }

    /****************�жϵ����Ч��****************/
    /* ���0����1�ľ���ƽ��temp1 */
    temp1 = (touch_xy[0].x - touch_xy[1].x)*(touch_xy[0].x - touch_xy[1].x) + (touch_xy[0].y - touch_xy[1].y)*(touch_xy[0].y - touch_xy[1].y);
    /* ���2����3�ľ���ƽ��temp2 */
    temp2 = (touch_xy[2].x - touch_xy[3].x)*(touch_xy[2].x - touch_xy[3].x) + (touch_xy[2].y - touch_xy[3].y)*(touch_xy[2].y - touch_xy[3].y);
    /* �ж��Ƿ�����Χ�� */
    if(((temp1/temp2)<CORRECT_RANGE_MIN) || ((temp1/temp2)>CORRECT_RANGE_MAX))
    {
        /* ����ȡ������ */
        goto THIS;
    }

    /* ���0����2�ľ���ƽ��temp1 */
    temp1 = (touch_xy[0].x - touch_xy[2].x)*(touch_xy[0].x - touch_xy[2].x) + (touch_xy[0].y - touch_xy[2].y)*(touch_xy[0].y - touch_xy[2].y);
    /* ���1����3�ľ���ƽ��temp2 */
    temp2 = (touch_xy[1].x - touch_xy[3].x)*(touch_xy[1].x - touch_xy[3].x) + (touch_xy[1].y - touch_xy[3].y)*(touch_xy[1].y - touch_xy[3].y);
    /* �ж��Ƿ�����Χ�� */
    if(((temp1/temp2)<CORRECT_RANGE_MIN) || ((temp1/temp2)>CORRECT_RANGE_MAX))
    {
        /* ����ȡ������ */
        goto THIS;
    }

    /* ���0����3�ľ���ƽ��temp1 */
    temp1 = (touch_xy[0].x - touch_xy[3].x)*(touch_xy[0].x - touch_xy[3].x) + (touch_xy[0].y - touch_xy[3].y)*(touch_xy[0].y - touch_xy[3].y);
    /* ���1����2�ľ���ƽ��temp2 */
    temp2 = (touch_xy[1].x - touch_xy[2].x)*(touch_xy[1].x - touch_xy[2].x) + (touch_xy[1].y - touch_xy[2].y)*(touch_xy[1].y - touch_xy[2].y);
    /* �ж��Ƿ�����Χ�� */
    if(((temp1/temp2)<CORRECT_RANGE_MIN) || ((temp1/temp2)>CORRECT_RANGE_MAX))
    {
        /* ����ȡ������ */
        goto THIS;
    }

    /***************��ʼ����LCD�ʹ������Ĺ�ϵ��������***************/
    /***
    * lcd�ʹ�������xy�����ƫ����
    * lcd�ʹ�������xy����ı���ϵ��
    * ȡ������������(LCD�봥����)�����ϵ��(ȡ���Σ���ƽ��ֵ����С���)
    * ע��:ֻ��ԶԽ��ߵĵ�
    * ��ʽ: xfac = (LCD_x1 - LED_x2) / (��x1 - ��x2)
    *       yfac = (LCD_y1 - LED_y2) / (��y1 - ��y2)
    * ��ע: xfac��yfac������ϵ��
    *      LCD_x1��LCD_y1��LCD��һ������������
    *      LCD_x2��LCD_y2��LCD�ڶ�������������
    *      ��x1����y1����������һ������������
    *      ��x2����y2���������ڶ�������������
    */
    tp_dev.xfac = ((float)(lcddev.width-20) - 20.0) / (touch_xy[3].x - touch_xy[0].x);
    tp_dev.xfac += (20.0 - (float)(lcddev.width-20)) / (touch_xy[2].x - touch_xy[1].x);
    tp_dev.xfac /= 2;
    tp_dev.yfac = ((float)(lcddev.height-20) - 20.0) / (touch_xy[3].y - touch_xy[0].y);
    tp_dev.yfac += ((float)(lcddev.height-20) - 20.0) / (touch_xy[2].y - touch_xy[1].y);
    tp_dev.yfac /= 2;

    /***
    * ��ȡlcd�ʹ�������xy�����ƫ����
    * �Ѿ��������ϵ����ֻ��ȡһ�����������(LCD�ʹ�����)�Ϳ�����ƫ����(ȡ���Σ���ƽ��ֵ����С���)
    * ��ʽ: xc = LCD_x - xfac * ��x
    *       yc = LCD_y - yfac * ��y
    * ��ע: b_x��d_y��x��y��ƫ����
    *       xc��yc������ϵ��
    *       LCD_x��LCD_y��LCDһ��У׼��������������
    *       ��x����y��������һ��У׼��������������
    */
    tp_dev.xc = 20 - tp_dev.xfac * touch_xy[0].x;
    tp_dev.xc += (lcddev.width-20) - tp_dev.xfac * touch_xy[3].x;
    tp_dev.xc /= 2;
    tp_dev.yc = 20 - tp_dev.yfac * touch_xy[0].y;
    tp_dev.yc += (lcddev.height-20) - tp_dev.yfac * touch_xy[3].y;
    tp_dev.yc /= 2;
}



/**********************************************************
* @funcName ����ȡ���败����������������
* @brief    ��Touch_Read_XY
* @param    ��OUCH_XY* coord (������������)
* @retval   ��uint8_t --- (1��Ч���� 0��Ч����)
* @details  ��
* @fn       ��
************************************************************/
uint8_t Touch_Read_XY(TOUCH_XY* coord)
{
    TOUCH_XY touch_xy1;
    TOUCH_XY touch_xy2;

    //��ȡ����������
    touch_xy1 = Touch_Smoothing();
    touch_xy2 = Touch_Smoothing();

    /* ǰ�����β�����+-TP_ERR_RANGE�� */
    if (((touch_xy2.x <= touch_xy1.x && touch_xy1.x < touch_xy2.x + TP_ERR_RANGE)  || \
         (touch_xy1.x <= touch_xy2.x && touch_xy2.x < touch_xy1.x + TP_ERR_RANGE)) && \
        ((touch_xy2.y <= touch_xy1.y && touch_xy1.y < touch_xy2.y + TP_ERR_RANGE)  || \
         (touch_xy1.y <= touch_xy2.y && touch_xy2.y < touch_xy1.y + TP_ERR_RANGE)))
    {
        coord->x = (touch_xy1.x + touch_xy2.x) / 2;
        coord->y = (touch_xy1.y + touch_xy2.y) / 2;
        
        return 1;
    }

    return 0;
}




/**********************************************************
* @funcName ����ȡ���败������������(�����������Ļ����)
* @brief    ��Touch_Scan
* @param    ��uint8_t mode
*   @arg    ��0 --- ��ȡ��������   1 --- ��ȡ��Ļ����
* @retval   ��TOUCH_XY --- (����)
* @details  ��
* @fn       ��
************************************************************/
TOUCH_XY Touch_Scan(uint8_t mode)
{
    TOUCH_XY coord;
    coord.x=0;
    coord.y=0;
    
    if(T_PEN == 0)
    {
        if(mode == 0)                /* �������������� */
        {
            Touch_Read_XY(&coord);
        }
        else                         /* ��Ļ���� */
        {
            if(Touch_Read_XY(&coord))
            {
                coord.x = (uint16_t)(tp_dev.xfac * coord.x + tp_dev.xc);
                coord.y = (uint16_t)(tp_dev.yfac * coord.y + tp_dev.yc);
            }
        }
    }
    
    return coord;
}



/**********************************************************
* @funcName ����������ʼ��
* @brief    ��Touch_Init
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void Touch_Init(void)
{
    tp_dev.touchtype = 0;                   /* Ĭ������(������ & ����) */
    tp_dev.touchtype |= lcddev.dir & 0X01;  /* ����LCD�ж��Ǻ����������� */
    
    Touch_IO_Init();                        /* ����������������� */
    
    if(tp_dev.touchtype & 0X01)             /* ����Ǻ��� */
    {
        tp_dev.comX = TOUCH_Y;
        tp_dev.comY = TOUCH_X;
    }
    else
    {
        tp_dev.comX = TOUCH_X;
        tp_dev.comY = TOUCH_Y;
    }
}


