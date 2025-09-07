/*
 * pwr.h
 *
 *  Created on: Mar 12, 2025
 *      Author: hp
 */

#ifndef INC_PWR_H_
#define INC_PWR_H_

#include <stdint.h>
#include <stdbool.h>
#pragma once
#include "memorymap.h"

/* --- PWR registers ------------------------------------------------------- */

/** Power control register (PWR_CR) */
#define PWR_CR				MMIO32(POWER_CONTROL_BASE + 0x00)

/** Power control/status register (PWR_CSR) */
#define PWR_CSR				MMIO32(POWER_CONTROL_BASE + 0x04)

/* --- PWR_CR values ------------------------------------------------------- */

/* Bits [31:9]: Reserved, must be kept at reset value. */

/** DBP: Disable backup domain write protection */
#define PWR_CR_DBP			(1 << 8)

/* PLS[7:5]: PVD level selection */
#define PWR_CR_PLS_LSB			5
/** @defgroup pwr_pls PVD level selection
@ingroup STM32F_pwr_defines

@{*/
#define PWR_CR_PLS_2V2			(0x0 << PWR_CR_PLS_LSB)
#define PWR_CR_PLS_2V3			(0x1 << PWR_CR_PLS_LSB)
#define PWR_CR_PLS_2V4			(0x2 << PWR_CR_PLS_LSB)
#define PWR_CR_PLS_2V5			(0x3 << PWR_CR_PLS_LSB)
#define PWR_CR_PLS_2V6			(0x4 << PWR_CR_PLS_LSB)
#define PWR_CR_PLS_2V7			(0x5 << PWR_CR_PLS_LSB)
#define PWR_CR_PLS_2V8			(0x6 << PWR_CR_PLS_LSB)
#define PWR_CR_PLS_2V9			(0x7 << PWR_CR_PLS_LSB)
/**@}*/
#define PWR_CR_PLS_MASK			(0x7 << PWR_CR_PLS_LSB)

/** PVDE: Power voltage detector enable */
#define PWR_CR_PVDE			(1 << 4)

/** CSBF: Clear standby flag */
#define PWR_CR_CSBF			(1 << 3)

/** CWUF: Clear wakeup flag */
#define PWR_CR_CWUF			(1 << 2)

/** PDDS: Power down deepsleep */
#define PWR_CR_PDDS			(1 << 1)

/** LPDS: Low-power deepsleep */
#define PWR_CR_LPDS			(1 << 0)

/* --- PWR_CSR values ------------------------------------------------------ */

/* Bits [31:9]: Reserved, must be kept at reset value. */

/** EWUP: Enable WKUP pin */
#define PWR_CSR_EWUP			(1 << 8)

/* Bits [7:3]: Reserved, must be kept at reset value. */

/** PVDO: PVD output */
#define PWR_CSR_PVDO			(1 << 2)

/** SBF: Standby flag */
#define PWR_CSR_SBF			(1 << 1)

/** WUF: Wakeup flag */
#define PWR_CSR_WUF			(1 << 0)

/* --- PWR function prototypes ------------------------------------------- */



void pwr_disable_backup_domain_write_protect(void);
void pwr_enable_backup_domain_write_protect(void);
void pwr_enable_power_voltage_detect(uint32_t pvd_level);
void pwr_disable_power_voltage_detect(void);
void pwr_clear_standby_flag(void);
void pwr_clear_wakeup_flag(void);
void pwr_set_standby_mode(void);
void pwr_set_stop_mode(void);
void pwr_voltage_regulator_on_in_stop(void);
void pwr_voltage_regulator_low_power_in_stop(void);
void pwr_enable_wakeup_pin(void);
void pwr_disable_wakeup_pin(void);
bool pwr_voltage_high(void);
bool pwr_get_standby_flag(void);
bool pwr_get_wakeup_flag(void);



/* --- PWR_CR values ------------------------------------------------------- */

/* Bits [31:15]: Reserved */

/* LPRUN: Low power run mode */
#define PWR_CR_LPRUN        (1 << 14)

/* VOS[12:11]: Regulator voltage scaling output selection */
#define PWR_CR_VOS_LSB			11
/** @defgroup pwr_vos Voltage Scaling Output level selection
@ingroup pwr_defines

@{*/
#define PWR_CR_VOS_RANGE1		(0x1 << PWR_CR_VOS_LSB)
#define PWR_CR_VOS_RANGE2		(0x2 << PWR_CR_VOS_LSB)
#define PWR_CR_VOS_RANGE3		(0x3 << PWR_CR_VOS_LSB)
/**@}*/
#define PWR_CR_VOS_MASK			(0x3 << PWR_CR_VOS_LSB)

/* FWU: Fast wakeup */
#define PWR_CR_FWU          (1 << 10)

/* ULP: Ultralow power mode */
#define PWR_CR_ULP          (1 << 9)

/* LPSDSR: Low-power deepsleep/sleep/low power run */
#define PWR_CR_LPSDSR		(1 << 0)  /* masks common PWR_CR_LPDS */

/* --- PWR_CSR values ------------------------------------------------------- */

/* EWUP2: Enable WKUP2 pin */
#define PWR_CSR_EWUP2			(1 << 9)

/* EWUP1: Enable WKUP1 pin */
#define PWR_CSR_EWUP1			PWR_CSR_EWUP

/* REGLPF : Regulator LP flag */
#define PWR_CSR_REGLPF          (1 << 5)

/* VOSF: Voltage Scaling select flag */
#define PWR_CSR_VOSF            (1 << 4)

/* VREFINTRDYF: Internal voltage reference (VREFINT) ready flag */
#define PWR_CSR_VREFINTRDYF	(1 << 3)



/* --- Function prototypes ------------------------------------------------- */

/** Voltage scales for internal regulator
 */
enum pwr_vos_scale {
	/** high performance, highest voltage */
	PWR_SCALE1,
	/** medium performance, flash operational but slow */
	PWR_SCALE2,
	/** low performance, no flash erase/program */
	PWR_SCALE3,
};



void pwr_set_vos_scale(enum pwr_vos_scale scale);



/*
 * This file extends the common STM32 version with definitions only
 * applicable to the STM32F4 series of devices.
 */
/** @addtogroup pwr_defines
 * @{*/

/* --- PWR_CR values ------------------------------------------------------- */

/** VOS: Regulator voltage scaling output selection */
#define PWR_CR_VOS_SHIFT			14
//#define PWR_CR_VOS_MASK			0x3

/** ADCDC1: Masks extra flash accesses by prefetch (see AN4073) */
#define PWR_CR_ADCDC1			(1 << 13)

/** MRLVDS/MRUDS: Main regulator Low Voltage / Under drive in Deep Sleep */
#define PWR_CR_MRLVDS			(1 << 11)
#define PWR_CR_MRUDS			PWR_CR_MRLVDS

/** LPLVDS/LPUDS: Low-power regulator Low Voltage / Under drive in Deep Sleep */
#define PWR_CR_LPLVDS			(1 << 10)
#define PWR_CR_LPUDS			PWR_CR_LPLVDS

/** FPDS: Flash power down in stop mode */
#define PWR_CR_FPDS			(1 << 9)

/* --- PWR_CSR values ------------------------------------------------------ */

/** VOSRDY: Regulator voltage scaling output selection ready bit */
#define PWR_CSR_VOSRDY			(1 << 14)

/** BRE: Backup regulator enable */
#define PWR_CSR_BRE			(1 << 9)

/** BRR: Backup regulator ready */
#define PWR_CSR_BRR			(1 << 3)

/* --- Function prototypes ------------------------------------------------- */




void pwr_set_vos_scale(enum pwr_vos_scale scale);




#endif /* INC_PWR_H_ */
