#include "dma.h"



/**
******************************************************************************
* @file      ��.\Drivers\BSP\src\dma.c
*              .\Drivers\BSP\inc\dma.h
* @author    ��XRbin
* @version   ��V1.0
* @date      ��2023-08-05
* @brief     ��DMA��ϰ����
******************************************************************************
* @attention
*   �ҵ�GitHub   ��https://github.com/XR-bin
*   �ҵ�gitee    ��https://gitee.com/xrbin
*   �ҵ�leetcode ��https://leetcode.cn/u/xrbin/
******************************************************************************
*/



DMA_HandleTypeDef  g_dma_handle;            /* DMA��� */

/**********************************************************
* @funcName ��DMA1_Usart1_Init
* @brief    ����DMA1��ͨ��4���г�ʼ������(����USART1�ķ���ͨ��)
* @param    ��uint8_t* data (DMA�洢����ַ)
* @retval   ��void
* @details  ��
* @fn       ��
*            DMA1���洢�������ݰ��˵�����1����
************************************************************/
void DMA1_Usart1_Init(void)
{
    /* ʱ��ʹ��  DMA1 */
    __HAL_RCC_DMA1_CLK_ENABLE();                        /* DMA1ʱ��ʹ�� */
    delay_ms(5);                                        /* �ȴ�DMAʱ���ȶ� */

    /* ����DMA */
    g_dma_handle.Instance                 = DMA1_Channel4;          /* USART1_TXʹ�õ�DMAͨ��Ϊ: DMA1_Channel4 */
    g_dma_handle.Init.Direction           = DMA_MEMORY_TO_PERIPH;   /* �洢��������ģʽ�����ݴӴ洢�������跽����(DMA_CCRx�Ĵ����ĵ�4λ) */
    g_dma_handle.Init.PeriphInc           = DMA_PINC_DISABLE;       /* ���������ģʽ�������ַָ��̶�(DMA_CCRx�Ĵ����ĵ�6λ) */
    g_dma_handle.Init.MemInc              = DMA_MINC_ENABLE;        /* �洢������ģʽ��ÿ�����ݴ���󣬴洢����ַָ�����(DMA_CCRx�Ĵ����ĵ�7λ) */
    g_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;    /* �������ݳ���8bit��ÿ�δ��������8�ֽ�����(DMA_CCRx�Ĵ����ĵ�8-9λ) */
    g_dma_handle.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;    /* �洢�����ݳ��ȣ�ÿ�δӴ洢������8�ֽ�����(DMA_CCRx�Ĵ����ĵ�10-11λ) */
    g_dma_handle.Init.Mode                = DMA_NORMAL;             /* ѭ��ģʽѡ�񣺽�ֹѭ��ģʽ(DMA_CCRx�Ĵ����ĵ�5λ) */
    g_dma_handle.Init.Priority            = DMA_PRIORITY_MEDIUM;    /* ���ȼ����е����ȼ�(DMA_CCRx�Ĵ����ĵ�12-13λ) */
    HAL_DMA_Init(&g_dma_handle);                                    /* ��ʼ��DMA */

    /* ���ڵ�DMA����/���չ���ʹ�� */
    __HAL_LINKDMA(&g_uart1_handle, hdmatx, g_dma_handle);           /* ��DMA��USART1��ϵ����(����DMA) */
}











