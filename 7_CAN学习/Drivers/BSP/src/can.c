#include "./BSP/inc/can.h"

CAN_HandleTypeDef   g_canx_handler;     /* CANx��� */
CAN_TxHeaderTypeDef g_canx_txheader;    /* ���Ͳ������ */
CAN_RxHeaderTypeDef g_canx_rxheader;    /* ���ղ������ */

/**********************************************************
*@funcName ��CAN_Init
*@brief    ��ʹ�����ʱ�ӣ�����ʼ��CAN�Լ����IO
*
*@param    ��tsjw    ������ͬ����Ծʱ�䵥Ԫ.��Χ: 1~3;
*                      ������ͬ�����ڣ�SJWֵ�ϴ�ʱ���������������ǿ������ͨѶ�ٶȻ��½�
*            tbs1    ��ʱ���1��ʱ�䵥Ԫ.��Χ: 1~16;
*            tbs2    ��ʱ���2��ʱ�䵥Ԫ.��Χ: 1~8;
*            brp     �������ʷ�Ƶ��.��Χ: 1~1024;
*            mode    : CAN_MODE_NORMAL ����ģʽ;  CAN_MODE_LOOPBACK �ػ�ģʽ;
*                      CAN_MODE_SILENT ��Ĭģʽ;  CAN_MODE_SILENT_LOOPBACK ��Ĭ�ػ�ģʽ;
*            
*@retval   ��CAN��ʼ�������0��ʾ��ʼ���ɹ�����0��ʾ��ʼ������
*@details  ��
*  λʱ�� : SS(ͬ���Σ�ռ1��tpʱ��)��PTS(����ʱ���,ռ1~8��tpʱ��)��
*           PBS1(��λ�����1,ռ1~8��tpʱ��)��PBS2(��λ�����2,ռ1~8��tpʱ��)
*  �����ʼ��㹫ʽ�ݱ���̣�
*                   ������ = 1 / ������λ����ʱ��
*                   ������λ����ʱ�� = tp + tb1 + tb2 
*                   tb1 = tp * (tbs1+1)    tb2 = tp * (tbs2+1)
*                   tp = brp * tpclk1      tpclk1 = 1/Fpclk1 = 1/PCLK1 = 1/36Mhz(����CAN����ʱ����)
*              ˵����tb1��ʾPTS+PBS1����ʱ�䡢tb2��ʾPBS2����ʱ�䡢tp��ʾ����ʱ��
*  
*  �����ʼ��㹫ʽ�� ������ = Fpclk1 / ((tbs1 + tbs2 + 1) * brp)                      
*
************************************************************/
uint8_t CAN_Init(uint32_t tsjw, uint32_t tbs1, uint32_t tbs2, uint16_t brp, uint32_t mode)
{
    g_canx_handler.Instance = CAN1;
    g_canx_handler.Init.Prescaler = brp;                /* ��Ƶϵ��(Fdiv)Ϊbrp+1 */
    g_canx_handler.Init.Mode = mode;                    /* ģʽ���� */
    g_canx_handler.Init.SyncJumpWidth = tsjw;           /* ����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1TQ~CAN_SJW_4TQ */
    g_canx_handler.Init.TimeSeg1 = tbs1;                /* tbs1��ΧCAN_BS1_1TQ~CAN_BS1_16TQ */
    g_canx_handler.Init.TimeSeg2 = tbs2;                /* tbs2��ΧCAN_BS2_1TQ~CAN_BS2_8TQ */
    g_canx_handler.Init.TimeTriggeredMode = DISABLE;    /* ��ʱ�䴥��ͨ��ģʽ */
    g_canx_handler.Init.AutoBusOff = DISABLE;           /* ����Զ����߹��� */
    g_canx_handler.Init.AutoWakeUp = DISABLE;           /* ˯��ģʽͨ���������(���CAN->MCR��SLEEPλ) */
    g_canx_handler.Init.AutoRetransmission = ENABLE;    /* ��ֹ�����Զ����� */
    g_canx_handler.Init.ReceiveFifoLocked = DISABLE;    /* ���Ĳ�����,�µĸ��Ǿɵ� */
    g_canx_handler.Init.TransmitFifoPriority = DISABLE; /* ���ȼ��ɱ��ı�ʶ������ */
    
    //���ó�ʼ��CAN
    if (HAL_CAN_Init(&g_canx_handler) != HAL_OK)
    {
        return 1;
    }
    
    CAN_FilterTypeDef sFilterConfig;                      /* ���������ýṹ�� */
    /*����CAN������*/
    sFilterConfig.FilterBank = 0;                         /* ������0 */
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;     /* ��ʶ������λģʽ */
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;    /* ����32λλ��*/
    sFilterConfig.FilterIdHigh = 0x0000;                  /* 32λID */
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;              /* 32λMASK(���룬��������) */
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;    /* ������0������FIFO0 */
    sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;       /* �����˲���0 */
    sFilterConfig.SlaveStartFilterBank = 14;
    
    /* ���������� */
    if (HAL_CAN_ConfigFilter(&g_canx_handler, &sFilterConfig) != HAL_OK)
    {
        return 2;
    }

    /* ����CAN��Χ�豸 */
    if (HAL_CAN_Start(&g_canx_handler) != HAL_OK)
    {
        return 3;
    }
    
    return 0;
}

/**********************************************************
*@funcName ��HAL_CAN_MspInit
*@brief    ��CAN�ײ��ʼ������(�ײ��ʼ���ص����Ӻ���)
*@param    ��hcan---CAN�������ָ��
*@retval   ��void(��)
*@details  ��
*       �����������HAL_CAN_Init()�����б����ã���CAN�ײ��
*    ʼ���ص����Ӻ�����
*           PA11   CAN_RX   ������  ��������
*           PA12   CAN_TX   ������  ��������
************************************************************/
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
    if (CAN1 == hcan->Instance)
    {
        CAN_RX_GPIO_CLK_ENABLE();       /* CAN_RX��ʱ��ʹ�� */
        CAN_TX_GPIO_CLK_ENABLE();       /* CAN_TX��ʱ��ʹ�� */
        __HAL_RCC_CAN1_CLK_ENABLE();    /* ʹ��CAN1ʱ�� */

        GPIO_InitTypeDef gpio_initure;

        gpio_initure.Pin = CAN_TX_GPIO_PIN;
        gpio_initure.Mode = GPIO_MODE_AF_PP;
        gpio_initure.Pull = GPIO_PULLUP;
        gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(CAN_TX_GPIO_PORT, &gpio_initure); /* CAN_TX�� ģʽ���� */

        gpio_initure.Pin = CAN_RX_GPIO_PIN;
        gpio_initure.Mode = GPIO_MODE_AF_INPUT;
        HAL_GPIO_Init(CAN_RX_GPIO_PORT, &gpio_initure); /* CAN_RX�� �������ó�����ģʽ */
    }
}

/**********************************************************
*@funcName ��CAN_Send_Message
*@brief    ��CAN����һ������(��׼ ����֡)
*@param    ��id����׼ID(11λ)
*            msg��Ҫ���͵����ݻ�����
*            len�����ݳ���(���ȷ�Χ0~8���ֽ�)
*@retval   �����������0���ͳɹ� ��1����ʧ��
*@details  ��
************************************************************/
uint8_t CAN_Send_Message(uint32_t id, uint8_t *msg, uint8_t len)
{
    uint32_t TxMailbox = CAN_TX_MAILBOX0;

    g_canx_txheader.StdId = id;         /* ��׼��ʶ�� */
    g_canx_txheader.ExtId = id;         /* ��չ��ʶ��(29λ) ��׼��ʶ������£��ó�Ա��Ч*/
    g_canx_txheader.IDE = CAN_ID_STD;   /* ʹ�ñ�׼��ʶ�� */
    g_canx_txheader.RTR = CAN_RTR_DATA; /* ����֡ */
    g_canx_txheader.DLC = len;

    /* ����е����������Ϣ�����������䷢�� */
    if (HAL_CAN_AddTxMessage(&g_canx_handler, &g_canx_txheader, msg, &TxMailbox) != HAL_OK) 
    {
        return 1;
    }
  
    /* �ȴ��������,��������(����������)Ϊ�� */
    /* ѧϰʱ������д��ʵ��Ӧ�ò�����Ҫ�ȴ�����ȫ�� */
    while (HAL_CAN_GetTxMailboxesFreeLevel(&g_canx_handler) != 3); 

    return 0;
}



/**********************************************************
*@funcName ��CAN_Receive_Message
*@brief    ��CAN����һ������(��׼ ����֡)
*@param    ��id����׼ID(11λ)
*            buf�����յ����ݻ�����
*@retval   ��һ��֡���ݵ����ݳ��ȣ�0����մ��󣬷�0��ʾ���ݳ���
*@details  ��
************************************************************/
uint8_t CAN_Receive_Message(uint32_t id, uint8_t *buf)
{
    /* �ж������Ƿ�Ϊ�� */
    if (HAL_CAN_GetRxFifoFillLevel(&g_canx_handler, CAN_RX_FIFO0) == 0)
    {
        return 0;
    }

    /* ��ȡ����һ������ */
    if (HAL_CAN_GetRxMessage(&g_canx_handler, CAN_RX_FIFO0, &g_canx_rxheader, buf) != HAL_OK)  /* ��ȡ���� */
    {
        return 0;
    }
  
    /* ���յ���ID���� / ���Ǳ�׼֡ / ��������֡ */
    if (g_canx_rxheader.StdId!= id || g_canx_rxheader.IDE != CAN_ID_STD || g_canx_rxheader.RTR != CAN_RTR_DATA)       
    {
        return 0;    
    }

    //����֡���ݳ���
    return g_canx_rxheader.DLC;
}


