#include "./SYSTEM/inc/sys.h"
#include "./SYSTEM/inc/delay.h"
#include "./SYSTEM/inc/usart.h"
#include "./BSP/inc/key.h"
#include "./BSP/inc/can.h"

int main(void)
{
    uint8_t key=0;
    uint8_t rxlen = 0;          /* ���յ����ݳ��� */
    uint8_t mode = 1;           /* CAN����ģʽ: 0,����ģʽ; 1,�ػ�ģʽ */
    uint8_t buf[8] = {0};
    uint8_t buf1[] = "1234567"; /* һ֡�������8���ֽڣ��ַ������и�/0 */
    uint8_t buf2[] = "7654321";

    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* ����ʱ��,72M */
    delay_init(72);                             /* ��ʼ����ʱ���� */
    usart_init(115200);                         /* ���ڳ�ʼ�� 115200 */
    Key_Init();                                 /* ������ʼ�� */
    CAN_Init(CAN_SJW_1TQ, CAN_BS1_9TQ,CAN_BS2_8TQ,  4, CAN_MODE_LOOPBACK); /* CAN1��ʼ�� �ػ�ģʽ */
    
    while(1)
    {
        key = Key_Scan(0);
        
        if(key != 0)
        {
            switch(key)
            {
                case KEY0_PRES: 
                    printf("��������1234567\r\n");
                    CAN_Send_Message(0x12, buf1, 8);
                    break;
                case KEY1_PRES: 
                    printf("��������7654321\r\n");
                    CAN_Send_Message(0x12, buf2, 8);
                    break;
                case WKUP_PRES: 
                    mode = !mode;
                    if (mode == 0) /* ����ģʽ����Ҫ2�������� */
                    {
                        printf("�л�Ϊ����ģʽ\r\n");
                        CAN_Init(CAN_SJW_1TQ, CAN_BS1_9TQ,CAN_BS2_8TQ,  4, CAN_MODE_NORMAL); 
                    }
                    else
                    {
                        printf("�л�Ϊ�ػ�ģʽ\r\n");
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
                printf("���յ������ݳ���:%d\r\n", rxlen);
                printf("���յ������ݣ�%s\r\n", buf);
                printf("\r\n");
                
                rxlen = 0;
            }
            delay_ms(20);
        }
    }
}
