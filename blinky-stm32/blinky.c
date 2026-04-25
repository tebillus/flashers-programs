#include "stm32f411xx.h"

/* Simple busy-wait delay */
static void delay(volatile uint32_t count)
{
    while (count--);
}

int main(void)
{
    /* Enable GPIOC clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    /* Configure PC13 as output */
    GPIOC->MODER &= ~(3U << (13 * 2));
    GPIOC->MODER |=  (1U << (13 * 2));
    GPIOC->OTYPER  &= ~(1U << 13);
    GPIOC->OSPEEDR &= ~(3U << (13 * 2));
    GPIOC->PUPDR   &= ~(3U << (13 * 2));

    while (1) {
        GPIOC->BSRR = GPIO_BSRR_BS13;   /* LED OFF (PC13 high) */
        delay(9600000);                  /* ~0.5 s at 96 MHz */
        GPIOC->BSRR = GPIO_BSRR_BR13;   /* LED ON  (PC13 low)  */
        delay(9600000);                  /* ~0.5 s at 96 MHz */
    }
}
