/*
 * Header for PL011 UART device on BCM2711, BCM2835
 */
#ifndef _PL011_H_
#define _PL011_H_

#define UART0					0x00201000
#define UART2					0x00201400
#define UART3					0x00201600
#define UART4					0x00201800
#define UART5					0x00201A00

#define PL011_DR				0x00
#define PL011_RSRECR			0x04
#define PL011_FR				0x18
#define PL011_ILPR				0x20
#define PL011_IBRD				0x24
#define PL011_FBRD				0x28
#define PL011_LCRH				0x2C
#define PL011_CR				0x30
#define PL011_IFLS				0x34
#define PL011_IMSC				0x38
#define PL011_RIS				0x3C
#define PL011_MIS				0x40
#define PL011_ICR				0x44
#define PL011_DMACR				0x48
#define PL011_ITCR				0x80
#define PL011_ITIP				0x84
#define PL011_ITOP				0x88
#define PL011_TDR				0x8C

#define PL011_REG(UART, REG)	MMIO(UART + REG)


#define PL011_DR_DATA_MASK			0xFF
#define PL011_DR_FE					BIT(8)
#define PL011_DR_PE					BIT(9)
#define PL011_DR_BE					BIT(10)
#define PL011_DR_OE					BIT(11)

#define PL011_RSRECR_FE				BIT(0)
#define PL011_RSRECR_PE				BIT(1)
#define PL011_RSRECR_BE				BIT(2)
#define PL011_RSRECR_OE				BIT(3)

#define PL011_FR_CTS				BIT(0)
#define PL011_FR_BUSY				BIT(3)
#define PL011_FR_RXFE				BIT(4)
#define PL011_FR_TXFF				BIT(5)
#define PL011_FR_RXFF				BIT(6)
#define PL011_FR_TXFE				BIT(7)

#define PL011_LCRH_BRK				BIT(0)
#define PL011_LCRH_PEN				BIT(1)
#define PL011_LCRH_EPS				BIT(2)
#define PL011_LCRH_STP2				BIT(3)
#define PL011_LCRH_FEN				BIT(4)
#define PL011_LCRH_WLEN_5BITS		BITS(0b00, 5)
#define PL011_LCRH_WLEN_6BITS		BITS(0b01, 5)
#define PL011_LCRH_WLEN_7BITS		BITS(0b10, 5)
#define PL011_LCRH_WLEN_8BITS		BITS(0b11, 5)
#define PL011_LCRH_SPS				BIT(7)

#define PL011_CR_UARTEN				BIT(0)
#define PL011_CR_LBE				BIT(7)
#define PL011_CR_TXE				BIT(8)
#define PL011_CR_RXE				BIT(9)
#define PL011_CR_DTR				BIT(10)
#define PL011_CR_RTS				BIT(11)
#define PL011_CR_RTSEN				BIT(14)
#define PL011_CR_CTSEN				BIT(15)

#define PL011_IMSC_CTSMIM			BIT(1)
#define PL011_IMSC_RXIM				BIT(4)
#define PL011_IMSC_TXIM				BIT(5)
#define PL011_IMSC_RTIM				BIT(6)
#define PL011_IMSC_FEIM				BIT(7)
#define PL011_IMSC_PEIM				BIT(8)
#define PL011_IMSC_BEIM				BIT(9)
#define PL011_IMSC_OEIM				BIT(10)

#define PL011_RIS_CTSRMIS			BIT(1)
#define PL011_RIS_RXRIS				BIT(4)
#define PL011_RIS_TXRIS				BIT(5)
#define PL011_RIS_RTRIS				BIT(6)
#define PL011_RIS_FERIS				BIT(7)
#define PL011_RIS_PERIS				BIT(8)
#define PL011_RIS_BERIS				BIT(9)
#define PL011_RIS_OERIS				BIT(10)

#define PL011_MIS_CTSMMIS			BIT(1)
#define PL011_MIS_RXMIS				BIT(4)
#define PL011_MIS_TXMIS				BIT(5)
#define PL011_MIS_RTMIS				BIT(6)
#define PL011_MIS_FEMIS				BIT(7)
#define PL011_MIS_PEMIS				BIT(8)
#define PL011_MIS_BEMIS				BIT(9)
#define PL011_MIS_OEMIS				BIT(10)

#define PL011_ICR_CTSMIC			BIT(1)
#define PL011_ICR_RXIC				BIT(4)
#define PL011_ICR_TXIC				BIT(5)
#define PL011_ICR_RTIC				BIT(6)
#define PL011_ICR_FEIC				BIT(7)
#define PL011_ICR_PEIC				BIT(8)
#define PL011_ICR_BEIC				BIT(9)
#define PL011_ICR_OEIC				BIT(10)

#define PL011_DMACR_RXDMAE			BIT(0)
#define PL011_DMACR_TXDMAE			BIT(1)
#define PL011_DMACR_DMAONERR		BIT(2)

#define PL011_ITCR_ITCR0			BIT(0)
#define PL011_ITCR_ITCR1			BIT(1)

#define PL011_ITIP_ITIP0			BIT(0)
#define PL011_ITIP_ITIP3			BIT(3)

#define PL011_ITOP_ITOP0			BIT(0)
#define PL011_ITOP_ITOP3			BIT(3)
#define PL011_ITOP_ITOP6			BIT(6)
#define PL011_ITOP_ITOP7			BIT(7)
#define PL011_ITOP_ITOP8			BIT(8)
#define PL011_ITOP_ITOP9			BIT(9)
#define PL011_ITOP_ITOP10			BIT(10)
#define PL011_ITOP_ITOP11			BIT(11)

#define PL011_TDR_TDR10_0_MASK		0x3FF

#ifndef __ASSEMBLER__

void pl011_init_uart(void *uart);

#endif  // !__ASSEMBLER__

#endif  // _PL011_H_
