#include "spi.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\spi.c
*              .\Drivers\BSP\inc\spi.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-01
* @brief     ��SPI���ô���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



SPI_HandleTypeDef g_spi2_handler; /* SPI2��� */

/**********************************************************
* @funcName ��SPI2_Init
* @brief    ����SPI2ͨ��Ҫ�õ�GPIO�ڽ��г�ʼ���Ͷ�SPI2�ļĴ�����������
* @param    ��void
* @retval   ��void
* @details  ��
*            PB13   SPI2_SCK   ʱ����  ��������
*            PB14   SPI2_MISO  ������  ��������
*            PB15   SPI2_MOSI  �����  ��������
*            ���ԣ�SCKΪ�͵�ƽΪ����״̬
*            ��λ�����ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ
* @fn       ��
************************************************************/
void SPI2_Init(void)
{
    /* ʱ��ʹ��   SPI2 */
    SPI2_SPI_CLK_ENABLE(); /* SPI2ʱ��ʹ�� */

    g_spi2_handler.Instance               = SPI2_SPI;                   /* SPI2 */
    g_spi2_handler.Init.Mode              = SPI_MODE_MASTER;            /* ����Ϊ��SPI(SPI_CR1�Ĵ����ĵ�2��8λ) */
    g_spi2_handler.Init.Direction         = SPI_DIRECTION_2LINES;       /* SPI����Ϊ˫��˫��ȫ˫��,��NSS�ϵĵ�ƽΪ�ߵ�ƽ(SPI_CR1�Ĵ����ĵ�10��15λ) */
    g_spi2_handler.Init.DataSize          = SPI_DATASIZE_8BIT;          /* ʹ��8λ����֡��ʽ���з���/����(SPI_CR1�Ĵ����ĵ�11λ) */
    g_spi2_handler.Init.CLKPolarity       = SPI_POLARITY_LOW;           /* ����״̬ʱ�� SCK���ֵ͵�ƽ(SPI_CR1�Ĵ����ĵ�1λ) */
    g_spi2_handler.Init.CLKPhase          = SPI_PHASE_1EDGE;            /* ���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ(SPI_CR1�Ĵ����ĵ�0λ) */
    g_spi2_handler.Init.NSS               = SPI_NSS_SOFT;               /* ����������豸����NSS�����ϵĵ�ƽ��SSIλ��ֵ����(SPI_CR1�Ĵ����ĵ�9λ) */
    g_spi2_handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;    /* ������Ԥ��ƵֵΪ2  �����ʣ�fPCLK/2  ==  36M/2  ==  18M (SPI_CR1�Ĵ����ĵ�3-5λ) */
    g_spi2_handler.Init.FirstBit          = SPI_FIRSTBIT_MSB;           /* ���ݴ����MSBλ��ʼ(�����׼:�����)(SPI_CR1�Ĵ����ĵ�7λ) */
    g_spi2_handler.Init.TIMode            = SPI_TIMODE_DISABLE;         /* �ر�TIģʽ */
    g_spi2_handler.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE; /* �ر�Ӳ��CRCУ�� */
    g_spi2_handler.Init.CRCPolynomial     = 7;                          /* CRCֵ����Ķ���ʽ:0x7�Ǹ�λֵ(SPI_CRCPR�Ĵ���) */
    HAL_SPI_Init(&g_spi2_handler);                                      /* ��ʼ�� */

    __HAL_SPI_ENABLE(&g_spi2_handler);                                  /* ʹ��SPI2 */
}



/**********************************************************
* @funcName ��HAL_SPI_MspInit
* @brief    ��SPI�ײ��ʼ������(�ײ��ʼ���ص����Ӻ���)
* @param    ��SPI_HandleTypeDef *hspi (SPI�������ָ��)
* @retval   ��void
* @details  ��
* @fn       ��
*        �����������HAL_SPI_Init()�����б����ã���SPI�ײ��
*    ʼ���ص����Ӻ�����
************************************************************/
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef gpio_init_struct;                          /* GPIO���ýṹ�� */

    if (hspi->Instance == SPI2_SPI)
    {
        SPI2_SCK_GPIO_CLK_ENABLE();                             /* SPI2_SCK��ʱ��ʹ�� */
        SPI2_MISO_GPIO_CLK_ENABLE();                            /* SPI2_MISO��ʱ��ʹ�� */
        SPI2_MOSI_GPIO_CLK_ENABLE();                            /* SPI2_MOSI��ʱ��ʹ�� */

        /* SCK����ģʽ����(�������) */
        gpio_init_struct.Pin   = SPI2_SCK_GPIO_PIN;             /* SCK���� */
        gpio_init_struct.Mode  = GPIO_MODE_AF_PP;               /* ���������� */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* IO���ٶ�Ϊ50MHz */
        HAL_GPIO_Init(SPI2_SCK_GPIO_PORT, &gpio_init_struct);   /* ��ʼ�� */

        /* MISO����ģʽ����(�������) */
        gpio_init_struct.Pin = SPI2_MISO_GPIO_PIN;              /* MISO���� */
        HAL_GPIO_Init(SPI2_MISO_GPIO_PORT, &gpio_init_struct);  /* ��ʼ�� */

        /* MOSI����ģʽ����(�������) */
        gpio_init_struct.Pin = SPI2_MOSI_GPIO_PIN;              /* MOSI���� */
        HAL_GPIO_Init(SPI2_MOSI_GPIO_PORT, &gpio_init_struct);  /* ��ʼ�� */
    }
}



/**********************************************************
* @funcName ��SPI2_RS_Byte
* @brief    ��SPI2����/����һ���ֽ�����(8λ)
* @param    ��uint8_t data
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
uint8_t SPI2_RS_Byte(uint8_t data)
{
    /* �жϷ��ͻ������Ƿ�Ϊ�� */
    while(!(SPI2->SR & (1<<1)));
    SPI2->DR = data;
    /* �жϽ��ջ������Ƿ�Ϊ�� */
    while(!(SPI2->SR & (1<<0)));
    data = SPI2->DR;

    return data;
}






