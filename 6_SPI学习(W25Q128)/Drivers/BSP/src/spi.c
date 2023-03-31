#include "./BSP/inc/spi.h"

SPI_HandleTypeDef g_spi2_handler; /* SPI2句柄 用于配置SPI2信息的全局变量*/

/**********************************************************
*@funcName ：SPI2_Init
*@brief    ：对SPI2通信要用的GPIO口进行初始化和对SPI1的寄存器进行设置
*@param    ：void(无)
*@retval   ：void(无)
*@details  ：
************************************************************/
void SPI2_Init(void)
{
    SPI2_SPI_CLK_ENABLE(); /* SPI2时钟使能 */

    g_spi2_handler.Instance = SPI2_SPI;                                /* SPI2 */
    g_spi2_handler.Init.Mode = SPI_MODE_MASTER;                        /* 设置SPI工作模式，设置为主模式 */
    g_spi2_handler.Init.Direction = SPI_DIRECTION_2LINES;              /* 设置SPI单向或者双向的数据模式:SPI设置为双线模式 */
    g_spi2_handler.Init.DataSize = SPI_DATASIZE_8BIT;                  /* 设置SPI的数据大小:SPI发送接收8位帧结构 */
    g_spi2_handler.Init.CLKPolarity = SPI_POLARITY_HIGH;               /* 串行同步时钟的空闲状态为高电平 */
    g_spi2_handler.Init.CLKPhase = SPI_PHASE_2EDGE;                    /* 串行同步时钟的第二个跳变沿（上升或下降）数据被采样 */
    g_spi2_handler.Init.NSS = SPI_NSS_SOFT;                            /* NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制 */
    g_spi2_handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; /* 定义波特率预分频的值:波特率预分频值为256 */
    g_spi2_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;                   /* 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始 */
    g_spi2_handler.Init.TIMode = SPI_TIMODE_DISABLE;                   /* 关闭TI模式 */
    g_spi2_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;   /* 关闭硬件CRC校验 */
    g_spi2_handler.Init.CRCPolynomial = 7;                             /* CRC值计算的多项式 */
    HAL_SPI_Init(&g_spi2_handler);                                     /* 初始化 */

    __HAL_SPI_ENABLE(&g_spi2_handler); /* 使能SPI2 */
}

/**********************************************************
*@funcName ：HAL_SPI_MspInit
*@brief    ：SPI底层初始化函数(底层初始化回调钩子函数)
*@param    ：hspi---SPI句柄类型指针
*@retval   ：void(无)
*@details  ：
*       这个函数会在HAL_SPI_Init()函数中被调用，是SPI底层初
*    始化回调钩子函数。
*           PB13   SPI2_SCK   时钟线  复用推挽
*           PB14   SPI2_MISO  主输入  复用推挽
*           PB15   SPI2_MOSI  主输出  复用推挽
************************************************************/
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    if (hspi->Instance == SPI2_SPI)
    {
        SPI2_SCK_GPIO_CLK_ENABLE();  /* SPI2_SCK脚时钟使能 */
        SPI2_MISO_GPIO_CLK_ENABLE(); /* SPI2_MISO脚时钟使能 */
        SPI2_MOSI_GPIO_CLK_ENABLE(); /* SPI2_MOSI脚时钟使能 */

        /* SCK引脚模式设置(复用输出) */
        gpio_init_struct.Pin = SPI2_SCK_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SPI2_SCK_GPIO_PORT, &gpio_init_struct);

        /* MISO引脚模式设置(复用输出) */
        gpio_init_struct.Pin = SPI2_MISO_GPIO_PIN;
        HAL_GPIO_Init(SPI2_MISO_GPIO_PORT, &gpio_init_struct);

        /* MOSI引脚模式设置(复用输出) */
        gpio_init_struct.Pin = SPI2_MOSI_GPIO_PIN;
        HAL_GPIO_Init(SPI2_MOSI_GPIO_PORT, &gpio_init_struct);
    }
}



/**********************************************************
*@funcName ：SPI2_Set_Speed
*@brief    ：SPI2速度设置函数
*@param    ：speed---SPI2时钟分频系数
*@retval   ：void(无)
*@details  ：
*           SPI2时钟选择来自APB1, 即PCLK1, 为36Mhz
*              SPI速度 = PCLK1 / 2^(speed + 1)
************************************************************/
void SPI2_Set_Speed(uint8_t speed)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(speed)); /* 判断有效性 */
    __HAL_SPI_DISABLE(&g_spi2_handler);             /* 关闭SPI */
    g_spi2_handler.Instance->CR1 &= 0XFFC7;         /* 位3-5清零，用来设置波特率 */
    g_spi2_handler.Instance->CR1 |= speed << 3;     /* 设置SPI速度 */
    __HAL_SPI_ENABLE(&g_spi2_handler);              /* 使能SPI */
}


/**********************************************************
*@funcName ：SPI2_RW_Byte
*@brief    ：SPI2读写一个字节数据
*@param    ：txdata---要发送的数据(1字节)
*@retval   ：void(无)
*@details  ：
************************************************************/
uint8_t SPI2_RW_Byte(uint8_t txdata)
{
    uint8_t rxdata;
    HAL_SPI_TransmitReceive(&g_spi2_handler, &txdata, &rxdata, 1, 1000);   /* 数据大小为1个字节，延时1000ms */
    return rxdata; /* 返回收到的数据 */
}


