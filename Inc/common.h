/*
 * common.h
 *
 *  Created on: Mar 20, 2025
 *      Author: hp
 */

#ifndef INC_COMMON_H_
#define INC_COMMON_H_
#pragma once
#include <stdint.h>
#include <stdbool.h>

/* Generic memory-mapped I/O accessor functions */
#define MMIO8(addr)		(*(volatile uint8_t *)(addr))
#define MMIO16(addr)		(*(volatile uint16_t *)(addr))
#define MMIO32(addr)		(*(volatile uint32_t *)(addr))
#define MMIO64(addr)		(*(volatile uint64_t *)(addr))

/* Generic bit-band I/O accessor functions */
#define BBIO_SRAM(addr, bit) \
	MMIO32((((uint32_t)addr) & 0x0FFFFF) * 32 + 0x22000000 + (bit) * 4)

#define BBIO_PERIPH(addr, bit) \
	MMIO32((((uint32_t)addr) & 0x0FFFFF) * 32 + 0x42000000 + (bit) * 4)


/* Generic bit definition */
#define BIT0  (1<<0)
#define BIT1  (1<<1)
#define BIT2  (1<<2)
#define BIT3  (1<<3)
#define BIT4  (1<<4)
#define BIT5  (1<<5)
#define BIT6  (1<<6)
#define BIT7  (1<<7)
#define BIT8  (1<<8)
#define BIT9  (1<<9)
#define BIT10 (1<<10)
#define BIT11 (1<<11)
#define BIT12 (1<<12)
#define BIT13 (1<<13)
#define BIT14 (1<<14)
#define BIT15 (1<<15)
#define BIT16 (1<<16)
#define BIT17 (1<<17)
#define BIT18 (1<<18)
#define BIT19 (1<<19)
#define BIT20 (1<<20)
#define BIT21 (1<<21)
#define BIT22 (1<<22)
#define BIT23 (1<<23)
#define BIT24 (1<<24)
#define BIT25 (1<<25)
#define BIT26 (1<<26)
#define BIT27 (1<<27)
#define BIT28 (1<<28)
#define BIT29 (1<<29)
#define BIT30 (1<<30)
#define BIT31 (1<<31)

/* --- ARM Cortex-M0, M3 and M4 specific definitions ----------------------- */

/* Private peripheral bus - Internal */
#define PPBI_BASE                       (0xE0000000U)


/* ITM: Instrumentation Trace Macrocell */
#define ITM_BASE                        (PPBI_BASE + 0x0000)

/* DWT: Data Watchpoint and Trace unit */
#define DWT_BASE                        (PPBI_BASE + 0x1000)

/* FPB: Flash Patch and Breakpoint unit */
#define FPB_BASE                        (PPBI_BASE + 0x2000)


/* PPBI_BASE + 0x3000 (0xE000 3000 - 0xE000 DFFF): Reserved */

#define SCS_BASE                        (PPBI_BASE + 0xE000)

/* PPBI_BASE + 0xF000 (0xE000 F000 - 0xE003 FFFF): Reserved */

/* Those defined only on ARMv7 and above */

#define TPIU_BASE                       (PPBI_BASE + 0x40000)


/* --- SCS: System Control Space --- */


/* ITR: Interrupt Type Register */
#define ITR_BASE                        (SCS_BASE + 0x0000)


/* SYS_TICK: System Timer */
#define SYS_TICK_BASE                   (SCS_BASE + 0x0010)

/* NVIC: Nested Vector Interrupt Controller */
#define NVIC_BASE                       (SCS_BASE + 0x0100)

/* SCB: System Control Block */
#define SCB_BASE                        (SCS_BASE + 0x0D00)

/* MPU: Memory protection unit */
#define MPU_BASE                        (SCS_BASE + 0x0D90)


/* STE: Software Trigger Interrupt Register */
#define STIR_BASE                       (SCS_BASE + 0x0F00)
/* ID: ID space */
#define ID_BASE                         (SCS_BASE + 0x0FD0)


/**
 * @defgroup coresight_registers Coresight Registers
 * @{
 * CoreSight Lock Status Registers and Lock Access Registers are
 * documented for the DWT, ITM, FPB and TPIU peripherals
 */
#define CORESIGHT_LSR_OFFSET	0xfb4
#define CORESIGHT_LAR_OFFSET	0xfb0

/** CoreSight Lock Status Register lock status bit */
#define CORESIGHT_LSR_SLK		(1<<1)
/** CoreSight Lock Status Register lock availability bit */
#define CORESIGHT_LSR_SLI		(1<<0)
/** CoreSight Lock Access key, common for all */
#define CORESIGHT_LAR_KEY		0xC5ACCE55


#endif /* INC_COMMON_H_ */

