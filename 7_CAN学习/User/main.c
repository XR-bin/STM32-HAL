#include "./SYSTEM/inc/sys.h"
#include "./SYSTEM/inc/delay.h"
#include "./SYSTEM/inc/usart.h"
#include "./BSP/inc/key.h"
#include "./BSP/inc/can.h"

int main(void)
{
    uint8_t key=0;
    uint8_t rxlen = 0;          /* 接收的数据长度 */
    uint8_t mode = 1;           /* CAN工作模式: 0,正常模式; 1,回环模式 */
    uint8_t buf[8] = {0};
    uint8_t buf1[] = "1234567"; /* 一帧数据最多8个字节，字符串还有个/0 */
    uint8_t buf2[] = "7654321";

    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* 设置时钟,72M */
    delay_init(72);                             /* 初始化延时函数 */
    usart_init(115200);                         /* 串口初始化 115200 */
    Key_Init();                                 /* 按键初始化 */
    CAN_Init(CAN_SJW_1TQ, CAN_BS1_9TQ,CAN_BS2_8TQ,  4, CAN_MODE_LOOPBACK); /* CAN1初始化 回环模式 */
    
    while(1)
    {
        key = Key_Scan(0);
        
        if(key != 0)
        {
            switch(key)
            {
                case KEY0_PRES: 
                    printf("发送数据1234567\r\n");
                    CAN_Send_Message(0x12, buf1, 8);
                    break;
                case KEY1_PRES: 
                    printf("发送数据7654321\r\n");
                    CAN_Send_Message(0x12, buf2, 8);
                    break;
                case WKUP_PRES: 
                    mode = !mode;
                    if (mode == 0) /* 正常模式，需要2个开发板 */
                    {
                        printf("切换为正常模式\r\n");
                        CAN_Init(CAN_SJW_1TQ, CAN_BS1_9TQ,CAN_BS2_8TQ,  4, CAN_MODE_NORMAL); 
                    }
                    else
                    {
                        printf("切换为回环模式\r\n");
                        CAN_Init(CAN_SJW_1TQ, CAN_BS1_9TQ,CAN_BS2_8TQ,  4, CAN_MODE_LOOPBACK);
                    }
                    break;
                default: break;
            }
        }
        else
        {
            rxlen = CAN_Receive_Message(0x12, buf);
            if(rxlen)
            {
                printf("接收到的数据长度:%d\r\n", rxlen);
                printf("接收到的数据：%s\r\n", buf);
                printf("\r\n");
                
                rxlen = 0;
            }
            delay_ms(20);
        }
    }
}
