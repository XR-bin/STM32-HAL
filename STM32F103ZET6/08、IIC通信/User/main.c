#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "iic.h"
#include "at24cxx.h"


int main(void)
{
//    uint8_t as1[] = "1234567";
//    uint8_t as2[] = "987654321";
//    uint8_t as3[] = "321654987";
//    uint8_t as4[] = "1237894";
//    uint8_t as5[] = "asdzxcqwe";
    uint8_t as6[] = "iopjklbnm";
    uint8_t bs[40] ;
    uint8_t err1,err2;

    HAL_Init();                          /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);  /* ����ʱ��,72M */
    SysTick_Init();                      /* �δ�ʱ����ʼ */
    USART_Init(115200);                  /* ����1��ʼ��  115200 */
    AT24CXX_Init();                      /* AT24CXX��ʼ�� */

    // ע�⣺AT24CXX_Write_Bytes1�ǲ��ܿ�ҳ�����ģ��������õ���AT24C02��һҳֻ��8���ֽ�
//    err1 = AT24CXX_Write_Bytes1(0x00, sizeof(as1), as1);
//    err2 = AT24CXX_Read_Bytes(0x00, sizeof(as1), bs);
//    printf("%s\r\n", bs);
//    printf("err1=%d    err2=%d\r\n", err1, err2);

    //�ɿ�ҳ����
//    err1 = AT24CXX_Write_Bytes(0x00, sizeof(as2), as2);
//    err2 = AT24CXX_Read_Bytes(0x00, sizeof(as2), bs);
//    printf("%s\r\n", bs);
//    printf("err1=%d    err2=%d\r\n", err1, err2);

    // �ɿ�ҳ����
//    err1 = AT24CXX_Write_Addr(0x00, sizeof(as3), as3);
//    err2 = AT24CXX_Read_Addr(0x00, sizeof(as3), bs);
//    printf("%s\r\n", bs);
//    printf("err1=%d    err2=%d\r\n", err1, err2);

    // ���ɿ�ҳ����
//    err1 = AT24CXXaddr_Write_Bytes1(AT24CXX, AT24CXX_WRITE, 0x00, sizeof(as4), as4);
//    err2 = AT24CXXaddr_Read_Bytes(AT24CXX, AT24CXX_WRITE, 0x00, sizeof(as4), bs);
//    printf("%s\r\n", bs);
//    printf("err1=%d    err2=%d\r\n", err1, err2);

    // �ɿ�ҳ����
//    err1 = AT24CXXaddr_Write_Bytes(AT24CXX, AT24CXX_WRITE, AT24CXX_PAGE, 0x00, sizeof(as5), as5);
//    err2 = AT24CXXaddr_Read_Bytes(AT24CXX, AT24CXX_WRITE, 0x00, sizeof(as5), bs);
//    printf("%s\r\n", bs);
//    printf("err1=%d    err2=%d\r\n", err1, err2);

    // �ɿ�ҳ����
    err1 = AT24CXXaddr_Write_Addr(AT24CXX, AT24CXX_WRITE, 0x00, sizeof(as6), as6);
    err2 = AT24CXXaddr_Read_Addr(AT24CXX, AT24CXX_WRITE, 0x00, sizeof(as6), bs);
    printf("%s\r\n", bs);
    printf("err1=%d    err2=%d\r\n", err1, err2);

    while(1)
    {
    }
}
