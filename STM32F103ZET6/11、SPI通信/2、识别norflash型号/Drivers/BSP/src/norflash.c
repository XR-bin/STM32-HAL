#include "norflash.h"
#include "spi.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\norflash.c
*              .\Drivers\BSP\inc\norflash.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-07-01
* @brief     ��ʶ��norflashоƬID����
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ��NorFlash_Init
* @brief    ��NorFlashоƬ������ų�ʼ��
* @param    ��void
* @retval   ��void
* @details  ��
*             SPI_CS     PB12
* @fn       ��
************************************************************/
void NorFlash_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;               /* GPIO���ýṹ�� */

    __HAL_RCC_GPIOB_CLK_ENABLE();                    /* ʱ��ʹ��   GPIOB */

    /* GPIOx��ʼ������ */
    /* GPIOx�˿����� */
    /* PB12 */
    gpio_init_struct.Pin   = GPIO_PIN_12;            /* PB12 �˿����� */
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_PP;    /* ������� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;   /* IO���ٶ�Ϊ50MHz */
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);         /* �����趨������ʼ��PB12 */

    /* �˿�������ݼĴ���(Ƭѡ����) */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);  /* �˿�����Ĵ��������(��������) */

    SPI2_Init();                                          /* SPI��ʼ�� */
}



/**********************************************************
* @funcName ��NorFlash_Read_ID
* @brief    ����ȡnorflashоƬ��ID
* @param    ��void
* @retval   ��uint16_t -- ����оƬID
* @details  ��
* @fn       ��
************************************************************/
uint16_t NorFlash_Read_ID(void)
{
    uint16_t id;

    SPI_CS_L;                               /* Ƭѡʹ�� */
    SPI2_RS_Byte(FLASH_ManufactDeviceID);   /* ���Ͷ� ID ���� */
    SPI2_RS_Byte(0);                        /* д��һ���ֽ� */
    SPI2_RS_Byte(0);
    SPI2_RS_Byte(0);
    id =  SPI2_RS_Byte(0xFF) << 8;          /* ��ȡ��8λ�ֽ� */
    id |= SPI2_RS_Byte(0xFF);               /* ��ȡ��8λ�ֽ� */
    SPI_CS_H;                               /* Ƭѡʧ�� */

    /* оƬID�б�ο�norfalsh.h��ĺ궨�� */
    printf("оƬID : %x\r\n", id);

    return id;
}































