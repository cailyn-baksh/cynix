#include "kernel/common.h"
#include "kernel/board.h"
#include "kernel/uart.h"

.global init_uart1
.global uart1_putc
.global uart1_puts
.global kprintf

.section ".text"

@ nops r0 times
noploop:
	bne 0f
	subs r0,r0,#1
	nop
	b noploop
0:
	bx lr

@ Initializes UART1
init_uart1:
	push {r4-r6, lr}

	@ Get MMIO base address
	ldr r4,=board  @  typeof r4 = HWData **
	ldr r4,[r4]  @ typeof r4 = HWData *
	ldr r4,[r4, #HWData_mmio_base_offset]  @ r4 = board->mmio_base

	@ AUX_ENABLE register |= 1
	ldr r5,=AUX_ENABLE_OFFSET
	add r5,r4,r5
	
	ldr r6,[r5]
	orr r6,#1
	str r6,[r5]

	@ AUX_MU_CNTL register = 0
	ldr r5,=AUX_MU_CNTL_OFFSET
	add r5,r4,r5

	mov r6,#0
	str r6,[r5] 

	@ AUX_MU_LCR register = 3
	ldr r5,=AUX_MU_LCR_OFFSET
	add r5,r4,r5

	mov r6,#3
	str r6,[r5]

	@ AUX_MU_MCR register = 0
	ldr r5,=AUX_MU_MCR_OFFSET
	add r5,r4,r5

	mov r6,#0
	str r6,[r5]

	@ AUX_MU_IER reg = 0
	ldr r5,=AUX_MU_IER_OFFSET
	add r5,r4,r5

	mov r6,#0
	str r6,[r5]

	@ AUX1_MU_IIR reg = 0xc6
	ldr r5,=AUX_MU_IIR_OFFSET
	add r5,r4,r5

	mov r6,#0xc6
	str r6,[r5]

	@ set baud
	ldr r5,=AUX_MU_BAUD_OFFSET
	add r5,r4,r5

	ldr r6,=270
	str r6,[r5]

	@ map to gpio pins 14 and 15
	ldr r5,=GPFSEL1_OFFSET
	add r5,r4,r5

	ldr r6,[r5]
	bic r6,r6,#0x3F000  @ clear bits 12 to 17
	orr r6,r6,#(2 << 12) | (2 << 15)
	str r6,[r5]

	ldr r5,=GPPUD_OFFSET
	add r5,r4,r5

	mov r6,#0
	str r6,[r5]

	push {r0, lr}
	mov r0,#150
	bl noploop
	pop {r0, lr}

	ldr r5,=GPPUDCLK0_OFFSET
	add r5,r4,r5
	mov r6,#(1 << 14) | (1 << 15)
	str r6,[r5]

	push {r0, lr}
	mov r0,#150
	bl noploop
	pop {r0, lr}

	mov r6,#0
	str r6,[r5]

	@ enable Tx, Rx
	ldr r5,=AUX_MU_CNTL_OFFSET
	add r5,r4,r5
	mov r6,#3
	str r6,[r5]

0:
	ldr r0,=board
	pop {r4-r6, pc}


@ writes a character to uart1
@ takes the character in r0
uart1_putc:
	push {r4-r6, lr}

	@ get MMIO base address
	ldr r6,=board  @ r6 = ptr to ptr to HWData struct
	ldr r6,[r6]  @ r6 = ptr to HWData struct
	ldr r6,[r6, #HWData_mmio_base_offset]

	ldr r4,=AUX_MU_LSR_OFFSET
	add r4,r6,r4

1:	nop
	ldr r5,[r4]
	tst r5,#0x20
	beq 1b  @ do while !(AUX_MU_LSR & 0x20)

	@ Write char
	ldr r4,=AUX_MU_IO_OFFSET
	add r4,r6,r4
	strb r0,[r4]

0:
	pop {r4-r6, pc}
