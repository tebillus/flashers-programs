/* blinky_stepa.c
 * Step A: Verify C runtime handoff works.
 * Empty SystemInit, main turns PC13 LED ON solid.
 * No delays, no HSE, no PLL.
 */

#include <stdint.h>

/* --- Empty SystemInit --- */
void SystemInit(void)
{
    /* nothing */
}

/* --- Turn LED ON solid --- */
int main(void)
{
    /* Enable GPIOC clock */
    *((volatile uint32_t *)0x40023830) |= (1U << 2);

    /* PC13 output mode */
    *((volatile uint32_t *)0x40020800) &= ~(3U << 26);
    *((volatile uint32_t *)0x40020800) |=  (1U << 26);

    /* PC13 LOW -> LED ON (active-low) */
    *((volatile uint32_t *)0x40020818) = (1U << 29);

    while (1) {}
    return 0;
}
