
#include <usart.h>
#include <rcc.h>

/*---------------------------------------------------------------------------*/
/** @brief USART Set Baudrate.

The baud rate is computed from the APB high-speed prescaler clock (for
USART1/6) or the APB low-speed prescaler clock (for other USARTs). These values
must be correctly set before calling this function (refer to the
rcc_clock_setup-* functions in RCC).

Note: For LPUART, baudrates over 2**24 (~16.7 Mbaud) may overflow
the calculation and are therefore not supported by this function.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
@param[in] baud unsigned 32 bit. Baud rate specified in Hz.
*/

void usart_set_baudrate(uint32_t usart, uint32_t baud)
{
	uint32_t clock = rcc_get_usart_clk_freq(usart);

	/*
	 * Yes it is as simple as that. The reference manual is
	 * talking about fractional calculation but it seems to be only
	 * marketing babble to sound awesome. It is nothing else but a
	 * simple divider to generate the correct baudrate.
	 *
	 * Note: We round() the value rather than floor()ing it, for more
	 * accurate divisor selection.
	 */
#ifdef LPUART1
	if (usart == LPUART1) {
		USART_BRR(usart) = (clock / baud) * 256
			+ ((clock % baud) * 256 + baud / 2) / baud;
		return;
	}
#endif

	USART_BRR(usart) = (clock + baud / 2) / baud;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Set Word Length.

The word length is set to 8 or 9 bits. Note that the last bit will be a parity
bit if parity is enabled, in which case the data length will be 7 or 8 bits
respectively.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
@param[in] bits unsigned 32 bit. Word length in bits 8 or 9.
*/

void usart_set_databits(uint32_t usart, uint32_t bits)
{
	if (bits == 8) {
		USART_CR1(usart) &= ~USART_CR1_M; /* 8 data bits */
	} else {
		USART_CR1(usart) |= USART_CR1_M;  /* 9 data bits */
	}
}

/*---------------------------------------------------------------------------*/
/** @brief USART Get Word Length.

The word length is set to 8 or 9 bits. Note that the last bit will be a parity
bit if parity is enabled, in which case the data length will be 7 or 8 bits
respectively.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
@returns unsigned 32 bit Word length in bits 8 or 9.
*/

uint32_t usart_get_databits(uint32_t usart)
{
	const uint32_t reg32 = USART_CR1(usart) & USART_CR1_M;
	if (reg32)
		return 9;
	else
		return 8;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Set Stop Bit(s).

The stop bits are specified as 0.5, 1, 1.5 or 2.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
@param[in] stopbits unsigned 32 bit. Stop bits @ref usart_cr2_stopbits.
*/

void usart_set_stopbits(uint32_t usart, uint32_t stopbits)
{
	uint32_t reg32;

	reg32 = USART_CR2(usart);
	reg32 = (reg32 & ~USART_CR2_STOPBITS_MASK) | stopbits;
	USART_CR2(usart) = reg32;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Get Stop Bit(s).

The stop bits are specified as 0.5, 1, 1.5 or 2.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
@returns unsigned 32 bit Stop bits @ref usart_cr2_stopbits.
*/

uint32_t usart_get_stopbits(uint32_t usart)
{
	const uint32_t reg32 = USART_CR2(usart);
	return reg32 & USART_CR2_STOPBITS_MASK;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Set Parity.

The parity bit can be selected as none, even or odd.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
@param[in] parity unsigned 32 bit. Parity @ref usart_cr1_parity.
*/

void usart_set_parity(uint32_t usart, uint32_t parity)
{
	uint32_t reg32;

	reg32 = USART_CR1(usart);
	reg32 = (reg32 & ~USART_PARITY_MASK) | parity;
	USART_CR1(usart) = reg32;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Get Parity.

The stop bits are specified as 0.5, 1, 1.5 or 2.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
@returns unsigned 32 bit Parity @ref usart_cr2_stopbits.
*/

uint32_t usart_get_parity(uint32_t usart)
{
	const uint32_t reg32 = USART_CR1(usart);
	return reg32 & USART_PARITY_MASK;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Set Rx/Tx Mode.

The mode can be selected as Rx only, Tx only or Rx+Tx.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
@param[in] mode unsigned 32 bit. Mode @ref usart_cr1_mode.
*/

void usart_set_mode(uint32_t usart, uint32_t mode)
{
	uint32_t reg32;

	reg32 = USART_CR1(usart);
	reg32 = (reg32 & ~USART_MODE_MASK) | mode;
	USART_CR1(usart) = reg32;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Set Hardware Flow Control.

The flow control bit can be selected as none, RTS, CTS or RTS+CTS.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
@param[in] flowcontrol unsigned 32 bit. Flowcontrol @ref usart_cr3_flowcontrol.
*/

void usart_set_flow_control(uint32_t usart, uint32_t flowcontrol)
{
	uint32_t reg32;

	reg32 = USART_CR3(usart);
	reg32 = (reg32 & ~USART_FLOWCONTROL_MASK) | flowcontrol;
	USART_CR3(usart) = reg32;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Enable.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
*/

void usart_enable(uint32_t usart)
{
	USART_CR1(usart) |= USART_CR1_UE;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Disable.

At the end of the current frame, the USART is disabled to reduce power.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
*/

void usart_disable(uint32_t usart)
{
	USART_CR1(usart) &= ~USART_CR1_UE;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Send Data Word with Blocking

Blocks until the transmit data buffer becomes empty then writes the next data
word for transmission.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
@param[in] data unsigned 16 bit.
*/

void usart_send_blocking(uint32_t usart, uint16_t data)
{
	usart_wait_send_ready(usart);
	usart_send(usart, data);
}

/*---------------------------------------------------------------------------*/
/** @brief USART Read a Received Data Word with Blocking.

Wait until a data word has been received then return the word.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
@returns unsigned 16 bit data word.
*/

uint16_t usart_recv_blocking(uint32_t usart)
{
	usart_wait_recv_ready(usart);

	return usart_recv(usart);
}

/*---------------------------------------------------------------------------*/
/** @brief USART Receiver DMA Enable.

DMA is available on:
@li USART1 Rx DMA1 channel 5.
@li USART2 Rx DMA1 channel 6.
@li USART3 Rx DMA1 channel 3.
@li UART4 Rx DMA2 channel 3.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
*/

void usart_enable_rx_dma(uint32_t usart)
{
	USART_CR3(usart) |= USART_CR3_DMAR;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Receiver DMA Disable.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
*/

void usart_disable_rx_dma(uint32_t usart)
{
	USART_CR3(usart) &= ~USART_CR3_DMAR;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Transmitter DMA Enable.

DMA is available on:
@li USART1 Tx DMA1 channel 4.
@li USART2 Tx DMA1 channel 7.
@li USART3 Tx DMA1 channel 2.
@li UART4 Tx DMA2 channel 5.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
*/

void usart_enable_tx_dma(uint32_t usart)
{
	USART_CR3(usart) |= USART_CR3_DMAT;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Transmitter DMA Disable.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
*/

void usart_disable_tx_dma(uint32_t usart)
{
	USART_CR3(usart) &= ~USART_CR3_DMAT;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Receiver Interrupt Enable.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
*/

void usart_enable_rx_interrupt(uint32_t usart)
{
	USART_CR1(usart) |= USART_CR1_RXNEIE;
}


/*---------------------------------------------------------------------------*/
/** @brief USART Receiver Interrupt Disable.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
*/

void usart_disable_rx_interrupt(uint32_t usart)
{
	USART_CR1(usart) &= ~USART_CR1_RXNEIE;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Transmitter Interrupt Enable.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
*/

void usart_enable_tx_interrupt(uint32_t usart)
{
	USART_CR1(usart) |= USART_CR1_TXEIE;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Transmitter Interrupt Disable.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
*/

void usart_disable_tx_interrupt(uint32_t usart)
{
	USART_CR1(usart) &= ~USART_CR1_TXEIE;
}

/*---------------------------------------------------------------------------*/
/**
 * @brief USART Transmission Complete Interrupt Enable
 *
 * @param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
 */

void usart_enable_tx_complete_interrupt(uint32_t usart)
{
	USART_CR1(usart) |= USART_CR1_TCIE;
}

/*---------------------------------------------------------------------------*/
/**
 * @brief USART Transmission Complete Interrupt Disable
 *
 * @param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
 */

void usart_disable_tx_complete_interrupt(uint32_t usart)
{
	USART_CR1(usart) &= ~USART_CR1_TCIE;
}

/** @brief USART Idle Interrupt Enable.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
*/

void usart_enable_idle_interrupt(uint32_t usart)
{
	USART_CR1(usart) |= USART_CR1_IDLEIE;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Idle Interrupt Disable.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
*/

void usart_disable_idle_interrupt(uint32_t usart)
{
	USART_CR1(usart) &= ~USART_CR1_IDLEIE;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Error Interrupt Enable.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
*/

void usart_enable_error_interrupt(uint32_t usart)
{
	USART_CR3(usart) |= USART_CR3_EIE;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Error Interrupt Disable.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
*/

void usart_disable_error_interrupt(uint32_t usart)
{
	USART_CR3(usart) &= ~USART_CR3_EIE;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Send a Data Word.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
@param[in] data unsigned 16 bit.
*/

void usart_send(uint32_t usart, uint16_t data)
{
	/* Send data. */
	USART_DR(usart) = (data & USART_DR_MASK);
}

/*---------------------------------------------------------------------------*/
/** @brief USART Read a Received Data Word.

If parity is enabled the MSB (bit 7 or 8 depending on the word length) is the
parity bit.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
@returns unsigned 16 bit data word.
*/

uint16_t usart_recv(uint32_t usart)
{
	/* Receive data. */
	return USART_DR(usart) & USART_DR_MASK;
}

/*---------------------------------------------------------------------------*/
/** @brief USART Wait for Transmit Data Buffer Empty

Blocks until the transmit data buffer becomes empty and is ready to accept the
next data word.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
*/

void usart_wait_send_ready(uint32_t usart)
{
	/* Wait until the data has been transferred into the shift register. */
	while ((USART_SR(usart) & USART_SR_TXE) == 0);
}

/*---------------------------------------------------------------------------*/
/** @brief USART Wait for Received Data Available

Blocks until the receive data buffer holds a valid received data word.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
*/

void usart_wait_recv_ready(uint32_t usart)
{
	/* Wait until the data is ready to be received. */
	while ((USART_SR(usart) & USART_SR_RXNE) == 0);
}

/*---------------------------------------------------------------------------*/
/** @brief USART Read a Status Flag.

@param[in] usart unsigned 32 bit. USART block register address base @ref
usart_reg_base
@param[in] flag Unsigned int32. Status register flag  @ref usart_sr_flags.
@returns boolean: flag set.
*/

bool usart_get_flag(uint32_t usart, uint32_t flag)
{
	return ((USART_SR(usart) & flag) != 0);
}


