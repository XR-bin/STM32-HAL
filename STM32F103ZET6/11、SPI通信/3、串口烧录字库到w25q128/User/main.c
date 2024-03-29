#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "w25q128.h"

/**************************************************************
*备注事项：
*         1、W25Q64的0x0b0000~0x1F1F58用于存储字库
*            开始：块：0b   第11块      扇区：0    第0个扇区
*                  页：0    第0页       字节：0    第0个字节
*            结束：块：1F   第31块      扇区：1    第1个扇区
*                  页：F    第15页      字节：58   第88个字节
***************************************************************/

/***
* 实验大致过程：上电 -> 对25Q128进行擦除操作 -> 通过PC的串口调试发生‘字库.bin’ -> STM32串口接到数据后触发中断把数据写入W25Q128
*/

int main(void)
{
    HAL_Init();                          /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* 设置时钟,72M */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);         /* 2号分组方案 2 位抢占优先级， 2 位响应优先级 */
    SysTick_Init();                      /* 滴答定时器初始 */
    USART_Init(115200);                  /* 串口初始化  115200  使用中断*/
    LED_Init();                          /* LED初始化 */
    W25Q128_Init();                      /* W25Q128初始化 */

    /* 擦除 */
    printf("开始擦除\r\n");
    W25Q128_Blocks_Erase(0x0b0000,21);
    printf("擦除完成\r\n");
    printf("请发生字库的bin文件，当LED0亮时，文件正在烧录中，当LED0熄灭时，烧录完成\r\n");

    while(1)
    {
    }
}
