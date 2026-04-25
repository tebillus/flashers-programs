#ifndef __STM32F411xx_H
#define __STM32F411xx_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

/* ============================================================================= */
/*                     Memory Map                                                */
/* ============================================================================= */
#define FLASH_BASE            0x08000000UL
#define SRAM_BASE             0x20000000UL
#define PERIPH_BASE           0x40000000UL

/* AHB1 peripherals */
#define GPIOA_BASE            (PERIPH_BASE + 0x00020000UL)
#define GPIOB_BASE            (PERIPH_BASE + 0x00020400UL)
#define GPIOC_BASE            (PERIPH_BASE + 0x00020800UL)
#define GPIOD_BASE            (PERIPH_BASE + 0x00020C00UL)
#define GPIOE_BASE            (PERIPH_BASE + 0x00021000UL)
#define GPIOH_BASE            (PERIPH_BASE + 0x00021C00UL)
#define RCC_BASE              (PERIPH_BASE + 0x00023800UL)
#define FLASH_R_BASE          (PERIPH_BASE + 0x00023C00UL)
#define PWR_BASE              (PERIPH_BASE + 0x00007000UL)

/* ============================================================================= */
/*                     Peripheral Registers Structures                           */
/* ============================================================================= */

typedef struct
{
  volatile uint32_t MODER;
  volatile uint32_t OTYPER;
  volatile uint32_t OSPEEDR;
  volatile uint32_t PUPDR;
  volatile uint32_t IDR;
  volatile uint32_t ODR;
  volatile uint32_t BSRR;
  volatile uint32_t LCKR;
  volatile uint32_t AFR[2];
} GPIO_TypeDef;

typedef struct
{
  volatile uint32_t CR;
  volatile uint32_t PLLCFGR;
  volatile uint32_t CFGR;
  volatile uint32_t CIR;
  volatile uint32_t AHB1RSTR;
  volatile uint32_t AHB2RSTR;
  volatile uint32_t RESERVED0[2];
  volatile uint32_t APB1RSTR;
  volatile uint32_t APB2RSTR;
  volatile uint32_t RESERVED1[2];
  volatile uint32_t AHB1ENR;
  volatile uint32_t AHB2ENR;
  volatile uint32_t RESERVED2[2];
  volatile uint32_t APB1ENR;
  volatile uint32_t APB2ENR;
  volatile uint32_t RESERVED3[2];
  volatile uint32_t AHB1LPENR;
  volatile uint32_t AHB2LPENR;
  volatile uint32_t RESERVED4[2];
  volatile uint32_t APB1LPENR;
  volatile uint32_t APB2LPENR;
  volatile uint32_t RESERVED5[2];
  volatile uint32_t BDCR;
  volatile uint32_t CSR;
  volatile uint32_t RESERVED6[2];
  volatile uint32_t SSCGR;
  volatile uint32_t PLLI2SCFGR;
} RCC_TypeDef;

typedef struct
{
  volatile uint32_t ACR;
  volatile uint32_t KEYR;
  volatile uint32_t OPTKEYR;
  volatile uint32_t SR;
  volatile uint32_t CR;
  volatile uint32_t OPTCR;
} FLASH_TypeDef;

typedef struct
{
  volatile uint32_t CR;
  volatile uint32_t CSR;
} PWR_TypeDef;

/* ============================================================================= */
/*                     Peripheral Instances                                      */
/* ============================================================================= */
#define GPIOA                 ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB                 ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC                 ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD                 ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE                 ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOH                 ((GPIO_TypeDef *) GPIOH_BASE)
#define RCC                   ((RCC_TypeDef *) RCC_BASE)
#define FLASH                 ((FLASH_TypeDef *) FLASH_R_BASE)
#define PWR                   ((PWR_TypeDef *) PWR_BASE)

/* ============================================================================= */
/*                     RCC Bit Definitions                                       */
/* ============================================================================= */
#define RCC_AHB1ENR_GPIOAEN_Pos            (0U)
#define RCC_AHB1ENR_GPIOAEN_Msk            (0x1UL << RCC_AHB1ENR_GPIOAEN_Pos)
#define RCC_AHB1ENR_GPIOAEN                RCC_AHB1ENR_GPIOAEN_Msk

#define RCC_AHB1ENR_GPIOBEN_Pos            (1U)
#define RCC_AHB1ENR_GPIOBEN_Msk            (0x1UL << RCC_AHB1ENR_GPIOBEN_Pos)
#define RCC_AHB1ENR_GPIOBEN                RCC_AHB1ENR_GPIOBEN_Msk

#define RCC_AHB1ENR_GPIOCEN_Pos            (2U)
#define RCC_AHB1ENR_GPIOCEN_Msk            (0x1UL << RCC_AHB1ENR_GPIOCEN_Pos)
#define RCC_AHB1ENR_GPIOCEN                RCC_AHB1ENR_GPIOCEN_Msk

#define RCC_AHB1ENR_GPIODEN_Pos            (3U)
#define RCC_AHB1ENR_GPIODEN_Msk            (0x1UL << RCC_AHB1ENR_GPIODEN_Pos)
#define RCC_AHB1ENR_GPIODEN                RCC_AHB1ENR_GPIODEN_Msk

#define RCC_AHB1ENR_GPIOEEN_Pos            (4U)
#define RCC_AHB1ENR_GPIOEEN_Msk            (0x1UL << RCC_AHB1ENR_GPIOEEN_Pos)
#define RCC_AHB1ENR_GPIOEEN                RCC_AHB1ENR_GPIOEEN_Msk

#define RCC_AHB1ENR_GPIOHEN_Pos            (7U)
#define RCC_AHB1ENR_GPIOHEN_Msk            (0x1UL << RCC_AHB1ENR_GPIOHEN_Pos)
#define RCC_AHB1ENR_GPIOHEN                RCC_AHB1ENR_GPIOHEN_Msk

/* ============================================================================= */
/*                     GPIO Bit Definitions                                      */
/* ============================================================================= */
#define GPIO_MODER_MODE0_Pos               (0U)
#define GPIO_MODER_MODE0_Msk               (0x3UL << GPIO_MODER_MODE0_Pos)
#define GPIO_MODER_MODE0                   GPIO_MODER_MODE0_Msk
#define GPIO_MODER_MODE1_Pos               (2U)
#define GPIO_MODER_MODE1_Msk               (0x3UL << GPIO_MODER_MODE1_Pos)
#define GPIO_MODER_MODE1                   GPIO_MODER_MODE1_Msk
#define GPIO_MODER_MODE2_Pos               (4U)
#define GPIO_MODER_MODE2_Msk               (0x3UL << GPIO_MODER_MODE2_Pos)
#define GPIO_MODER_MODE2                   GPIO_MODER_MODE2_Msk
#define GPIO_MODER_MODE3_Pos               (6U)
#define GPIO_MODER_MODE3_Msk               (0x3UL << GPIO_MODER_MODE3_Pos)
#define GPIO_MODER_MODE3                   GPIO_MODER_MODE3_Msk
#define GPIO_MODER_MODE4_Pos               (8U)
#define GPIO_MODER_MODE4_Msk               (0x3UL << GPIO_MODER_MODE4_Pos)
#define GPIO_MODER_MODE4                   GPIO_MODER_MODE4_Msk
#define GPIO_MODER_MODE5_Pos               (10U)
#define GPIO_MODER_MODE5_Msk               (0x3UL << GPIO_MODER_MODE5_Pos)
#define GPIO_MODER_MODE5                   GPIO_MODER_MODE5_Msk
#define GPIO_MODER_MODE6_Pos               (12U)
#define GPIO_MODER_MODE6_Msk               (0x3UL << GPIO_MODER_MODE6_Pos)
#define GPIO_MODER_MODE6                   GPIO_MODER_MODE6_Msk
#define GPIO_MODER_MODE7_Pos               (14U)
#define GPIO_MODER_MODE7_Msk               (0x3UL << GPIO_MODER_MODE7_Pos)
#define GPIO_MODER_MODE7                   GPIO_MODER_MODE7_Msk
#define GPIO_MODER_MODE8_Pos               (16U)
#define GPIO_MODER_MODE8_Msk               (0x3UL << GPIO_MODER_MODE8_Pos)
#define GPIO_MODER_MODE8                   GPIO_MODER_MODE8_Msk
#define GPIO_MODER_MODE9_Pos               (18U)
#define GPIO_MODER_MODE9_Msk               (0x3UL << GPIO_MODER_MODE9_Pos)
#define GPIO_MODER_MODE9                   GPIO_MODER_MODE9_Msk
#define GPIO_MODER_MODE10_Pos              (20U)
#define GPIO_MODER_MODE10_Msk              (0x3UL << GPIO_MODER_MODE10_Pos)
#define GPIO_MODER_MODE10                  GPIO_MODER_MODE10_Msk
#define GPIO_MODER_MODE11_Pos              (22U)
#define GPIO_MODER_MODE11_Msk              (0x3UL << GPIO_MODER_MODE11_Pos)
#define GPIO_MODER_MODE11                  GPIO_MODER_MODE11_Msk
#define GPIO_MODER_MODE12_Pos              (24U)
#define GPIO_MODER_MODE12_Msk              (0x3UL << GPIO_MODER_MODE12_Pos)
#define GPIO_MODER_MODE12                  GPIO_MODER_MODE12_Msk
#define GPIO_MODER_MODE13_Pos              (26U)
#define GPIO_MODER_MODE13_Msk              (0x3UL << GPIO_MODER_MODE13_Pos)
#define GPIO_MODER_MODE13                  GPIO_MODER_MODE13_Msk
#define GPIO_MODER_MODE14_Pos              (28U)
#define GPIO_MODER_MODE14_Msk              (0x3UL << GPIO_MODER_MODE14_Pos)
#define GPIO_MODER_MODE14                  GPIO_MODER_MODE14_Msk
#define GPIO_MODER_MODE15_Pos              (30U)
#define GPIO_MODER_MODE15_Msk              (0x3UL << GPIO_MODER_MODE15_Pos)
#define GPIO_MODER_MODE15                  GPIO_MODER_MODE15_Msk

#define GPIO_MODER_INPUT                   0x00000000U
#define GPIO_MODER_OUTPUT                  0x00000001U
#define GPIO_MODER_AF                      0x00000002U
#define GPIO_MODER_ANALOG                  0x00000003U

#define GPIO_BSRR_BS0                      0x00000001U
#define GPIO_BSRR_BS1                      0x00000002U
#define GPIO_BSRR_BS2                      0x00000004U
#define GPIO_BSRR_BS3                      0x00000008U
#define GPIO_BSRR_BS4                      0x00000010U
#define GPIO_BSRR_BS5                      0x00000020U
#define GPIO_BSRR_BS6                      0x00000040U
#define GPIO_BSRR_BS7                      0x00000080U
#define GPIO_BSRR_BS8                      0x00000100U
#define GPIO_BSRR_BS9                      0x00000200U
#define GPIO_BSRR_BS10                     0x00000400U
#define GPIO_BSRR_BS11                     0x00000800U
#define GPIO_BSRR_BS12                     0x00001000U
#define GPIO_BSRR_BS13                     0x00002000U
#define GPIO_BSRR_BS14                     0x00004000U
#define GPIO_BSRR_BS15                     0x00008000U

#define GPIO_BSRR_BR0                      0x00010000U
#define GPIO_BSRR_BR1                      0x00020000U
#define GPIO_BSRR_BR2                      0x00040000U
#define GPIO_BSRR_BR3                      0x00080000U
#define GPIO_BSRR_BR4                      0x00100000U
#define GPIO_BSRR_BR5                      0x00200000U
#define GPIO_BSRR_BR6                      0x00400000U
#define GPIO_BSRR_BR7                      0x00800000U
#define GPIO_BSRR_BR8                      0x01000000U
#define GPIO_BSRR_BR9                      0x02000000U
#define GPIO_BSRR_BR10                     0x04000000U
#define GPIO_BSRR_BR11                     0x08000000U
#define GPIO_BSRR_BR12                     0x10000000U
#define GPIO_BSRR_BR13                     0x20000000U
#define GPIO_BSRR_BR14                     0x40000000U
#define GPIO_BSRR_BR15                     0x80000000U

#ifdef __cplusplus
}
#endif

#endif /* __STM32F411xx_H */
