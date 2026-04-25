#include <stdint.h>

/* ============================================================================= */
/*  Raw Memory-Mapped Register Access — no headers, no structs                   */
/*  This version shows exactly which memory addresses are being touched.         */
/* ============================================================================= */

/* --- RCC (Reset & Clock Control) --- */
/* Base address: 0x4002 3800 */
#define RCC_BASE              0x40023800UL
#define RCC_AHB1ENR           (*(volatile uint32_t *)(RCC_BASE + 0x30UL))

/* Bit 2 = GPIOC clock enable */
#define RCC_AHB1ENR_GPIOCEN   (1UL << 2)

/* --- GPIO Port C --- */
/* Base address: 0x4002 0800 */
#define GPIOC_BASE            0x40020800UL
#define GPIOC_MODER           (*(volatile uint32_t *)(GPIOC_BASE + 0x00UL))
#define GPIOC_OTYPER          (*(volatile uint32_t *)(GPIOC_BASE + 0x04UL))
#define GPIOC_OSPEEDR         (*(volatile uint32_t *)(GPIOC_BASE + 0x08UL))
#define GPIOC_PUPDR           (*(volatile uint32_t *)(GPIOC_BASE + 0x0CUL))
#define GPIOC_ODR             (*(volatile uint32_t *)(GPIOC_BASE + 0x14UL))
#define GPIOC_BSRR            (*(volatile uint32_t *)(GPIOC_BASE + 0x18UL))

/* --- Helper masks for PC13 --- */
/* MODER: 2 bits per pin. Pin 13 -> bits 26-27 */
#define PC13_MODER_MASK       (0x3UL << 26)
#define PC13_MODER_OUTPUT     (0x1UL << 26)

/* BSRR: set bit (lower 16) = turn off LED (active low) */
#define PC13_BSRR_SET         (1UL << 13)
/* BSRR: reset bit (upper 16) = turn on LED */
#define PC13_BSRR_RESET       (1UL << 29)  /* 13 + 16 = 29 */

static void delay(volatile uint32_t count)
{
    while (count--);
}

int main(void)
{
    /* 1. Enable GPIOC peripheral clock */
    /* Before touching any GPIO register, we must feed it a clock. */
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    /* 2. Configure pin 13 as a general-purpose output */
    /* MODER13[1:0] = 01 (output mode) */
    GPIOC_MODER &= ~PC13_MODER_MASK;   /* clear existing mode */
    GPIOC_MODER |= PC13_MODER_OUTPUT;  /* set to output */

    /* Optional: configure output type, speed, pull-ups */
    GPIOC_OTYPER  &= ~(1UL << 13);     /* push-pull (0) */
    GPIOC_OSPEEDR &= ~(0x3UL << 26);   /* low speed (00) */
    GPIOC_PUPDR   &= ~(0x3UL << 26);   /* no pull (00) */

    while (1) {
        /* 3. Set PC13 high -> LED off (active-low on Black Pill) */
        GPIOC_BSRR = PC13_BSRR_SET;
        delay(9600000);                  /* ~0.5 s at 96 MHz */

        /* 4. Reset PC13 low -> LED on */
        GPIOC_BSRR = PC13_BSRR_RESET;
        delay(9600000);                  /* ~0.5 s at 96 MHz */
    }
}
