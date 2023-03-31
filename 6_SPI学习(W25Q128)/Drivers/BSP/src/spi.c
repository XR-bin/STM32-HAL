#include "./BSP/inc/spi.h"

SPI_HandleTypeDef g_spi2_handler; /* SPI2��� ��������SPI2��Ϣ��ȫ�ֱ���*/

/**********************************************************
*@funcName ��SPI2_Init
*@brief    ����SPI2ͨ��Ҫ�õ�GPIO�ڽ��г�ʼ���Ͷ�SPI1�ļĴ�����������
*@param    ��void(��)
*@retval   ��void(��)
*@details  ��
************************************************************/
void SPI2_Init(void)
{
    SPI2_SPI_CLK_ENABLE(); /* SPI2ʱ��ʹ�� */

    g_spi2_handler.Instance = SPI2_SPI;                                /* SPI2 */
    g_spi2_handler.Init.Mode = SPI_MODE_MASTER;                        /* ����SPI����ģʽ������Ϊ��ģʽ */
    g_spi2_handler.Init.Direction = SPI_DIRECTION_2LINES;              /* ����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ */
    g_spi2_handler.Init.DataSize = SPI_DATASIZE_8BIT;                  /* ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ */
    g_spi2_handler.Init.CLKPolarity = SPI_POLARITY_HIGH;               /* ����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ */
    g_spi2_handler.Init.CLKPhase = SPI_PHASE_2EDGE;                    /* ����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ����� */
    g_spi2_handler.Init.NSS = SPI_NSS_SOFT;                            /* NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ���� */
    g_spi2_handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; /* ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256 */
    g_spi2_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;                   /* ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ */
    g_spi2_handler.Init.TIMode = SPI_TIMODE_DISABLE;                   /* �ر�TIģʽ */
    g_spi2_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;   /* �ر�Ӳ��CRCУ�� */
    g_spi2_handler.Init.CRCPolynomial = 7;                             /* CRCֵ����Ķ���ʽ */
    HAL_SPI_Init(&g_spi2_handler);                                     /* ��ʼ�� */

    __HAL_SPI_ENABLE(&g_spi2_handler); /* ʹ��SPI2 */
}

/**********************************************************
*@funcName ��HAL_SPI_MspInit
*@brief    ��SPI�ײ��ʼ������(�ײ��ʼ���ص����Ӻ���)
*@param    ��hspi---SPI�������ָ��
*@retval   ��void(��)
*@details  ��
*       �����������HAL_SPI_Init()�����б����ã���SPI�ײ��
*    ʼ���ص����Ӻ�����
*           PB13   SPI2_SCK   ʱ����  ��������
*           PB14   SPI2_MISO  ������  ��������
*           PB15   SPI2_MOSI  �����  ��������
************************************************************/
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    if (hspi->Instance == SPI2_SPI)
    {
        SPI2_SCK_GPIO_CLK_ENABLE();  /* SPI2_SCK��ʱ��ʹ�� */
        SPI2_MISO_GPIO_CLK_ENABLE(); /* SPI2_MISO��ʱ��ʹ�� */
        SPI2_MOSI_GPIO_CLK_ENABLE(); /* SPI2_MOSI��ʱ��ʹ�� */

        /* SCK����ģʽ����(�������) */
        gpio_init_struct.Pin = SPI2_SCK_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SPI2_SCK_GPIO_PORT, &gpio_init_struct);

        /* MISO����ģʽ����(�������) */
        gpio_init_struct.Pin = SPI2_MISO_GPIO_PIN;
        HAL_GPIO_Init(SPI2_MISO_GPIO_PORT, &gpio_init_struct);

        /* MOSI����ģʽ����(�������) */
        gpio_init_struct.Pin = SPI2_MOSI_GPIO_PIN;
        HAL_GPIO_Init(SPI2_MOSI_GPIO_PORT, &gpio_init_struct);
    }
}



/**********************************************************
*@funcName ��SPI2_Set_Speed
*@brief    ��SPI2�ٶ����ú���
*@param    ��speed---SPI2ʱ�ӷ�Ƶϵ��
*@retval   ��void(��)
*@details  ��
*           SPI2ʱ��ѡ������APB1, ��PCLK1, Ϊ36Mhz
*              SPI�ٶ� = PCLK1 / 2^(speed + 1)
************************************************************/
void SPI2_Set_Speed(uint8_t speed)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(speed)); /* �ж���Ч�� */
    __HAL_SPI_DISABLE(&g_spi2_handler);             /* �ر�SPI */
    g_spi2_handler.Instance->CR1 &= 0XFFC7;         /* λ3-5���㣬�������ò����� */
    g_spi2_handler.Instance->CR1 |= speed << 3;     /* ����SPI�ٶ� */
    __HAL_SPI_ENABLE(&g_spi2_handler);              /* ʹ��SPI */
}


/**********************************************************
*@funcName ��SPI2_RW_Byte
*@brief    ��SPI2��дһ���ֽ�����
*@param    ��txdata---Ҫ���͵�����(1�ֽ�)
*@retval   ��void(��)
*@details  ��
************************************************************/
uint8_t SPI2_RW_Byte(uint8_t txdata)
{
    uint8_t rxdata;
    HAL_SPI_TransmitReceive(&g_spi2_handler, &txdata, &rxdata, 1, 1000);   /* ���ݴ�СΪ1���ֽڣ���ʱ1000ms */
    return rxdata; /* �����յ������� */
}


