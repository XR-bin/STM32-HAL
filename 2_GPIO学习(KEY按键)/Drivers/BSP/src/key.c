#include "./BSP/inc/key.h"

/**********************************************************
*@funcName ��Key_Init
*@brief    ��ʹ�����ʱ�ӣ�����ʼ��KEY�����IO
*@param    ��void(��)
*@retval   ��void(��)
*@details  ��
*             KEY0   --- PE4
*             KEY1   --- PE3
*             KEY_UP --- PA0
************************************************************/
void Key_Init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    KEY_GPIO_CLK_ENABLE();                                  /* KEY0��KEY1��KEY_UPʱ��ʹ�� */
    
    /* KEY0�������� */
    gpio_init_struct.Pin = KEY0_GPIO_PIN;                   /* KEY0���� */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                /* ��� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    HAL_GPIO_Init(KEY0_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��KEY0���� */
    /* KEY1�������� */
    gpio_init_struct.Pin = KEY1_GPIO_PIN;                   /* KEY1���� */
    HAL_GPIO_Init(KEY1_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��KEY1���� */
    /* KEY_UP�������� */
    gpio_init_struct.Pin = KEYUP_GPIO_PIN;                   /* KEY_UP���� */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                   /* ���� */
    HAL_GPIO_Init(KEYUP_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��KEY_UP���� */
}



/***************************************************************************
*@funcName ��Key_Scan
*@brief    ������ɨ�躯��
*@param    ��mode(�Ƿ�֧��������)
*   @arg  mode == 0,  ��֧��������(���������²���ʱ, ֻ�е�һ�ε��û᷵�ؼ�ֵ,
*                     �����ɿ��Ժ�, �ٴΰ��²Ż᷵��������ֵ)
*   @arg  mode == 1,  ֧��������(���������²���ʱ, ÿ�ε��øú������᷵�ؼ�ֵ)
*@retval   ����ֵ, ��������:
*            KEY0_PRES, 1, KEY0����
*            KEY1_PRES, 2, KEY1����
*            WKUP_PRES, 3, WKUP����
*@details  ��
*             KEY0   --- PE4
*             KEY1   --- PE3
*             KEY_UP --- PA0
****************************************************************************/
uint8_t Key_Scan(uint8_t mode)
{
    static uint8_t key_up = 1;  /* �������ɿ���־ */
    uint8_t keyval = 0;

    if (mode) key_up = 1;       /* ֧������ */

    if (key_up && (KEY0 == 0 || KEY1 == 0 || KEY_UP == 1))  /* �����ɿ���־Ϊ1, ��������һ������������ */
    {
        delay_ms(10);           /* ȥ���� */
        key_up = 0;

        if (KEY0 == 0)  keyval = KEY0_PRES;

        if (KEY1 == 0)  keyval = KEY1_PRES;

        if (KEY_UP == 1) keyval = WKUP_PRES;
    }
    else if (KEY0 == 1 && KEY1 == 1 && KEY_UP == 0) /* û���κΰ�������, ��ǰ����ɿ� */
    {
        key_up = 1;
    }

    return keyval;              /* ���ؼ�ֵ */
}

