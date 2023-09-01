#ifndef __DMA_H
#define __DMA_H

    /****************   外部头文件声明   ****************/
    #include "sys.h"
    #include "delay.h"
    #include "usart.h"



    /*****************   外部变量声明   *****************/
    extern DMA_HandleTypeDef  g_dma_handle;            /* DMA句柄 */



    /****************    函数外部声明   *****************/
    void DMA1_Usart1_Init(void);   /* 对DMA1的通道4进行初始化设置(连接USART1的发送通道) */

#endif




