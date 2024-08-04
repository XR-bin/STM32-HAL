#include "fsmc.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\fsmc.c
*              .\Drivers\BSP\inc\fsmc.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-07-01
* @brief     ：FSMC配置代码
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



SRAM_HandleTypeDef g_sram_handle;    /* SRAM句柄(用于控制LCD) */

/**********************************************************
* @funcName ：FSMC_LCD_Init
* @brief    ：对LCD对应的GPIO口进行初始化设置,对特定的GPIO口进行FSMC复用
* @param    ：void
* @retval   ：void
* @details  ：
*     数据线：
*           DO0---PD14    DO1---PD15    DO2---PD0    DO3---PD1
*           DO4---PE7     DO5---PE8     DO6---PE9    DO7---PE10
*           DO8---PE11    DO9---PE12    DO10---PE13  DO11---PE14
*           DO12---PE15   DO13---PD8    DO14---PD9   DO15---PD10
*
*     控制线：
*           WR---PD5    RD---PD4    CS---PG12
*           RS/DC(数据/命名)---PG0
*           BL（背光）--PB0
*     整理：
*         推挽复用
*               PD：0-1   4-5   8-10   14-15  
*               PE：7-15
*               PG：0   12
*         推挽输出
*               PB：0
*
*     需要设置的是FSMC的Bank1的区域4
* @fn       ：
************************************************************/
void FSMC_LCD_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;                          /* GPIO配置结构体 */
    FSMC_NORSRAM_TimingTypeDef fsmc_read_handle;                /* 片选时序寄存器配置结构体(读时序控制寄存器) */
    FSMC_NORSRAM_TimingTypeDef fsmc_write_handle;               /* 写入时序寄存器配置结构体(写时序控制寄存器) */

    /* 时钟使能   GPIOB  GPIOD  GPIOE  GPIOG  FSMC */
    __HAL_RCC_GPIOB_CLK_ENABLE();           /* 使能GPIOB时钟 */
    __HAL_RCC_GPIOD_CLK_ENABLE();           /* 使能GPIOD时钟 */
    __HAL_RCC_GPIOE_CLK_ENABLE();           /* 使能GPIOE时钟 */
    __HAL_RCC_GPIOG_CLK_ENABLE();           /* 使能GPIOG时钟 */
    __HAL_RCC_FSMC_CLK_ENABLE();            /* 使能FSMC时钟 */

    /***************GPIOx寄存器配置***************/
    /* PB0 */
    /* 背光灯，配置通用输出 */
    /* 端口配置寄存器 */
    gpio_init_struct.Pin   = GPIO_PIN_0;                /* PB0 端口配置 */
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_PP;       /* 推挽输出 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;      /* IO口速度为50MHz */
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);            /* 根据设定参数初始化PB0 */
    /* PD0/1/4/5/8/9/10/14/15 */
    /* 配置复用输出 */
    /* 端口配置寄存器 */
    gpio_init_struct.Pin  = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;    /* PD0/1/4/5/8/9/10/14/15 端口配置 */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* 推挽输出 */
    HAL_GPIO_Init(GPIOD, &gpio_init_struct);            /* 根据设定参数初始化PD0/1/4/5/8/9/10/14/15 */
    /* PE7-15 */
    /* 配置复用输出 */
    /* 端口配置寄存器 */
    gpio_init_struct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;  /* PE7-15 端口配置 */
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);            /* 根据设定参数初始化PE7-15 */
    /* PG0/12 */
    /* 配置复用输出 */
    /* 端口配置寄存器 */
    gpio_init_struct.Pin = GPIO_PIN_0|GPIO_PIN_12;      /* PG0/12 端口配置 */
    HAL_GPIO_Init(GPIOG, &gpio_init_struct);            /* 根据设定参数初始化PG0/12 */

    /********** 关背光 **********/
    GPIOB->ODR &= ~(1<<0); 

    /***************FSMC寄存器配置***************/
    /***
    * 注意：bank1有NE1~4,每一个区域都有一个BCR+TCR+BWTR，所以总共12个寄存器。
    *
    *      FSMC_BCRx 和 FSMC_BTRx，组合成 BTCR[8]寄存器组，他们的对应关系如下：
    *      BTCR[0]对应 FSMC_BCR1，BTCR[1]对应 FSMC_BTR1
    *      BTCR[2]对应 FSMC_BCR2，BTCR[3]对应 FSMC_BTR2
    *      BTCR[4]对应 FSMC_BCR3，BTCR[5]对应 FSMC_BTR3
    *      BTCR[6]对应 FSMC_BCR4，BTCR[7]对应 FSMC_BTR4
    *
    *      FSMC_BWTRx 则组合成 BWTR[7]，他们的对应关系如下：
    *      BWTR[0]对应 FSMC_BWTR1，BWTR[2]对应 FSMC_BWTR2，
    *      BWTR[4]对应 FSMC_BWTR3，BWTR[6]对应 FSMC_BWTR4，
    *      BWTR[1]、BWTR[3]和 BWTR[5]保留，没有用到。
    */
    /* SRAM/NOR-Flash 片选时序寄存器 （读时序控制寄存器）*/
    fsmc_read_handle.AddressSetupTime      = 0;                      /* 地址建立时间(ADDSET)为1个HCLK 1/72M = 13.9ns (实际 > 200ns) (FSMC->BTR寄存器的第0-3位) */
    fsmc_read_handle.AddressHoldTime       = 0;                      /* 地址保持时间(ADDHLD) 模式A是没有用到 (FSMC->BTR寄存器的第4-7位) */
    /* 因为液晶驱动IC的读数据的时候，速度不能太快,尤其是个别奇葩芯片 */
    fsmc_read_handle.DataSetupTime         = 15;                     /* 数据保存时间(DATAST)为16个HCLK = 13.9 * 16 = 222.4ns (FSMC->BTR寄存器的第8-15位) */
    fsmc_read_handle.BusTurnAroundDuration = 0;                      /* 总线恢复时间=1个HCLK时钟周期 (FSMC->BTR寄存器的第16-19位) */
    fsmc_read_handle.CLKDivision           = 0;                      /* CLK 信号的时钟分频比：保留 (FSMC->BTR寄存器的第20-23位) */
    fsmc_read_handle.DataLatency           = 0;                      /* 同步突发 NOR Flash 的数据延迟：2个 CLK 时钟周期的数据延迟 (FSMC->BTR寄存器的第24-27位) */
    fsmc_read_handle.AccessMode            = FSMC_ACCESS_MODE_A;     /* 访问模式：访问模式 A (FSMC->BTR寄存器的第28-29位) */

    /* SRAM/NOR-Flash 写入时序寄存器   (写时序控制寄存器) */
    fsmc_write_handle.AddressSetupTime      = 0;                     /* 地址建立时间(ADDSET)为1个HCLK = 13.9ns (FSMC->BWTR寄存器的第0-3位) */
    fsmc_write_handle.AddressHoldTime       = 0;                     /* 地址保持时间(ADDHLD) 模式A是没有用到 (FSMC->BWTR寄存器的第4-7位) */
    fsmc_write_handle.DataSetupTime         = 1;                     /* 数据保存时间(DATAST)为2个HCLK = 13.9 * 2 = 27.8ns (实际 > 200ns) (FSMC->BWTR寄存器的第8-15位) */
    fsmc_write_handle.BusTurnAroundDuration = 0;                     /* 总线周转阶段的持续时间：0×HCLK 时钟周期 (FSMC->BWTR寄存器的第16-19位) */
    fsmc_write_handle.CLKDivision           = 0;                     /* CLK 信号的时钟分频比： 保留 (FSMC->BWTR寄存器的第20-23位) */
    fsmc_write_handle.DataLatency           = 0;                     /* 同步突发 NOR Flash 的数据延迟：2 个 CLK 时钟周期的数据延迟 (FSMC->BWTR寄存器的第24-27位) */
    /* 某些液晶驱动IC的写信号脉宽，最少也得50ns。 */
    fsmc_write_handle.AccessMode            = FSMC_ACCESS_MODE_A;    /* 访问模式：访问模式 A (FSMC->BWTR寄存器的第28-29位) */

    /* SRAM/NOR-Flash 片选控制寄存器 */
    /* 选择操作bank1 */
    g_sram_handle.Instance = FSMC_NORSRAM_DEVICE;
    g_sram_handle.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
    /* 配置bank相关寄存器 */
    g_sram_handle.Init.NSBank             = FSMC_NORSRAM_BANK4;             /* 选择块与区：bank1(块1)的NE4(区域4)也就对应BTCR[6],[7]。 */
    g_sram_handle.Init.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;  /* 地址/数据复用使能位：不使能 (FSMC->BCR寄存器的第1位) */
    g_sram_handle.Init.MemoryDataWidth    = FSMC_NORSRAM_MEM_BUS_WIDTH_16;  /* 存储器数据总线宽度：16bit  (FSMC->BCR寄存器的第4-5位) */
    g_sram_handle.Init.BurstAccessMode    = FSMC_BURST_ACCESS_MODE_DISABLE; /* 禁用成组访问模式；这是复位后的默认状态。 (FSMC->BCR寄存器的第8位) */
    g_sram_handle.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;  /* 等待信号极性位：NWAIT 低电平有效 (FSMC->BCR寄存器的第9位) */
    g_sram_handle.Init.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;     /* 等待时序配置：NWAIT 信号在等待周期之前的一个数据周期有效 (FSMC->BCR寄存器的第11位) */
    g_sram_handle.Init.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;    /* 写入使能位：FSMC 在存储区域内使能了写入操作 (FSMC->BCR寄存器的第12位) */
    g_sram_handle.Init.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;       /* 等待使能位：禁止 NWAIT 信号 (FSMC->BCR寄存器的第13位) */
    g_sram_handle.Init.ExtendedMode       = FSMC_EXTENDED_MODE_ENABLE;      /* 读和写操作是否采用相同时序：读写使用不相同的时序 (FSMC->BCR寄存器的第14位) */
    g_sram_handle.Init.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE; /* 异步传输期间的等待信号：运行异步协议时不考虑 NWAIT 信号 (FSMC->BCR寄存器的第15位) */
    g_sram_handle.Init.WriteBurst         = FSMC_WRITE_BURST_DISABLE;       /* 写入突发使能：始终在异步模式下进行写入操作 (FSMC->BCR寄存器的第19位) */
    g_sram_handle.Init.MemoryType         = FSMC_MEMORY_TYPE_SRAM;          /* 存储器类型：SRAM、 ROM类型 (FSMC->BCR寄存器的第2-3位) */
    g_sram_handle.Init.WrapMode           = FSMC_WRAP_MODE_DISABLE;         /* 不允许直接的非对齐成组操作；这是复位后的默认状态。 (FSMC->BCR寄存器的第10位) */

    /* 初始化FSMC后,必须等待一定时间才能开始初始化 */  
    HAL_SRAM_Init(&g_sram_handle, &fsmc_read_handle, &fsmc_write_handle);   /* 初始化FSMC配置 */
}








