#include "./SYSTEM/inc/sys.h"
#include "./SYSTEM/inc/delay.h"
#include "./SYSTEM/inc/usart.h"
#include "./BSP/inc/at24cxx.h"
#include "./BSP/inc/key.h"

int main(void)
{
    uint8_t p[7] = "123321";
    uint8_t pbuf[7] = {0};
    uint8_t key = 0;
    uint8_t temp = 1;
    
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* 设置时钟,72M */
    delay_init(72);                             /* 初始化延时函数 */
    Key_Init();                                 /* 初始化按键 */
    usart_init(115200);                         /* 初始化串口 115200 */
    AT24CXX_Init();                             /* 初始AT24CXX */
    
    while(1)
    {
        key = Key_Scan(0);
        
        switch(key)
        {
            case KEY0_PRES: 
                AT24CXX_Read(0x00, 7, pbuf);
                printf("读取到的数据：%s\r\n", pbuf);
                break;
            case KEY1_PRES: 
                if(temp) AT24CXX_Write(0x00, 7, p);
                else     printf("已经写入过\r\n");
                temp = 0;
                break;
        }
        
        delay_ms(10);
    }
}
