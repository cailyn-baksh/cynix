/*
 * Header for auxiliary devices on raspberry pi boards.
 * Targets BCM2711, BCM2835
 */
#ifndef _HW_AUXILIARY_H_
#define _HW_AUXILIARY_H_

/* Offsets */
#define AUX_IRQ						0x00215000
#define AUX_ENABLES					0x00215004

#define AUX_MU_IO_REG				0x00215040
#define AUX_MU_IER_REG				0x00215044
#define AUX_MU_IIR_REG				0x00215048
#define AUX_MU_LCR_REG				0x0021504C
#define AUX_MU_MCR_REG				0x00215050
#define AUX_MU_LSR_REG				0x00215054
#define AUX_MU_MSR_REG				0x00215058
#define AUX_MU_SCRATCH				0x0021505C
#define AUX_MU_CNTL_REG				0x00215060
#define AUX_MU_STAT_REG				0x00215064
#define AUX_MU_BAUD_REG				0x00215068

/* AUX_IRQ */
#define AUX_IRQ_MU						BIT(0)
#define AUX_IRQ_SPI1					BIT(1)
#define AUX_IRQ_SPI2					BIT(2)

/* AUX_ENABLES */
#define AUX_MU_ENABLED					BIT(0)
#define AUX_SPI1_ENABLED				BIT(1)
#define AUX_SPI2_ENABLED				BIT(2)

/* AUX_MU_IIR_REG */
#define AUX_MU_IIR_PENDING				BIT(0)

#define AUX_MU_IIR_NO_INT				BITS(0b00, 1)
#define AUX_MU_IIR_TRANSING				BITS(0b01, 1)
#define AUX_MU_IIR_VALID				BITS(0b10, 1)

#define AUX_MU_IIR_CLEAR_RX_FIFO		BIT(1)
#define AUX_MU_IIR_CLEAR_TX_FIFO		BIT(2)

// FIFO is always enabled
#define AUX_MU_IIR_FIFO_ENABLE			BITS(0b11, 6)

/* AUX_MU_LCR_REG */
#define AUX_MU_LCR_8_BIT				BIT(0)
#define AUX_MU_LCR_BREAK				BIT(6)
#define AUX_MU_LCR_DLAB					BIT(7)

/* AUX_MU_MCR_REG */
#define AUX_MU_MCR_RTS					BIT(1)

/* AUX_MU_CNTL */
#define AUX_MU_CNTL_RX_ENABLE			BIT(0)
#define AUX_MU_CNTL_TX_ENABLE			BIT(1)
#define AUX_MU_CNTL_RX_RTS_AUTO_ENABLE	BIT(2)
#define AUX_MU_CNTL_TX_CTS_AUTO_ENABLE	BIT(3)

#define AUX_MU_AF_RTS_AUTO_3_EMPTY		BITS(0b00, 4)
#define AUX_MU_AF_RTS_AUTO_2_EMPTY		BITS(0b01, 4)
#define AUX_MU_AF_RTS_AUTO_1_EMPTY		BITS(0b10, 4)
#define AUX_MU_AF_RTS_AUTO_4_EMPTY		BITS(0b11, 4)

#define AUX_MU_AF_RTS_ASSERT_LEVEL		BIT(6)
#define AUX_MU_AF_CTS_ASSERT_LEVEL		BIT(7)


#ifndef __ASSEMBLER__

/*
 * Initializes mini UART.
 */
void mini_uart_init();

/*
 * Write a character to the mini uart.
 *
 * c		The character to write.
 */
void uart1_putc(char c);

#endif  // !__ASSEMBLER__

#endif  // _HW_AUXILIARY_H_
