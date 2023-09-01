#include "dma.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\dma.c
*              .\Drivers\BSP\inc\dma.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-08-05
* @brief     ：DMA练习代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



DMA_HandleTypeDef  g_dma_handle;            /* DMA句柄 */

/**********************************************************
* @funcName ：DMA1_Usart1_Init
* @brief    ：对DMA1的通道4进行初始化设置(连接USART1的发送通道)
* @param    ：uint8_t* data (DMA存储器地址)
* @retval   ：void
* @details  ：
* @fn       ：
*            DMA1将存储器的数据搬运到串口1外设
************************************************************/
void DMA1_Usart1_Init(void)
{
    /* 时钟使能  DMA1 */
    __HAL_RCC_DMA1_CLK_ENABLE();                        /* DMA1时钟使能 */
    delay_ms(5);                                        /* 等待DMA时钟稳定 */

    /* 配置DMA */
    g_dma_handle.Instance                 = DMA1_Channel4;          /* USART1_TX使用的DMA通道为: DMA1_Channel4 */
    g_dma_handle.Init.Direction           = DMA_MEMORY_TO_PERIPH;   /* 存储器到外设模式：数据从存储器向外设方向传输(DMA_CCRx寄存器的第4位) */
    g_dma_handle.Init.PeriphInc           = DMA_PINC_DISABLE;       /* 外设非增量模式：外设地址指针固定(DMA_CCRx寄存器的第6位) */
    g_dma_handle.Init.MemInc              = DMA_MINC_ENABLE;        /* 存储器增量模式：每次数据传输后，存储器地址指针递增(DMA_CCRx寄存器的第7位) */
    g_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;    /* 外设数据长度8bit：每次从外设搬运8字节数据(DMA_CCRx寄存器的第8-9位) */
    g_dma_handle.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;    /* 存储器数据长度：每次从存储器运输8字节数据(DMA_CCRx寄存器的第10-11位) */
    g_dma_handle.Init.Mode                = DMA_NORMAL;             /* 循环模式选择：禁止循环模式(DMA_CCRx寄存器的第5位) */
    g_dma_handle.Init.Priority            = DMA_PRIORITY_MEDIUM;    /* 优先级：中等优先级(DMA_CCRx寄存器的第12-13位) */
    HAL_DMA_Init(&g_dma_handle);                                    /* 初始化DMA */

    /* 串口的DMA发送/接收功能使能 */
    __HAL_LINKDMA(&g_uart1_handle, hdmatx, g_dma_handle);           /* 将DMA与USART1联系起来(发送DMA) */
}











