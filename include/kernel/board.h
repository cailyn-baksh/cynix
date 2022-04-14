/*
 * Hardware defines
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "common.h"

/* Board IDs */
#define BOARD_RPI1_ID	0x1
#define BOARD_RPI2_ID	0x2
#define BOARD_RPI3_ID	0x3
#define BOARD_RPI4_ID	0x4

#define GPFSEL0_OFFSET				0x200000
#define GPFSEL1_OFFSET				0x200004
#define GPFSEL2_OFFSET				0x200008
#define GPFSEL3_OFFSET				0x20000C
#define GPFSEL4_OFFSET				0x200010
#define GPFSEL5_OFFSET				0x200014

#define GPSET0_OFFSET				0x20001C
#define GPSET1_OFFSET				0x200020
#define GPCLR0_OFFSET				0x200028 
#define GPCLR1_OFFSET				0x20002C
#define GPLEV0_OFFSET				0x200034
#define GPLEV1_OFFSET				0x200038
#define GPEDS0_OFFSET				0x200040
#define GPEDS1_OFFSET				0x200044
#define GPREN0_OFFSET				0x20004C
#define GPREN1_OFFSET				0x200050
#define GPFEN0_OFFSET				0x200058
#define GPFEN1_OFFSET				0x20005C
#define GPHEN0_OFFSET				0x200064
#define GPHEN1_OFFSET				0x200068
#define GPLEN0_OFFSET				0x200070
#define GPLEN1_OFFSET				0x200074
#define GPAREN0_OFFSET				0x20007C
#define GPAREN1_OFFSET				0x200080
#define GPAFEN0_OFFSET				0x200088
#define GPAFEN1_OFFSET				0x20008C
#define GPPUD_OFFSET				0x200094
#define GPPUDCLK0_OFFSET			0x200098
#define GPPUDCLK1_OFFSET			0x20009C

/* UART offsets */
#define UART0						0x00201000
#define UART1						NULL
#define UART2						0x00201400
#define UART3						0x00201600
#define UART4						0x00201800
#define UART5						0x00201A00

#define PL011_UART_DR				0x00
#define PL011_UART_RSRECR			0x04
#define PL011_UART_FR				0x18
#define PL011_UART_ILPR				0x20
#define PL011_UART_IBRD				0x24
#define PL011_UART_FBRD				0x28
#define PL011_UART_LCRH				0x2C
#define PL011_UART_CR				0x30
#define PL011_UART_IFLS				0x34
#define PL011_UART_IMSC				0x38
#define PL011_UART_RIS				0x3C
#define PL011_UART_MIS				0x40
#define PL011_UART_ICR				0x44
#define PL011_UART_DMACR			0x48
#define PL011_UART_ITCR				0x80
#define PL011_UART_ITIP				0x84
#define PL011_UART_ITOP				0x88
#define PL011_UART_TDR				0x8C


#ifdef __ASSEMBLER__
/* asm-only */

// TODO: write script to automate this
#define HWData_id_offset		0x0
#define HWData_name_offset		0x1
#define HWData_mmio_base_offset	0xA

#else
/* C-only */

#include <stddef.h>
#include <stdint.h>

/*
 * Hardware-specific constants for every supported board
 *
 * name
 *   A human-readable name for this board
 * mmio_base
 *   A pointer to the beginning of memory-mapped I/O
 */
typedef struct __attribute__((__packed__)) {
	uint8_t id;
	char name[8];
	int : 8;  // forced null terminator for name
	void *mmio_base;
} HWData;

#ifndef NOEXTERN
extern const HWData boards[4];

/*
 * The kernel expects board to always have a non-null value, thus board must
 * immediately initialized on startup before any functions are called.
 */
extern volatile HWData const *board;
// TODO: change board to a non-const struct that is populated by mrc & devicetree
#endif  // NOEXTERN

#define MMIO(offset) (*((volatile uint32_t *)board->mmio_base + offset))

/*
 * Read a type from an MMIO offset
 *
 * type		The type to read
 * offset	The offset to read from
 * Returns the value read
 */
#define MMIO_READ(type, offset) (*(type *)(board->mmio_base + offset))

/*
 * Write a value of a given type to an MMIO offset
 *
 * type		The type being written
 * offset	The offset to write to
 * value	The value to write
 * Returns the value of the offset written to after the write operation.
 */
#define MMIO_WRITE(type, offset, value) (*(type *)(board->mmio_base + offset) = value)

#endif  // __ASSEMBLER__
#endif  // _BOARD_H_

