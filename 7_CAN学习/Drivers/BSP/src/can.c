#include "./BSP/inc/can.h"

CAN_HandleTypeDef   g_canx_handler;     /* CANx句柄 */
CAN_TxHeaderTypeDef g_canx_txheader;    /* 发送参数句柄 */
CAN_RxHeaderTypeDef g_canx_rxheader;    /* 接收参数句柄 */

/**********************************************************
*@funcName ：CAN_Init
*@brief    ：使能相关时钟，并初始化CAN以及相关IO
*
*@param    ：tsjw    ：重新同步跳跃时间单元.范围: 1~3;
*                      用于再同步调节，SJW值较大时，吸收误差能力更强，但是通讯速度会下降
*            tbs1    ：时间段1的时间单元.范围: 1~16;
*            tbs2    ：时间段2的时间单元.范围: 1~8;
*            brp     ：波特率分频器.范围: 1~1024;
*            mode    : CAN_MODE_NORMAL 正常模式;  CAN_MODE_LOOPBACK 回环模式;
*                      CAN_MODE_SILENT 静默模式;  CAN_MODE_SILENT_LOOPBACK 静默回环模式;
*            
*@retval   ：CAN初始化情况，0表示初始化成功，非0表示初始化出错
*@details  ：
*  位时序 : SS(同步段，占1个tp时间)、PTS(传播时间段,占1~8个tp时间)、
*           PBS1(相位缓冲段1,占1~8个tp时间)、PBS2(相位缓冲段2,占1~8个tp时间)
*  波特率计算公式演变过程：
*                   波特率 = 1 / 正常的位所用时间
*                   正常的位所用时间 = tp + tb1 + tb2 
*                   tb1 = tp * (tbs1+1)    tb2 = tp * (tbs2+1)
*                   tp = brp * tpclk1      tpclk1 = 1/Fpclk1 = 1/PCLK1 = 1/36Mhz(根据CAN所在时钟线)
*              说明：tb1表示PTS+PBS1所用时间、tb2表示PBS2所用时间、tp表示节拍时间
*  
*  波特率计算公式： 波特率 = Fpclk1 / ((tbs1 + tbs2 + 1) * brp)                      
*
************************************************************/
uint8_t CAN_Init(uint32_t tsjw, uint32_t tbs1, uint32_t tbs2, uint16_t brp, uint32_t mode)
{
    g_canx_handler.Instance = CAN1;
    g_canx_handler.Init.Prescaler = brp;                /* 分频系数(Fdiv)为brp+1 */
    g_canx_handler.Init.Mode = mode;                    /* 模式设置 */
    g_canx_handler.Init.SyncJumpWidth = tsjw;           /* 重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1TQ~CAN_SJW_4TQ */
    g_canx_handler.Init.TimeSeg1 = tbs1;                /* tbs1范围CAN_BS1_1TQ~CAN_BS1_16TQ */
    g_canx_handler.Init.TimeSeg2 = tbs2;                /* tbs2范围CAN_BS2_1TQ~CAN_BS2_8TQ */
    g_canx_handler.Init.TimeTriggeredMode = DISABLE;    /* 非时间触发通信模式 */
    g_canx_handler.Init.AutoBusOff = DISABLE;           /* 软件自动离线管理 */
    g_canx_handler.Init.AutoWakeUp = DISABLE;           /* 睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位) */
    g_canx_handler.Init.AutoRetransmission = ENABLE;    /* 禁止报文自动传送 */
    g_canx_handler.Init.ReceiveFifoLocked = DISABLE;    /* 报文不锁定,新的覆盖旧的 */
    g_canx_handler.Init.TransmitFifoPriority = DISABLE; /* 优先级由报文标识符决定 */
    
    //配置初始化CAN
    if (HAL_CAN_Init(&g_canx_handler) != HAL_OK)
    {
        return 1;
    }
    
    CAN_FilterTypeDef sFilterConfig;                      /* 过滤器配置结构体 */
    /*配置CAN过滤器*/
    sFilterConfig.FilterBank = 0;                         /* 过滤器0 */
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;     /* 标识符屏蔽位模式 */
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;    /* 长度32位位宽*/
    sFilterConfig.FilterIdHigh = 0x0000;                  /* 32位ID */
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;              /* 32位MASK(掩码，即屏蔽码) */
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;    /* 过滤器0关联到FIFO0 */
    sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;       /* 激活滤波器0 */
    sFilterConfig.SlaveStartFilterBank = 14;
    
    /* 过滤器配置 */
    if (HAL_CAN_ConfigFilter(&g_canx_handler, &sFilterConfig) != HAL_OK)
    {
        return 2;
    }

    /* 启动CAN外围设备 */
    if (HAL_CAN_Start(&g_canx_handler) != HAL_OK)
    {
        return 3;
    }
    
    return 0;
}

/**********************************************************
*@funcName ：HAL_CAN_MspInit
*@brief    ：CAN底层初始化函数(底层初始化回调钩子函数)
*@param    ：hcan---CAN句柄类型指针
*@retval   ：void(无)
*@details  ：
*       这个函数会在HAL_CAN_Init()函数中被调用，是CAN底层初
*    始化回调钩子函数。
*           PA11   CAN_RX   发送线  复用输入
*           PA12   CAN_TX   接收入  复用推挽
************************************************************/
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
    if (CAN1 == hcan->Instance)
    {
        CAN_RX_GPIO_CLK_ENABLE();       /* CAN_RX脚时钟使能 */
        CAN_TX_GPIO_CLK_ENABLE();       /* CAN_TX脚时钟使能 */
        __HAL_RCC_CAN1_CLK_ENABLE();    /* 使能CAN1时钟 */

        GPIO_InitTypeDef gpio_initure;

        gpio_initure.Pin = CAN_TX_GPIO_PIN;
        gpio_initure.Mode = GPIO_MODE_AF_PP;
        gpio_initure.Pull = GPIO_PULLUP;
        gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(CAN_TX_GPIO_PORT, &gpio_initure); /* CAN_TX脚 模式设置 */

        gpio_initure.Pin = CAN_RX_GPIO_PIN;
        gpio_initure.Mode = GPIO_MODE_AF_INPUT;
        HAL_GPIO_Init(CAN_RX_GPIO_PORT, &gpio_initure); /* CAN_RX脚 必须设置成输入模式 */
    }
}

/**********************************************************
*@funcName ：CAN_Send_Message
*@brief    ：CAN发送一组数据(标准 数据帧)
*@param    ：id：标准ID(11位)
*            msg：要发送的数据缓存区
*            len：数据长度(长度范围0~8个字节)
*@retval   ：发送情况，0表发送成功 ，1表发送失败
*@details  ：
************************************************************/
uint8_t CAN_Send_Message(uint32_t id, uint8_t *msg, uint8_t len)
{
    uint32_t TxMailbox = CAN_TX_MAILBOX0;

    g_canx_txheader.StdId = id;         /* 标准标识符 */
    g_canx_txheader.ExtId = id;         /* 扩展标识符(29位) 标准标识符情况下，该成员无效*/
    g_canx_txheader.IDE = CAN_ID_STD;   /* 使用标准标识符 */
    g_canx_txheader.RTR = CAN_RTR_DATA; /* 数据帧 */
    g_canx_txheader.DLC = len;

    /* 向空闲的邮箱放入信息，并激活邮箱发送 */
    if (HAL_CAN_AddTxMessage(&g_canx_handler, &g_canx_txheader, msg, &TxMailbox) != HAL_OK) 
    {
        return 1;
    }
  
    /* 等待发送完成,所有邮箱(有三个邮箱)为空 */
    /* 学习时才这样写，实际应用并不需要等待邮箱全空 */
    while (HAL_CAN_GetTxMailboxesFreeLevel(&g_canx_handler) != 3); 

    return 0;
}



/**********************************************************
*@funcName ：CAN_Receive_Message
*@brief    ：CAN接收一组数据(标准 数据帧)
*@param    ：id：标准ID(11位)
*            buf：接收的数据缓存区
*@retval   ：一组帧数据的数据长度，0表接收错误，非0表示数据长度
*@details  ：
************************************************************/
uint8_t CAN_Receive_Message(uint32_t id, uint8_t *buf)
{
    /* 判断邮箱是否为空 */
    if (HAL_CAN_GetRxFifoFillLevel(&g_canx_handler, CAN_RX_FIFO0) == 0)
    {
        return 0;
    }

    /* 读取邮箱一个数据 */
    if (HAL_CAN_GetRxMessage(&g_canx_handler, CAN_RX_FIFO0, &g_canx_rxheader, buf) != HAL_OK)  /* 读取数据 */
    {
        return 0;
    }
  
    /* 接收到的ID不对 / 不是标准帧 / 不是数据帧 */
    if (g_canx_rxheader.StdId!= id || g_canx_rxheader.IDE != CAN_ID_STD || g_canx_rxheader.RTR != CAN_RTR_DATA)       
    {
        return 0;    
    }

    //返回帧数据长度
    return g_canx_rxheader.DLC;
}


