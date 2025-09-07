/*
 * uart_reg.h
 *
 *  Created on: Aug 6, 2025
 *      Author: hp
 */

#ifndef DRIVER_UART_INTERNAL_UART_REG_H_
#define DRIVER_UART_INTERNAL_UART_REG_H_



#define   __I     volatile const       /*!< Defines 'read only' permissions */
#define     __O     volatile             /*!< Defines 'write only' permissions */
#define     __IO    volatile             /*!< Defines 'read / write' permissions */


typedef struct {
    __I uint32_t SR;        /*!< Offset: 0x00, Control Register */
    __IO  uint32_t DR;      /*!< Offset: 0x04, Status Register */
    __IO  uint32_t BRR;         /*!< Offset: 0x08, Receive Data Register */
    __IO  uint32_t CR1;         /*!< Offset: 0x0C, Transmit Data Register */
    __IO uint32_t CR2;         /*!< Offset: 0x10, Baud Rate Register */
    __IO uint32_t CR3;      /*!< Offset: 0x18, Interrupt Enable Register */
    __IO uint32_t GTPR;      /*!< Offset: 0x18, Interrupt Enable Register */
} uart_reg_map_t;


//Status Register (SR)
/** CTS: CTS flag */
/** @note: undefined on UART4 and UART5 */
#define USART_SR_CTS			(1 << 9)

/** LBD: LIN break detection flag */
#define USART_SR_LBD			(1 << 8)

/** TXE: Transmit data buffer empty */
#define USART_SR_TXE			(1 << 7)

/** TC: Transmission complete */
#define USART_SR_TC			(1 << 6)

/** RXNE: Read data register not empty */
#define USART_SR_RXNE			(1 << 5)

/** IDLE: Idle line detected */
#define USART_SR_IDLE			(1 << 4)

/** ORE: Overrun error */
#define USART_SR_ORE			(1 << 3)

/** NE: Noise error flag */
#define USART_SR_NE			(1 << 2)

/** FE: Framing error */
#define USART_SR_FE			(1 << 1)

/** PE: Parity error */
#define USART_SR_PE			(1 << 0)

/* --- USART_DR values ----------------------------------------------------- */

/* USART_DR[8:0]: DR[8:0]: Data value */
#define USART_DR_MASK                   0x1FF

/* --- USART_BRR values ---------------------------------------------------- */

/* DIV_Mantissa[11:0]: mantissa of USARTDIV */
#define USART_BRR_DIV_MANTISSA_MASK     (0xFFF << 4)
/* DIV_Fraction[3:0]: fraction of USARTDIV */
#define USART_BRR_DIV_FRACTION_MASK     0xF

/* --- USART_CR1 values ---------------------------------------------------- */

/* OVER8: Oversampling mode */
#define USART_CR1_OVER8			(1 << 15)
/* UE: USART enable */
#define USART_CR1_UE			(1 << 13)

/* M: Word length */
#define USART_CR1_M			(1 << 12)

/* WAKE: Wakeup method */
#define USART_CR1_WAKE			(1 << 11)

/* PCE: Parity control enable */
#define USART_CR1_PCE			(1 << 10)

/* PS: Parity selection */
#define USART_CR1_PS			(1 << 9)

/* PEIE: PE interrupt enable */
#define USART_CR1_PEIE			(1 << 8)

/* TXEIE: TXE interrupt enable */
#define USART_CR1_TXEIE			(1 << 7)

/* TCIE: Transmission complete interrupt enable */
#define USART_CR1_TCIE			(1 << 6)

/* RXNEIE: RXNE interrupt enable */
#define USART_CR1_RXNEIE		(1 << 5)

/* IDLEIE: IDLE interrupt enable */
#define USART_CR1_IDLEIE		(1 << 4)

/* TE: Transmitter enable */
#define USART_CR1_TE			(1 << 3)

/* RE: Receiver enable */
#define USART_CR1_RE			(1 << 2)

/* RWU: Receiver wakeup */
#define USART_CR1_RWU			(1 << 1)

/* SBK: Send break */
#define USART_CR1_SBK			(1 << 0)

/* --- USART_CR2 values ---------------------------------------------------- */

/* LINEN: LIN mode enable */
#define USART_CR2_LINEN			(1 << 14)

/* CLKEN: Clock enable */
#define USART_CR2_CLKEN			(1 << 11)

/* CPOL: Clock polarity */
#define USART_CR2_CPOL			(1 << 10)

/* CPHA: Clock phase */
#define USART_CR2_CPHA			(1 << 9)

/* LBCL: Last bit clock pulse */
#define USART_CR2_LBCL			(1 << 8)

/* LBDIE: LIN break detection interrupt enable */
#define USART_CR2_LBDIE			(1 << 6)

/* LBDL: LIN break detection length */
#define USART_CR2_LBDL			(1 << 5)

/* ADD[3:0]: Address of the usart node */
#define USART_CR2_ADD_MASK              0xF

/* --- USART_CR3 values ---------------------------------------------------- */

/* ONEBIT: One sample bit method enable */
#define USART_CR3_ONEBIT		(1 << 11)
/* CTSIE: CTS interrupt enable */
/* Note: N/A on UART4 & UART5 */
#define USART_CR3_CTSIE			(1 << 10)

/* CTSE: CTS enable */
/* Note: N/A on UART4 & UART5 */
#define USART_CR3_CTSE			(1 << 9)

/* RTSE: RTS enable */
/* Note: N/A on UART4 & UART5 */
#define USART_CR3_RTSE			(1 << 8)

/* DMAT: DMA enable transmitter */
/* Note: N/A on UART5 */
#define USART_CR3_DMAT			(1 << 7)

/* DMAR: DMA enable receiver */
/* Note: N/A on UART5 */
#define USART_CR3_DMAR			(1 << 6)

/* SCEN: Smartcard mode enable */
/* Note: N/A on UART4 & UART5 */
#define USART_CR3_SCEN			(1 << 5)

/* NACK: Smartcard NACK enable */
/* Note: N/A on UART4 & UART5 */
#define USART_CR3_NACK			(1 << 4)

/* HDSEL: Half-duplex selection */
#define USART_CR3_HDSEL			(1 << 3)

/* IRLP: IrDA low-power */
#define USART_CR3_IRLP			(1 << 2)

/* IREN: IrDA mode enable */
#define USART_CR3_IREN			(1 << 1)

/* EIE: Error interrupt enable */
#define USART_CR3_EIE			(1 << 0)

/* --- USART_GTPR values --------------------------------------------------- */

/* GT[7:0]: Guard time value */
/* Note: N/A on UART4 & UART5 */
#define USART_GTPR_GT_MASK              (0xFF << 8)

/* PSC[7:0]: Prescaler value */
/* Note: N/A on UART4/5 */
#define USART_GTPR_PSC_MASK             0xFF





#endif /* DRIVER_UART_INTERNAL_UART_REG_H_ */
