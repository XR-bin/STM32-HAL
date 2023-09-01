    该文件夹下面存放驱动层代码，包括：
    1，BSP：自己编写的板级支持包驱动代码（原HARDWARE文件夹下的代码），如：LED、BEEP、KEY、EXTI、TIMER、WDG...等
    2，CMSIS：ARM提供的CMSIS代码（主要包括各种头文件和启动文件（.s文件），为了方便使用，减少占用空间，我们进行了精简）
    3，STM32F1xx_HAL_Driver：ST提供的F1xx HAL库驱动代码