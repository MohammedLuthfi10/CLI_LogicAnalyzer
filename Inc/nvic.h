/*
 * nvic.h
 *
 *  Created on: Mar 20, 2025
 *      Author: hp
 */

#ifndef INC_NVIC_H_
#define INC_NVIC_H_
#pragma once
#include "common.h"
#define NVIC_IRQ_COUNT 0

#define SCB_SHPR(ipr_id)		MMIO8(SCS_BASE + 0xD18 + (ipr_id))

/** @defgroup nvic_registers NVIC Registers
 * @{
 */

/** ISER: Interrupt Set Enable Registers
 * @note 8 32bit Registers
 * @note Single register on CM0
 */
#define NVIC_ISER(iser_id)		MMIO32(NVIC_BASE + 0x00 + \
						((iser_id) * 4))

/* NVIC_BASE + 0x020 (0xE000 E120 - 0xE000 E17F): Reserved */

/** ICER: Interrupt Clear Enable Registers
 * @note 8 32bit Registers
 * @note Single register on CM0
 */
#define NVIC_ICER(icer_id)		MMIO32(NVIC_BASE + 0x80 + \
						((icer_id) * 4))

/* NVIC_BASE + 0x0A0 (0xE000 E1A0 - 0xE000 E1FF): Reserved */

/** ISPR: Interrupt Set Pending Registers
 * @note 8 32bit Registers
 * @note Single register on CM0
 */
#define NVIC_ISPR(ispr_id)		MMIO32(NVIC_BASE + 0x100 + \
						((ispr_id) * 4))

/* NVIC_BASE + 0x120 (0xE000 E220 - 0xE000 E27F): Reserved */

/** ICPR: Interrupt Clear Pending Registers
 * @note 8 32bit Registers
 * @note Single register on CM0
 */
#define NVIC_ICPR(icpr_id)		MMIO32(NVIC_BASE + 0x180 + \
						((icpr_id) * 4))

/* NVIC_BASE + 0x1A0 (0xE000 E2A0 - 0xE00 E2FF): Reserved */


/** IABR: Interrupt Active Bit Register
 * @note 8 32bit Registers */
#define NVIC_IABR(iabr_id)		MMIO32(NVIC_BASE + 0x200 + \
						((iabr_id) * 4))


/* NVIC_BASE + 0x220 (0xE000 E320 - 0xE000 E3FF): Reserved */

/** IPR: Interrupt Priority Registers
 * @note 240 8bit Registers
 * @note 32 8bit Registers on CM0, requires word access
 */

#define NVIC_IPR(ipr_id)		MMIO8(NVIC_BASE + 0x300 + \
						(ipr_id))



/** STIR: Software Trigger Interrupt Register */
#define NVIC_STIR			MMIO32(STIR_BASE)


/**@}*/

/* --- IRQ channel numbers-------------------------------------------------- */

/* Cortex M0, M3 and M4 System Interrupts */
/** @defgroup nvic_sysint Cortex M0/M3/M4 System Interrupts
@ingroup CM3_nvic_defines

IRQ numbers -3 and -6 to -9 are reserved
@{*/
#define NVIC_NMI_IRQ			-14
#define NVIC_HARD_FAULT_IRQ		-13


#define NVIC_MEM_MANAGE_IRQ		-12
#define NVIC_BUS_FAULT_IRQ		-11
#define NVIC_USAGE_FAULT_IRQ		-10


/* irq numbers -6 to -9 are reserved */
#define NVIC_SV_CALL_IRQ		-5


#define DEBUG_MONITOR_IRQ		-4


/* irq number -3 reserved */
#define NVIC_PENDSV_IRQ			-2
#define NVIC_SYSTICK_IRQ		-1
/**@}*/

/* @note User interrupts are family specific and are defined in a family
 * specific header file in the corresponding subfolder.
 */


/* --- NVIC functions ------------------------------------------------------ */



void nvic_enable_irq(uint8_t irqn);
void nvic_disable_irq(uint8_t irqn);
uint8_t nvic_get_pending_irq(uint8_t irqn);
void nvic_set_pending_irq(uint8_t irqn);
void nvic_clear_pending_irq(uint8_t irqn);
uint8_t nvic_get_irq_enabled(uint8_t irqn);
void nvic_set_priority(uint8_t irqn, uint8_t priority);


uint8_t nvic_get_active_irq(uint8_t irqn);
void nvic_generate_software_interrupt(uint16_t irqn);


void reset_handler(void);
void nmi_handler(void);
void hard_fault_handler(void);
void sv_call_handler(void);
void pend_sv_handler(void);
void sys_tick_handler(void);


void mem_manage_handler(void);
void bus_fault_handler(void);
void usage_fault_handler(void);
void debug_monitor_handler(void);


#endif /* INC_NVIC_H_ */

