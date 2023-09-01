#include "oled.h"
#include "iic.h"
#include "matrix.h"



/**********************************************************
* @funcName ��OLED_Write_Command
* @brief    ����OLE������ָ��
* @param    ��uint8_t cmd (ָ��)
* @retval   ��uint8_t    0---�����ɹ�   1---����ʧ��
* @details  ��
* @fn       ��
************************************************************/
uint8_t OLED_Write_Command(uint8_t cmd)
{
    IIC_Start();             /* ��ʼ�ź� */
    IIC_Send_Byte(0x78);     /* ����OLED������ַ�����λ��ʾ��дλ��0Ϊд��1Ϊ�� */

    /* �ж�Ӧ�� */
    if(IIC_Receive_Ack() != 0)
    {
        IIC_Stop();
        return 0;
    }

    IIC_Send_Byte(0x00);         /* 0x00��ʾ��η�����ָ�� */

    /* �ж�Ӧ�� */
    if(IIC_Receive_Ack() != 0)
    {
        IIC_Stop();
        return 0;
    }

    IIC_Send_Byte(cmd);         /* ����ָ�� */

    /* �ж�Ӧ�� */
    if(IIC_Receive_Ack() != 0)
    {
        IIC_Stop();
        return 0;
    }

    IIC_Stop();
    return 1;
}



/**********************************************************
* @funcName ��OLED_Write_Data
* @brief    ����OLE����������
* @param    ��uint8_t data (����)
* @retval   ��uint8_t    0---�����ɹ�   1---����ʧ��
* @details  ��
* @fn       ��
************************************************************/
uint8_t OLED_Write_Data(uint8_t data)
{
    IIC_Start();             /* ��ʼ�ź� */
    IIC_Send_Byte(0x78);     /* ����OLED������ַ�����λ��ʾ��дλ��0Ϊд��1Ϊ�� */

    /* �ж�Ӧ�� */
    if(IIC_Receive_Ack() != 0)
    {
        IIC_Stop();
        return 0;
    }

    IIC_Send_Byte(0x40);         /* 0x40��ʾ��η��������� */

    /* �ж�Ӧ�� */
    if(IIC_Receive_Ack() != 0)
    {
        IIC_Stop();
        return 0;
    }

    IIC_Send_Byte(data);         /* ����ָ�� */

    /* �ж�Ӧ�� */
    if(IIC_Receive_Ack() != 0)
    {
        IIC_Stop();
        return 0;
    }

    IIC_Stop();
    return 1;
}



/**********************************************************
* @funcName ��OLED_WR_Byte
* @brief    ����OLE������ָ��/����
* @param    ��uint8_t Byte (ָ��ֵ/����ֵ)
* @param    ��uint8_t DC (ѡ������ֵ��ָ�������)
*   @arg    ��0 --- ָ��   1 --- ����
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void OLED_WR_Byte(uint8_t Byte, uint8_t DC)
{
    if(DC) OLED_Write_Data(Byte);
    else OLED_Write_Command(Byte);
}



/**********************************************************
* @funcName ��OLED_Clear
* @brief    ����OLE������(��������Ļ�ǰ���)
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
*             0x00~0x0f:  ����8λ��ʼ�е�ַ�ĵ���λ��
*             0x10~0x1f:  ����8λ��ʼ�е�ַ�ĸ���λ��
*             0xb0~0xb7:  ����ҳ��ַ����8ҳ��ÿҳ8�����ص㣩������OLED��128*64�����ص�
*        ע�⣺  
*              ����˵��ҳָ�����У�һҳ���ư��У�һ����8ҳ(64��)
*              �������������ָ����ƣ��ֱ���Ƶ���λ�͸���λ��һ����128��
*              ��������ʱ��һ���ֽ����ݿ���8��1�е�״̬
************************************************************/
void OLED_Clear(void)
{
    uint8_t i,j;
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte(0xb0+i,OLED_CMD);   /* ѡ��0+iҳ(����8*i ~ 8*(i+1)-1��) */
        OLED_WR_Byte(0x00,OLED_CMD);     /* ������ʾλ�á��е͵�ַ */
        OLED_WR_Byte(0x10,OLED_CMD);     /* ������ʾλ�á��иߵ�ַ  �ӵ�0�п�ʼ */
        for(j=0;j<128;j++) OLED_WR_Byte(0x00,OLED_DATA); /* һ���ֽڿ���8��1�� */
    }
}



/**********************************************************
* @funcName ��OLED_XY
* @brief    ��������ʾ��������
* @param    ��uint8_t x (��ʼ���x����)
* @param    ��uint8_t y (��ʼ���y����)
* @retval   ��void
* @details  ��
* @fn       ��
*             0x00~0x0f:  ����8λ��ʼ�е�ַ�ĵ���λ��
*             0x10~0x1f:  ����8λ��ʼ�е�ַ�ĸ���λ��
*             0xb0~0xb7:  ����ҳ��ַ����8ҳ��ÿҳ8�����ص㣩������OLED��128*64�����ص�
*        ע�⣺  
*              ����˵��ҳָ�����У�һҳ���ư��У�һ����8ҳ(64��)
*              �������������ָ����ƣ��ֱ���Ƶ���λ�͸���λ��һ����128��
*              ��������ʱ��һ���ֽ����ݿ���8��1�е�״̬
************************************************************/
void OLED_XY(uint8_t x,uint8_t y)
{
    OLED_WR_Byte(0xb0+y,OLED_CMD);           /* ������һҳ(�İ���) */
    OLED_WR_Byte(0x00|(x&0x0f),OLED_CMD);    /* ������һ�еĵ�8λ */
    OLED_WR_Byte(0x10|(x>>4),OLED_CMD);      /* ������һ�еĸ�8λ */
}



/**********************************************************
* @funcName ��OLED_Open
* @brief    ������OLED��
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
*             0x8Dָ����0X14����0x10:  �������߹رյ�ɱá�
*             0xAE��0xAF:  ������Ļ��ʾ�أ�˯��ģʽ��������ʾ��������ģʽ��
************************************************************/
void OLED_Open(void)
{
    /* ������ɱ� */
    OLED_WR_Byte(0x8D,OLED_CMD);
    OLED_WR_Byte(0x14,OLED_CMD);
    /* ��OLED������������ģʽ */
    OLED_WR_Byte(0xAF,OLED_CMD);
}



/**********************************************************
* @funcName ��OLED_Close
* @brief    ���ر�OLED��
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
*             0x8Dָ����0X14����0x10:  �������߹رյ�ɱá�
*             0xAE��0xAF:  ������Ļ��ʾ�أ�˯��ģʽ��������ʾ��������ģʽ��
************************************************************/
void OLED_Close(void)
{
    /* �رյ�ɱ� */
    OLED_WR_Byte(0x8D,OLED_CMD);
    OLED_WR_Byte(0x10,OLED_CMD);
    /* ��OLED����˯��ģʽ */
    OLED_WR_Byte(0xAE,OLED_CMD);
}



/**********************************************************
* @funcName ��OLED_Init
* @brief    ��OLED��ʽ��ʼ��
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void OLED_Init(void)
{
    /* GPIOx��ʼ�� */
    IIC_Init();

    delay_ms(5);

    /*���Ҵ���*/
    OLED_WR_Byte(0xAE,OLED_CMD);    /* �ر���ʾ */
    OLED_WR_Byte(0x00,OLED_CMD);    /* ���õ��е�ַ */
    OLED_WR_Byte(0x10,OLED_CMD);    /* ���ø��е�ַ */
    OLED_WR_Byte(0x40,OLED_CMD);    /* ������ʼ�е�ַ */
    OLED_WR_Byte(0xB0,OLED_CMD);    /* ����ҳ��ַ */

    OLED_WR_Byte(0x81,OLED_CMD);    /* �Աȶ����ã����������� */
    OLED_WR_Byte(0xFF,OLED_CMD);    /* 265 */

    OLED_WR_Byte(0xA1,OLED_CMD);    /* ���ö�(SEG)����ʼӳ���ַ */
    OLED_WR_Byte(0xA6,OLED_CMD);    /* ������ʾ��0xa7����ʾ */

    OLED_WR_Byte(0xA8,OLED_CMD);    /* ��������·����16~64�� */
    OLED_WR_Byte(0x3F,OLED_CMD);    /* 64duty */

    OLED_WR_Byte(0xC8,OLED_CMD);    /* ��ӳ��ģʽ��COM[N-1]~COM0ɨ�� */

    OLED_WR_Byte(0xD3,OLED_CMD);    /* ������ʾƫ�� */
    OLED_WR_Byte(0x00,OLED_CMD);    /* ��ƫ�� */

    OLED_WR_Byte(0xD5,OLED_CMD);    /* ����������Ƶ */
    OLED_WR_Byte(0x80,OLED_CMD);    /* ʹ��Ĭ��ֵ */

    OLED_WR_Byte(0xD9,OLED_CMD);    /* ���� Pre-Charge Period */
    OLED_WR_Byte(0xF1,OLED_CMD);    /* ʹ�ùٷ��Ƽ�ֵ */

    OLED_WR_Byte(0xDA,OLED_CMD);    /* ���� com pin configuartion */
    OLED_WR_Byte(0x12,OLED_CMD);    /* ʹ��Ĭ��ֵ */

    OLED_WR_Byte(0xDB,OLED_CMD);    /* ���� Vcomh���ɵ������ȣ�Ĭ�ϣ� */
    OLED_WR_Byte(0x40,OLED_CMD);    /* ʹ�ùٷ��Ƽ�ֵ */

    OLED_WR_Byte(0x8D,OLED_CMD);    /* ����OLED��ɱ� */
    OLED_WR_Byte(0x14,OLED_CMD);    /* ����ʾ */

    OLED_WR_Byte(0xAF,OLED_CMD);    /* ����OLED�����ʾ */

    /*����*/
    OLED_Clear();
    /*��ʼ����*/
    OLED_XY(0,0);
}



/**********************************************************
* @funcName ��OLED_ShowChar16
* @brief    ��OLED��ʾһ��16*16���ַ�
* @param    ��uint16_t x  (��ʼλ��x����)
* @param    ��uint16_t y  (��ʼλ��y����)
* @param    ��uint16_t ch (Ҫ��ʾ���ַ�)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void OLED_ShowChar16(uint16_t x,uint8_t y, uint8_t ch)
{
    uint8_t i,c;

    /* �����ַ�ƫ���� */
    c = ch - ' ';

    /* �ж��Ƿ񳬳����� */
    if(x+16 > (OLED_LINE-1))
    {
        y=y+(16/8)*((x+16)/(OLED_LINE-1));   /* yֵ��Ҫ����7 */
        if((x+16-(OLED_LINE-1))<16)x=0;
        else x=(x+16-(OLED_LINE-1))/16*16;
    }

    /* ע�⣺16���ַ���2ҳ��� */
    /* ȷ������ */
    OLED_XY(x,y);
    /* ��ʾ��һҳ���ַ�����(1ҳ=8��) */
    for(i=0;i<8;i++)
    OLED_WR_Byte(CH16[c*16+i],OLED_DATA);
    /* ȷ������ */
    OLED_XY(x,y+1);
    /* ��ʾ��һҳ���ַ�����(1ҳ=8��) */
    for(i=0;i<8;i++)
    OLED_WR_Byte(CH16[c*16+i+8],OLED_DATA);
}



/**********************************************************
* @funcName ��OLED_ShowString16
* @brief    ��OLED��ʾһ��16*16���ַ���
* @param    ��uint16_t x    (��ʼλ��x����)
* @param    ��uint16_t y    (��ʼλ��y����)
* @param    ��uint16_t *str (Ҫ��ʾ���ַ���)
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void OLED_ShowString16(uint8_t x,uint8_t y,uint8_t *str)
{
    uint8_t i=0;

    while(*str != '\0')
    {
        OLED_ShowChar16(x+i*(16/2),y,*str);
        str++;
        i++;
    }
}


