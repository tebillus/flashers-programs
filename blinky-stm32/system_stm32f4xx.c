#include <stdint.h>
#include "stm32f411xx.h"

#define SYSCLK_FREQ  96000000U  /* 96 MHz */

static void SetSysClock(void)
{
    /* Enable HSE */
    RCC->CR |= (1U << 16);
    while (!(RCC->CR & (1U << 17)));

    /* Configure flash latency for 96MHz (3 wait states for 2.7-3.6V) */
    FLASH->ACR = (FLASH->ACR & ~0xFU) | 0x3U;
    while ((FLASH->ACR & 0xFU) != 0x3U);

    /* Enable PWR clock and set voltage regulator scale 2 */
    RCC->APB1ENR |= (1U << 28);
    PWR->CR = (PWR->CR & ~(3U << 14)) | (1U << 14);

    /* Configure PLL: HSE(25MHz) / 25 * 192 = 192MHz VCO, /2 = 96MHz sysclk */
    /* PLLM=25, PLLN=192, PLLP=2 (00), PLLQ=4 */
    RCC->PLLCFGR = (25U << 0) | (192U << 6) | (0U << 16) | (4U << 24) | (1U << 22);

    /* Enable PLL */
    RCC->CR |= (1U << 24);
    while (!(RCC->CR & (1U << 25)));

    /* Select PLL as system clock */
    RCC->CFGR = (RCC->CFGR & ~0x3U) | 0x2U;
    while ((RCC->CFGR & 0xCU) != 0x8U);
}

void SystemInit(void)
{
    /* FPU enabled by startup, set CP10/CP11 full access if needed */
    /* Default after reset: HSI running, no prescalers */

    /* Set HSION just in case */
    RCC->CR |= 0x00000001U;

    /* Reset CFGR */
    RCC->CFGR = 0x00000000U;

    /* Reset PLLON, HSEON, CSSON */
    RCC->CR &= 0xFEF6FFFFU;

    /* Reset PLLCFGR */
    RCC->PLLCFGR = 0x24003010U;

    /* Disable all interrupts */
    RCC->CIR = 0x00000000U;

    SetSysClock();
}
