#include "usart.h"
#include "w25q128.h"
#include "led.h"



UART_HandleTypeDef g_uart1_handle;      /* UART1��� �������ô�����Ϣ��ȫ�ֱ��� */

uint8_t g_rx_buffer[RXBUFFERSIZE];      /* HAL��ʹ�õĴ��ڽ��ջ��� */



/**********************************************************
* @funcName ��USART1_Init
* @brief    ����USART1��Ӧ��GPIO�ڽ��г�ʼ������
* @param    ��uint32_t baud
* @retval   ��void
* @details  ��
*             PA9     TX     ---------���
*             PA10    RX     ---------����
* @fn       ��
************************************************************/
void USART_Init(uint32_t baud)
{
    /*UART ��ʼ������*/
    g_uart1_handle.Instance        = USART_UX;               /* USART_UX---ѡ�񴮿�1 */
    g_uart1_handle.Init.BaudRate   = baud;                   /* ������ */
    g_uart1_handle.Init.WordLength = UART_WORDLENGTH_8B;     /* �ֳ�Ϊ8λ���ݸ�ʽ */
    g_uart1_handle.Init.StopBits   = UART_STOPBITS_1;        /* һ��ֹͣλ */
    g_uart1_handle.Init.Parity     = UART_PARITY_NONE;       /* ����żУ��λ */
    g_uart1_handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;    /* ��Ӳ������ */
    g_uart1_handle.Init.Mode       = UART_MODE_TX_RX;        /* �շ�ģʽ */
    HAL_UART_Init(&g_uart1_handle);                          /* HAL_UART_Init()��ʹ��UART1 */

    /* �ж����ȼ����� */
    HAL_NVIC_EnableIRQ(USART_UX_IRQn);                       /* ʹ��USART1�ж�ͨ�� */
    HAL_NVIC_SetPriority(USART_UX_IRQn, 1, 1);               /* ��2��������ȼ�:��ռ���ȼ�1�������ȼ�1 */

    /* �ж�ʹ�� */
     __HAL_UART_ENABLE_IT(&g_uart1_handle,UART_IT_IDLE);     /* ʹ�ܿ����ж� */
     __HAL_UART_ENABLE_IT(&g_uart1_handle,UART_IT_RXNE);     /* ʹ�ܽ����ж� */

    /* �ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ��������������� */
    HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t *)g_rx_buffer, RXBUFFERSIZE);
}



/**********************************************************
* @funcName ��HAL_UART_MspInit
* @brief    ��UART�ײ��ʼ������(�ײ��ʼ���ص����Ӻ���)
* @param    ��UART_HandleTypeDef *huart (UART�������ָ��)
* @retval   ��void
* @details  ��
* @fn       ��
*        �����������HAL_UART_Init()�����б����ã���UART�ײ��
*    ʼ���ص����Ӻ�����
************************************************************/
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_struct;                          /* GPIO���ýṹ�� */

    if (huart->Instance == USART_UX)                            /* ����Ǵ���1�����д���1 MSP��ʼ�� */
    {
        USART_TX_GPIO_CLK_ENABLE();                             /* ʹ�ܴ���TX��ʱ�� */
        USART_RX_GPIO_CLK_ENABLE();                             /* ʹ�ܴ���RX��ʱ�� */
        USART_UX_CLK_ENABLE();                                  /* ʹ�ܴ���ʱ�� */

        gpio_init_struct.Pin   = USART_TX_GPIO_PIN;             /* ���ڷ������ź� */
        gpio_init_struct.Mode  = GPIO_MODE_AF_PP;               /* ����������� */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* IO�ٶ�����Ϊ���� */
        HAL_GPIO_Init(USART_TX_GPIO_PORT, &gpio_init_struct);   /* ����TX�� ���óɸ����������ģʽ */

        gpio_init_struct.Pin   = USART_RX_GPIO_PIN;             /* ����RX�� ģʽ���� */
        gpio_init_struct.Mode  = GPIO_MODE_AF_INPUT;            /* �������� */
        gpio_init_struct.Pull  = GPIO_PULLUP;                   /* ���� */
        HAL_GPIO_Init(USART_RX_GPIO_PORT, &gpio_init_struct);   /* ����RX�� �������óɸ�����������ģʽ */
    }
}



/**********************************************************
* @funcName ��USART1_Send_Byte
* @brief    ������1����һ���ֽڵ����ݣ�uint8_t��
* @param    ��uint8_t data
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void USART1_Send_Byte(uint8_t data)
{
    while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC) != SET);             /* �ȴ����ͽ��� */
    HAL_UART_Transmit(&g_uart1_handle,&data, 1, 0xffff);                         /* ���͵����� */
}



/**********************************************************
* @funcName ��USART1_Send_Str
* @brief    ������1����һ���ַ���
* @param    ��uint8_t *str
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void USART1_Send_Str(uint8_t *str)
{
    while(*str != '\0')
    {
        while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC) != SET);             /* �ȴ����ͽ��� */
        HAL_UART_Transmit(&g_uart1_handle,str, 1, 0xffff);                           /* ���͵����� */
        str++;
    }
}



/**********************************************************
* @funcName ��USART1_Receive_Byte
* @brief    ������1����һ���ֽڵ����ݣ�uint8_t��
* @param    ��void
* @retval   ��uint8_t
* @details  ��
* @fn       ��
************************************************************/
uint8_t USART1_Receive_Byte(void)
{
    uint8_t data;

    while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_RXNE) != SET);           /* �ȴ����ղ�Ϊ�� */
    HAL_UART_Receive(&g_uart1_handle, &data, 1, 0xffff);                         /* ��ȡ���յ������� */

    return data;
}



/**********************************************************
* @funcName ��USART1_Receive_Str
* @brief    ������1����һ���ַ���
* @param    ��uint8_t *str
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void USART1_Receive_Str(uint8_t *str)
{
    while(1)
    {
        while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_RXNE) != SET);           /* �ȴ����ղ�Ϊ�� */
        HAL_UART_Receive(&g_uart1_handle, str, 1, 0xffff);                           /* ��ȡ���յ������� */
        if(*str == '\r')
        {
            while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_RXNE) != SET);           /* �ȴ����ղ�Ϊ�� */
            HAL_UART_Receive(&g_uart1_handle, str, 1, 0xffff);                           /* ��ȡ���յ������� */
            if(*str == '\n') break;
        }
        str++;
    }
    *str = '\0';
}



/**********************************************************
* @funcName ��USART_UX_IRQHandler
* @brief    �������жϺ���
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void USART_UX_IRQHandler(void)
{
    uint8_t data;
    static uint32_t addr = 0x0b0000;

    /* �ж��жϴ�������ʽ */
    /* ��������ж� */
    if(__HAL_UART_GET_FLAG(&g_uart1_handle, UART_FLAG_RXNE) ==  SET)
    {
        /* �ж�Ҫִ�е����� */
        LED0_ON;
        /* �ֿ�����д��W25Q128 */
        data = USART1->DR;
        W25Q128_Page_Write(addr,1,&data);
        addr++;

        /* �ȶ�USART1->SR���ٸ�USART1->DR����������жϱ�־ */
        /* ����ֱ���޸�USART1->SR���Ӷ�����жϱ�־ */
        /* ���������ַ�ʽ���� */
        USART1->SR &= ~(1<<5);
    }

    if(__HAL_UART_GET_FLAG(&g_uart1_handle, UART_FLAG_IDLE) ==  SET)
    {
        LED0_OFF;

        /* �ȶ�USART1->SR���ٸ�USART1->DR����������жϱ�־ */
        /* ����ֱ���޸�USART1->SR���Ӷ�����жϱ�־ */
        /* ���������ַ�ʽ���� */
        USART1->SR &= ~(1<<4);
        USART1->DR;
    }
}






















/******************************************************************************************/
/* �������´���, ֧��printf����, ������Ҫѡ��use MicroLIB */

#if 1

#if (__ARMCC_VERSION >= 6010050)            /* ʹ��AC6������ʱ */
__asm(".global __use_no_semihosting\n\t");  /* ������ʹ�ð�����ģʽ */
__asm(".global __ARM_use_no_argv \n\t");    /* AC6����Ҫ����main����Ϊ�޲�����ʽ�����򲿷����̿��ܳ��ְ�����ģʽ */

#else
/* ʹ��AC5������ʱ, Ҫ�����ﶨ��__FILE �� ��ʹ�ð�����ģʽ */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* ��ʹ�ð�����ģʽ��������Ҫ�ض���_ttywrch\_sys_exit\_sys_command_string����,��ͬʱ����AC6��AC5ģʽ */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* ����_sys_exit()�Ա���ʹ�ð�����ģʽ */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}


/* FILE �� stdio.h���涨��. */
FILE __stdout;

/* MDK����Ҫ�ض���fputc����, printf�������ջ�ͨ������fputc����ַ��������� */
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0);     /* �ȴ���һ���ַ�������� */

    USART1->DR = (uint8_t)ch;             /* ��Ҫ���͵��ַ� ch д�뵽DR�Ĵ��� */
    return ch;
}
#endif
/******************************************************************************************/



