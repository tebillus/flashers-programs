#include "stm32f411xx.h"

/* Multi-pin blinky: toggle PA1, PB12, and PC13 simultaneously.
 * One of these should blink the onboard LED.
 * Running on HSI 16 MHz — no HSE/PLL, so SystemInit cannot hang.
 */

static void delay(volatile uint32_t count)
{
    while (count--);
}

int main(void)
{
    /* Enable GPIOA, GPIOB, GPIOC clocks */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    /* --- PA1 as output --- */
    GPIOA->MODER &= ~(3U << (1 * 2));
    GPIOA->MODER |=  (1U << (1 * 2));
    GPIOA->OTYPER  &= ~(1U << 1);
    GPIOA->OSPEEDR &= ~(3U << (1 * 2));
    GPIOA->PUPDR   &= ~(3U << (1 * 2));

    /* --- PB12 as output --- */
    GPIOB->MODER &= ~(3U << (12 * 2));
    GPIOB->MODER |=  (1U << (12 * 2));
    GPIOB->OTYPER  &= ~(1U << 12);
    GPIOB->OSPEEDR &= ~(3U << (12 * 2));
    GPIOB->PUPDR   &= ~(3U << (12 * 2));

    /* --- PC13 as output --- */
    GPIOC->MODER &= ~(3U << (13 * 2));
    GPIOC->MODER |=  (1U << (13 * 2));
    GPIOC->OTYPER  &= ~(1U << 13);
    GPIOC->OSPEEDR &= ~(3U << (13 * 2));
    GPIOC->PUPDR   &= ~(3U << (13 * 2));

    while (1) {
        /* All on */
        GPIOA->BSRR = (1U << 1);
        GPIOB->BSRR = (1U << 12);
        GPIOC->BSRR = GPIO_BSRR_BS13;
        delay(500000);

        /* All off */
        GPIOA->BSRR = (1U << (1 + 16));
        GPIOB->BSRR = (1U << (12 + 16));
        GPIOC->BSRR = GPIO_BSRR_BR13;
        delay(500000);
    }
}
