#include "stmflash.h"



/**
******************************************************************************
* @file      ：.\Drivers\BSP\src\stmflash.c
*              .\Drivers\BSP\inc\stmflash.h
* @author    ：XRbin
* @version   ：V1.0
* @date      ：2023-12-12
* @brief     ：STM32内部Flash读写操作
******************************************************************************
* @attention
*   我的GitHub   ：https://github.com/XR-bin
*   我的gitee    ：https://gitee.com/xrbin
*   我的leetcode ：https://leetcode.cn/u/xrbin/
******************************************************************************
*/



/**********************************************************
* @funcName ：STMFLASH_Read_HalfWord
* @brief    ：读取指定地址的半字(16位数据)
* @param    ：uint32_t faddr(读地址,此地址必须为2的倍数!!)
* @retval   ：uint16_t --- (地址中的数据)
* @details  ：
* @fn       ：
************************************************************/
uint16_t STMFLASH_Read_HalfWord(uint32_t faddr)
{
    return *(volatile uint16_t*)faddr; 
}



/**********************************************************
* @funcName ：STMFLASH_Read
* @brief    ：从指定地址开始读出指定长度的数据
* @param    ：uint32_t rAddr (起始地址，此地址必须为2的倍数!!,否则写入出错!)
* @param    ：uint16_t *pBuf (存数据的缓存)
* @param    ：uint16_t length (要读的数据长度)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void STMFLASH_Read(uint32_t rAddr, uint16_t *pBuf, uint16_t length)
{
    uint16_t i;

    for(i=0; i<length; i++)
    {
        pBuf[i]=STMFLASH_Read_HalfWord(rAddr);/* 读取2个字节 */
        rAddr+=2;                            /* 偏移2个字节 */
    }
}



/**********************************************************
* @funcName ：STMFLASH_Write_NoCheck
* @brief    ：从指定地址开始写入指定长度的数据(不检查的写入)
* @param    ：uint32_t wAddr (起始地址，此地址必须为2的倍数!!,否则写入出错!)
* @param    ：uint16_t *pBuf (存数据的缓存)
* @param    ：uint16_t length (要写的数据长度)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void STMFLASH_Write_NoCheck(uint32_t wAddr, uint16_t *pBuf, uint16_t length)
{
    uint16_t i;

    for(i=0; i<length; i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, wAddr, pBuf[i]);  /* 写数据 */
        wAddr+=2;                               /* 地址增加2 */
    }  
}



uint16_t STMFLASH_BUF[STM32_SECTOR_SIZE/2];     /* 读取数据的缓存，最多是2K字节 */

/**********************************************************
* @funcName ：STMFLASH_Write
* @brief    ：从指定地址开始写入指定长度的数据(有检查的写入)
* @param    ：uint32_t wAddr (起始地址，此地址必须为2的倍数!!,否则写入出错!)
* @param    ：uint16_t *pBuf (存数据的缓存)
* @param    ：uint16_t length (要写的数据长度)
* @retval   ：void
* @details  ：
* @fn       ：
************************************************************/
void STMFLASH_Write(uint32_t wAddr, uint16_t *pBuf, uint16_t length)
{
    uint32_t secpos;        /* 扇区地址 */
    uint16_t secoff;        /* 扇区内偏移地址(16位字计算) */
    uint16_t secremain;     /* 扇区内剩余地址(16位字计算) */
    uint16_t i;
    uint32_t offaddr;       /* 去掉0X08000000后的地址 */
    FLASH_EraseInitTypeDef flash_eraseop;
    uint32_t erase_addr;    /* 擦除错误，这个值为发生错误的扇区地址 */

    if (wAddr < STM32_FLASH_BASE || (wAddr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))
    {
        return;      /* 非法地址 */
    }

    HAL_FLASH_Unlock();  /* 解锁 */

    offaddr = wAddr - STM32_FLASH_BASE;         /* 实际偏移地址 */
    secpos = offaddr / STM32_SECTOR_SIZE;       /* 扇区地址  0~127 for STM32F103RBT6 */
    secoff = (offaddr % STM32_SECTOR_SIZE) / 2; /* 在扇区内的偏移(2个字节为基本单位.) */
    secremain = STM32_SECTOR_SIZE / 2 - secoff; /* 扇区剩余空间大小 */

    if (length <= secremain)
    {
        secremain = length; /* 不大于该扇区范围 */
    }

    while(1)
    {
        /* 读出整个扇区的内容 */
        STMFLASH_Read(secpos*STM32_SECTOR_SIZE+STM32_FLASH_BASE, STMFLASH_BUF, STM32_SECTOR_SIZE/2);

        for(i = 0; i < secremain; i++)     /* 校验数据 */
        {
            if(STMFLASH_BUF[secoff + i] != 0XFFFF)
            {
                break;      /* 需要擦除 */
            }
        }

        if(i < secremain)  /* 需要擦除 */
        {
            /* 擦除这个扇区 */
            flash_eraseop.TypeErase = FLASH_TYPEERASE_PAGES;        /* 选择页擦除 */
            flash_eraseop.Banks = FLASH_BANK_1;
            flash_eraseop.NbPages = 1;
            flash_eraseop.PageAddress = secpos * STM32_SECTOR_SIZE + STM32_FLASH_BASE;  /* 要擦除的扇区 */
            HAL_FLASHEx_Erase( &flash_eraseop, &erase_addr);

            for(i=0;i<secremain;i++)    /* 复制 */
            {
                STMFLASH_BUF[i+secoff] = pBuf[i];
            }

            /* 写入整个扇区 */
            STMFLASH_Write_NoCheck(secpos*STM32_SECTOR_SIZE+STM32_FLASH_BASE, STMFLASH_BUF, STM32_SECTOR_SIZE/2);
        }
        else
        {
            /* 写已经擦除了的,直接写入扇区剩余区间 */
            STMFLASH_Write_NoCheck(wAddr, pBuf, secremain);
        }

        if(length == secremain)
        {
            break;//写入结束了
        }
        else
        {
            secpos++;               /* 扇区地址增1 */
            secoff = 0;             /* 偏移位置为0 */
            pBuf += secremain;      /* 指针偏移 */
            wAddr += secremain * 2; /* 写地址偏移(16位数据地址,需要*2) */
            length -= secremain;    /* 字节(16位)数递减 */

            if (length > (STM32_SECTOR_SIZE / 2))
            {
                secremain = STM32_SECTOR_SIZE / 2; /* 下一个扇区还是写不完 */
            }
            else
            {
                secremain = length;                /* 下一个扇区可以写完了 */
            }
        }
    }

    HAL_FLASH_Lock();       /* 上锁 */
}








