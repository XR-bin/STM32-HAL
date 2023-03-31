#include "./SYSTEM/inc/sys.h"
#include "./SYSTEM/inc/delay.h"
#include "./SYSTEM/inc/usart.h"
#include "./BSP/inc/key.h"
#include "./BSP/inc/norflash.h"

int main(void)
{
    uint8_t key = 0, i;
    uint8_t buf[11] = {0};
    uint8_t p1[11] = "0123456789";
    uint8_t p2[11] = "9876543210";

    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* ����ʱ��,72M */
    delay_init(72);                             /* ��ʼ����ʱ���� */
    usart_init(115200);                         /* ����1��ʼ��115200 */
    
    Key_Init();
    norFlash_Init();                            /* W25QXXX��ʼ�� */

    while(1)
    {
        key = Key_Scan(0);
        
        switch(key)
        {
            case KEY0_PRES:
                    for(i=0; i<11; i++) buf[i] = 0;
                    norFlash_Read(buf, 0x00, 11);
                    printf("�����������ǣ�%s\r\n\r\n", buf);
                    break;
            case KEY1_PRES: 
                    norFlash_Write(p1, 0x00, 11);
                    printf("д��������ǣ�0123456789\r\n\r\n");
                    break;
            case WKUP_PRES:
                    norFlash_Write(p2, 0x00, 11);
                    printf("д��������ǣ�9876543210\r\n\r\n");
                    break;
            default: break;
        }
        
        delay_ms(10);
    }
}
