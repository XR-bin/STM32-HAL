#include "usart.h"



UART_HandleTypeDef g_uart1_handle;  /* UART1��� �������ô�����Ϣ��ȫ�ֱ��� */



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



