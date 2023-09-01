#include "fsmc.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\fsmc.c
*              .\Drivers\BSP\inc\fsmc.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-01
* @brief     ��FSMC���ô���
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



SRAM_HandleTypeDef g_sram_handle;    /* SRAM���(���ڿ���LCD) */

/**********************************************************
* @funcName ��FSMC_LCD_Init
* @brief    ����LCD��Ӧ��GPIO�ڽ��г�ʼ������,���ض���GPIO�ڽ���FSMC����
* @param    ��void
* @retval   ��void
* @details  ��
*     �����ߣ�
*           DO0---PD14    DO1---PD15    DO2---PD0    DO3---PD1
*           DO4---PE7     DO5---PE8     DO6---PE9    DO7---PE10
*           DO8---PE11    DO9---PE12    DO10---PE13  DO11---PE14
*           DO12---PE15   DO13---PD8    DO14---PD9   DO15---PD10
*
*     �����ߣ�
*           WR---PD5    RD---PD4    CS---PG12
*           RS/DC(����/����)---PG0
*           BL�����⣩--PB0
*     ����
*         ���츴��
*               PD��0-1   4-5   8-10   14-15  
*               PE��7-15
*               PG��0   12
*         �������
*               PB��0
*
*     ��Ҫ���õ���FSMC��Bank1������4
* @fn       ��
************************************************************/
void FSMC_LCD_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;                          /* GPIO���ýṹ�� */
    FSMC_NORSRAM_TimingTypeDef fsmc_read_handle;                /* Ƭѡʱ��Ĵ������ýṹ��(��ʱ����ƼĴ���) */
    FSMC_NORSRAM_TimingTypeDef fsmc_write_handle;               /* д��ʱ��Ĵ������ýṹ��(дʱ����ƼĴ���) */

    /* ʱ��ʹ��   GPIOB  GPIOD  GPIOE  GPIOG  FSMC */
    __HAL_RCC_GPIOB_CLK_ENABLE();           /* ʹ��GPIOBʱ�� */
    __HAL_RCC_GPIOD_CLK_ENABLE();           /* ʹ��GPIODʱ�� */
    __HAL_RCC_GPIOE_CLK_ENABLE();           /* ʹ��GPIOEʱ�� */
    __HAL_RCC_GPIOG_CLK_ENABLE();           /* ʹ��GPIOGʱ�� */
    __HAL_RCC_FSMC_CLK_ENABLE();            /* ʹ��FSMCʱ�� */

    /***************GPIOx�Ĵ�������***************/
    /* PB0 */
    /* ����ƣ�����ͨ����� */
    /* �˿����üĴ��� */
    gpio_init_struct.Pin   = GPIO_PIN_0;                /* PB0 �˿����� */
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_PP;       /* ������� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* IO���ٶ�Ϊ50MHz */
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);            /* �����趨������ʼ��PB0 */
    /* PD0/1/4/5/8/9/10/14/15 */
    /* ���ø������ */
    /* �˿����üĴ��� */
    gpio_init_struct.Pin  = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;    /* PD0/1/4/5/8/9/10/14/15 �˿����� */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* ������� */
    HAL_GPIO_Init(GPIOD, &gpio_init_struct);            /* �����趨������ʼ��PD0/1/4/5/8/9/10/14/15 */
    /* PE7-15 */
    /* ���ø������ */
    /* �˿����üĴ��� */
    gpio_init_struct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;  /* PE7-15 �˿����� */
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);            /* �����趨������ʼ��PE7-15 */
    /* PG0/12 */
    /* ���ø������ */
    /* �˿����üĴ��� */
    gpio_init_struct.Pin = GPIO_PIN_0|GPIO_PIN_12;      /* PG0/12 �˿����� */
    HAL_GPIO_Init(GPIOG, &gpio_init_struct);            /* �����趨������ʼ��PG0/12 */

    /********** �ر��� **********/
    GPIOB->ODR &= ~(1<<0); 

    /***************FSMC�Ĵ�������***************/
    /***
    * ע�⣺bank1��NE1~4,ÿһ��������һ��BCR+TCR+BWTR�������ܹ�12���Ĵ�����
    *
    *      FSMC_BCRx �� FSMC_BTRx����ϳ� BTCR[8]�Ĵ����飬���ǵĶ�Ӧ��ϵ���£�
    *      BTCR[0]��Ӧ FSMC_BCR1��BTCR[1]��Ӧ FSMC_BTR1
    *      BTCR[2]��Ӧ FSMC_BCR2��BTCR[3]��Ӧ FSMC_BTR2
    *      BTCR[4]��Ӧ FSMC_BCR3��BTCR[5]��Ӧ FSMC_BTR3
    *      BTCR[6]��Ӧ FSMC_BCR4��BTCR[7]��Ӧ FSMC_BTR4
    *
    *      FSMC_BWTRx ����ϳ� BWTR[7]�����ǵĶ�Ӧ��ϵ���£�
    *      BWTR[0]��Ӧ FSMC_BWTR1��BWTR[2]��Ӧ FSMC_BWTR2��
    *      BWTR[4]��Ӧ FSMC_BWTR3��BWTR[6]��Ӧ FSMC_BWTR4��
    *      BWTR[1]��BWTR[3]�� BWTR[5]������û���õ���
    */
    /* SRAM/NOR-Flash Ƭѡʱ��Ĵ��� ����ʱ����ƼĴ�����*/
    fsmc_read_handle.AddressSetupTime      = 0;                      /* ��ַ����ʱ��(ADDSET)Ϊ1��HCLK 1/72M = 13.9ns (ʵ�� > 200ns) (FSMC->BTR�Ĵ����ĵ�0-3λ) */
    fsmc_read_handle.AddressHoldTime       = 0;                      /* ��ַ����ʱ��(ADDHLD) ģʽA��û���õ� (FSMC->BTR�Ĵ����ĵ�4-7λ) */
    /* ��ΪҺ������IC�Ķ����ݵ�ʱ���ٶȲ���̫��,�����Ǹ�������оƬ */
    fsmc_read_handle.DataSetupTime         = 15;                     /* ���ݱ���ʱ��(DATAST)Ϊ16��HCLK = 13.9 * 16 = 222.4ns (FSMC->BTR�Ĵ����ĵ�8-15λ) */
    fsmc_read_handle.BusTurnAroundDuration = 0;                      /* ���߻ָ�ʱ��=1��HCLKʱ������ (FSMC->BTR�Ĵ����ĵ�16-19λ) */
    fsmc_read_handle.CLKDivision           = 0;                      /* CLK �źŵ�ʱ�ӷ�Ƶ�ȣ����� (FSMC->BTR�Ĵ����ĵ�20-23λ) */
    fsmc_read_handle.DataLatency           = 0;                      /* ͬ��ͻ�� NOR Flash �������ӳ٣�2�� CLK ʱ�����ڵ������ӳ� (FSMC->BTR�Ĵ����ĵ�24-27λ) */
    fsmc_read_handle.AccessMode            = FSMC_ACCESS_MODE_A;     /* ����ģʽ������ģʽ A (FSMC->BTR�Ĵ����ĵ�28-29λ) */

    /* SRAM/NOR-Flash д��ʱ��Ĵ���   (дʱ����ƼĴ���) */
    fsmc_write_handle.AddressSetupTime      = 0;                     /* ��ַ����ʱ��(ADDSET)Ϊ1��HCLK = 13.9ns (FSMC->BWTR�Ĵ����ĵ�0-3λ) */
    fsmc_write_handle.AddressHoldTime       = 0;                     /* ��ַ����ʱ��(ADDHLD) ģʽA��û���õ� (FSMC->BWTR�Ĵ����ĵ�4-7λ) */
    fsmc_write_handle.DataSetupTime         = 1;                     /* ���ݱ���ʱ��(DATAST)Ϊ2��HCLK = 13.9 * 2 = 27.8ns (ʵ�� > 200ns) (FSMC->BWTR�Ĵ����ĵ�8-15λ) */
    fsmc_write_handle.BusTurnAroundDuration = 0;                     /* ������ת�׶εĳ���ʱ�䣺0��HCLK ʱ������ (FSMC->BWTR�Ĵ����ĵ�16-19λ) */
    fsmc_write_handle.CLKDivision           = 0;                     /* CLK �źŵ�ʱ�ӷ�Ƶ�ȣ� ���� (FSMC->BWTR�Ĵ����ĵ�20-23λ) */
    fsmc_write_handle.DataLatency           = 0;                     /* ͬ��ͻ�� NOR Flash �������ӳ٣�2 �� CLK ʱ�����ڵ������ӳ� (FSMC->BWTR�Ĵ����ĵ�24-27λ) */
    /* ĳЩҺ������IC��д�ź���������Ҳ��50ns�� */
    fsmc_write_handle.AccessMode            = FSMC_ACCESS_MODE_A;    /* ����ģʽ������ģʽ A (FSMC->BWTR�Ĵ����ĵ�28-29λ) */

    /* SRAM/NOR-Flash Ƭѡ���ƼĴ��� */
    /* ѡ�����bank1 */
    g_sram_handle.Instance = FSMC_NORSRAM_DEVICE;
    g_sram_handle.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
    /* ����bank��ؼĴ��� */
    g_sram_handle.Init.NSBank             = FSMC_NORSRAM_BANK4;             /* ѡ���������bank1(��1)��NE4(����4)Ҳ�Ͷ�ӦBTCR[6],[7]�� */
    g_sram_handle.Init.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;  /* ��ַ/���ݸ���ʹ��λ����ʹ�� (FSMC->BCR�Ĵ����ĵ�1λ) */
    g_sram_handle.Init.MemoryDataWidth    = FSMC_NORSRAM_MEM_BUS_WIDTH_16;  /* �洢���������߿�ȣ�16bit  (FSMC->BCR�Ĵ����ĵ�4-5λ) */
    g_sram_handle.Init.BurstAccessMode    = FSMC_BURST_ACCESS_MODE_DISABLE; /* ���ó������ģʽ�����Ǹ�λ���Ĭ��״̬�� (FSMC->BCR�Ĵ����ĵ�8λ) */
    g_sram_handle.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;  /* �ȴ��źż���λ��NWAIT �͵�ƽ��Ч (FSMC->BCR�Ĵ����ĵ�9λ) */
    g_sram_handle.Init.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;     /* �ȴ�ʱ�����ã�NWAIT �ź��ڵȴ�����֮ǰ��һ������������Ч (FSMC->BCR�Ĵ����ĵ�11λ) */
    g_sram_handle.Init.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;    /* д��ʹ��λ��FSMC �ڴ洢������ʹ����д����� (FSMC->BCR�Ĵ����ĵ�12λ) */
    g_sram_handle.Init.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;       /* �ȴ�ʹ��λ����ֹ NWAIT �ź� (FSMC->BCR�Ĵ����ĵ�13λ) */
    g_sram_handle.Init.ExtendedMode       = FSMC_EXTENDED_MODE_ENABLE;      /* ����д�����Ƿ������ͬʱ�򣺶�дʹ�ò���ͬ��ʱ�� (FSMC->BCR�Ĵ����ĵ�14λ) */
    g_sram_handle.Init.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE; /* �첽�����ڼ�ĵȴ��źţ������첽Э��ʱ������ NWAIT �ź� (FSMC->BCR�Ĵ����ĵ�15λ) */
    g_sram_handle.Init.WriteBurst         = FSMC_WRITE_BURST_DISABLE;       /* д��ͻ��ʹ�ܣ�ʼ�����첽ģʽ�½���д����� (FSMC->BCR�Ĵ����ĵ�19λ) */
    g_sram_handle.Init.MemoryType         = FSMC_MEMORY_TYPE_SRAM;          /* �洢�����ͣ�SRAM�� ROM���� (FSMC->BCR�Ĵ����ĵ�2-3λ) */
    g_sram_handle.Init.WrapMode           = FSMC_WRAP_MODE_DISABLE;         /* ������ֱ�ӵķǶ��������������Ǹ�λ���Ĭ��״̬�� (FSMC->BCR�Ĵ����ĵ�10λ) */

    /* ��ʼ��FSMC��,����ȴ�һ��ʱ����ܿ�ʼ��ʼ�� */  
    HAL_SRAM_Init(&g_sram_handle, &fsmc_read_handle, &fsmc_write_handle);   /* ��ʼ��FSMC���� */
}








