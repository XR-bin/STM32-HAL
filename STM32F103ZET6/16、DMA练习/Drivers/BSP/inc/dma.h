#ifndef __DMA_H
#define __DMA_H

    /****************   �ⲿͷ�ļ�����   ****************/
    #include "sys.h"
    #include "delay.h"
    #include "usart.h"



    /*****************   �ⲿ��������   *****************/
    extern DMA_HandleTypeDef  g_dma_handle;            /* DMA��� */



    /****************    �����ⲿ����   *****************/
    void DMA1_Usart1_Init(void);   /* ��DMA1��ͨ��4���г�ʼ������(����USART1�ķ���ͨ��) */

#endif




