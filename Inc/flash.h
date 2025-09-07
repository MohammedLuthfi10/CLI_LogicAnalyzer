/*
 * flash.h
 *
 *  Created on: Mar 20, 2025
 *      Author: hp
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_


#pragma once
#include <stdint.h>
#include <stdbool.h>
#include"memorymap.h"


/**
 * This buffer is used for instruction fetches and may or may not be
 * enabled by default, depending on platform. (F1: yes, most others: no)
 *
 * Note carefully the clock restrictions under which the prefetch buffer may be
 * enabled or disabled. Changes are normally made while the clock is running in
 * the power-on low frequency mode before being set to a higher speed mode.
 *
 * Note carefully that prefetch may also results in increased consumption
 * and can only improve performance on "mostly linear" workloads where there
 * is at least one flash wait state.
 *
 * See the reference manual for your particular target for more details.
 */
void flash_prefetch_enable(void);

/**
 * Note carefully the clock restrictions under which the prefetch buffer may be
 * set to disabled. See the reference manual for details.
 */
void flash_prefetch_disable(void);


/** Set the Number of Wait States.

Used to match the system clock to the FLASH memory access time. See the
programming manual for more information on clock speed ranges. The latency must
be changed to the appropriate value <b>before</b> any increase in clock
speed, or <b>after</b> any decrease in clock speed.

@param[in] ws values from @ref flash_latency.
*/
void flash_set_ws(uint32_t ws);

/** Lock the Flash Program and Erase Controller
 * Used to prevent spurious writes to FLASH.
 */
void flash_lock(void);

/** Unlock the Flash Program and Erase Controller
 * This enables write access to the Flash memory. It is locked by default on
 * reset.
 */
void flash_unlock(void);

/** Unlock the Option Byte Access.
 * This enables write access to the option bytes. It is locked by default on
 * reset.
 */
void flash_unlock_option_bytes(void);




/**
 * Clear the End of OPeration flag.
 */
void flash_clear_eop_flag(void);

/**
 * Clear all status flags.
 * The number of bits can vary across families.
 */
void flash_clear_status_flags(void);

/** Wait until Last Operation has Ended.
 * This loops indefinitely until an operation (write or erase) has completed by
 * testing the busy flag
 */
void flash_wait_for_last_operation(void);


/**@{*/

/** @addtogroup flash_acr_values
 * @{
 */
#define FLASH_ACR_DCRST			(1 << 12)
#define FLASH_ACR_ICRST			(1 << 11)
#define FLASH_ACR_DCEN			(1 << 10)
#define FLASH_ACR_ICEN			(1 << 9)
/**@}*/




/** Enable the data cache */
void flash_dcache_enable(void);

/** Disable the data cache */
void flash_dcache_disable(void);

/** Enable the Instruction Cache */
void flash_icache_enable(void);

/** Disable the Instruction Cache */
void flash_icache_disable(void);

/** Reset the Data Cache.
 * The data cache must be disabled for this to have effect.
 */
void flash_dcache_reset(void);

/** Reset the Instruction Cache.
 * The instruction cache must be disabled for this to have effect.
 */
void flash_icache_reset(void);



/** @defgroup flash_registers Flash Registers
 * @ingroup flash_defines
@{*/
/** Flash Access Control register */
#define FLASH_ACR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x00)
/** Flash Key register */
#define FLASH_KEYR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x04)
/** Flash Option bytes key register */
#define FLASH_OPTKEYR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x08)
/** Flash Status register*/
#define FLASH_SR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x0C)
/** Flash Control register */
#define FLASH_CR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x10)
/** Flash Option Control register */
#define FLASH_OPTCR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x14)
/** Flash Option Control register 1 (bank 2) */
#define FLASH_OPTCR1			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x18)
/**@}*/

/** @defgroup flash_latency FLASH Wait States
@ingroup flash_defines
@{*/
#define FLASH_ACR_LATENCY(w)		((w) & FLASH_ACR_LATENCY_MASK)
#define FLASH_ACR_LATENCY_0WS          0x00
#define FLASH_ACR_LATENCY_1WS          0x01
#define FLASH_ACR_LATENCY_2WS          0x02
#define FLASH_ACR_LATENCY_3WS          0x03
#define FLASH_ACR_LATENCY_4WS          0x04
#define FLASH_ACR_LATENCY_5WS          0x05
#define FLASH_ACR_LATENCY_6WS          0x06
#define FLASH_ACR_LATENCY_7WS          0x07
/**@}*/
#define FLASH_ACR_LATENCY_SHIFT		0
#define FLASH_ACR_LATENCY_MASK		0x0f

/** @defgroup flash_acr_values FLASH_ACR values
 * @ingroup flash_registers
 * @brief Access Control register values
 * @{*/
#define FLASH_ACR_PRFTEN		(1 << 8)
/**@}*/

/* --- FLASH_SR values ----------------------------------------------------- */

#define FLASH_SR_BSY			(1 << 16)
#define FLASH_SR_PGPERR			(1 << 6)
#define FLASH_SR_PGAERR			(1 << 5)
#define FLASH_SR_WRPERR			(1 << 4)
#define FLASH_SR_OPERR			(1 << 1)
#define FLASH_SR_EOP			(1 << 0)

/* --- FLASH_CR values ----------------------------------------------------- */

#define FLASH_CR_LOCK			(1 << 31)
#define FLASH_CR_ERRIE			(1 << 25)
#define FLASH_CR_EOPIE			(1 << 24)
#define FLASH_CR_STRT			(1 << 16)
#define FLASH_CR_MER			(1 << 2)
#define FLASH_CR_SER			(1 << 1)
#define FLASH_CR_PG			(1 << 0)
#define FLASH_CR_SNB_SHIFT		3
#define FLASH_CR_SNB_MASK		0x1f
#define FLASH_CR_PROGRAM_MASK		0x3
#define FLASH_CR_PROGRAM_SHIFT		8
/** @defgroup flash_cr_program_width Flash programming width
@ingroup flash_group

@{*/
#define FLASH_CR_PROGRAM_X8		0
#define FLASH_CR_PROGRAM_X16		1
#define FLASH_CR_PROGRAM_X32		2
#define FLASH_CR_PROGRAM_X64		3
/**@}*/

/* --- FLASH_OPTCR values -------------------------------------------------- */

/* FLASH_OPTCR[27:16]: nWRP */
/* FLASH_OBR[15:8]: RDP */
#define FLASH_OPTCR_NRST_STDBY		(1 << 7)
#define FLASH_OPTCR_NRST_STOP		(1 << 6)
#define FLASH_OPTCR_OPTSTRT		(1 << 1)
#define FLASH_OPTCR_OPTLOCK		(1 << 0)
#define FLASH_OPTCR_BOR_LEVEL_3		(0x00 << 2)
#define FLASH_OPTCR_BOR_LEVEL_2		(0x01 << 2)
#define FLASH_OPTCR_BOR_LEVEL_1		(0x02 << 2)
#define FLASH_OPTCR_BOR_OFF		(0x03 << 2)

/* --- FLASH_OPTCR1 values ------------------------------------------------- */
/* Only on some devices */
/* FLASH_OPTCR1[27:16]: nWRP bank 2 */

/* --- FLASH Keys -----------------------------------------------------------*/

#define FLASH_KEYR_KEY1			((uint32_t)0x45670123)
#define FLASH_KEYR_KEY2			((uint32_t)0xcdef89ab)

#define FLASH_OPTKEYR_KEY1		((uint32_t)0x08192a3b)
#define FLASH_OPTKEYR_KEY2		((uint32_t)0x4c5d6e7f)

/* --- Function prototypes ------------------------------------------------- */



void flash_lock_option_bytes(void);
void flash_clear_pgperr_flag(void);
void flash_clear_wrperr_flag(void);
void flash_clear_pgaerr_flag(void);
void flash_erase_all_sectors(uint32_t program_size);
void flash_erase_sector(uint8_t sector, uint32_t program_size);
void flash_program_double_word(uint32_t address, uint64_t data);
void flash_program_word(uint32_t address, uint32_t data);
void flash_program_half_word(uint32_t address, uint16_t data);
void flash_program_byte(uint32_t address, uint8_t data);
void flash_program(uint32_t address, const uint8_t *data, uint32_t len);
void flash_program_option_bytes(uint32_t data);


/**@}*/




#endif /* INC_FLASH_H_ */
