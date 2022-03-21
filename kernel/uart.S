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
	bne _noploop_end
	subs r0,r0,#1
	nop
	b noploop

_noploop_end:
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

@ void kprintf(const char *format, ...);
___kprintf:
	@ r0 is format string
	@ r1-r3 are format arguments
	@ any other format arguments are pushed onto the stack in reverse order
	push {r1-r3}  @ Put all format arguments on the stack
	mov r1,sp  @ remember where varargs begins on the stack
	push {r4-r8, fp, lr}  @ Save registers

	sub sp,sp,#4  @ Move stack pointer to end of locals
	mov fp,sp  @ move frame pointer to bottom of stack

	@ Declare locals
	#define length fp

	#define FMTLEN_h	1
	#define FMTLEN_hh	2
	#define FMTLEN_l	3
	#define FMTLEN_ll	4
	#define FMTLEN_z	5
	#define FMTLEN_j	6
	#define FMTLEN_t	7

	@ r0-r5 are scratch registers  (i couldve kept it to r0-r3 if it weren't for decimal)
	mov r6,r0  @ r6 = pointer to format string
	mov r7,r1  @ r7 = pointer to next arg
	mov r8,#0  @ r8 = number of chars printed

1:
	@ length = 0
	mov r2,#0
	str r2,[length]

	ldrb r0,[r6],#1  @ read byte and increment ptr

	cmp r0,#0  @ Return on null terminator
		beq 0f
	cmp r0,#'%'  @ Format specifier
		beq 2f

	@ Print the char normally
	bl _kprintf_putc
	b 1b

2:  @ handle format specifiers
	ldrb r0,[r6],#1  @ read next char

	cmp r0,#0  @ Return on null terminator
		beq 0f

	@ Length specifiers
	
	cmp r0,#'h'  @ half word
	bne 3f
		ldrb r0,[r6],#1  @ read next char
		cmp r0,#0  @ return on null terminator
			beq 0f

		cmp r0,#'h'  @ half half word; 1 byte
			moveq r1,#1
		@ else only half word
			movne r1,#__SIZEOF_SHORT__
			sub r6,r6,#1  @ unread last char
		str r1,[length]
		beq 2b  @ move to next char
3:
	cmp r0,#'l'  @ long
	bne 4f
		ldrb r0,[r6],#1  @ read next charr
		cmp r0,#0  @ return on null terminator
			beq 0f

		cmp r0,#'l'  @ long long
			moveq r1,#__SIZEOF_LONG_LONG__
		@ else only long
			movne r1,#__SIZEOF_LONG__
			sub r6,r6,#1  @ unread last char
		str r1,[length]
		beq 2b
4:

5:	@ switch type specifiers
	cmp r0,#'d'  @ Signed integer
	cmpne r0,#'i'
		beq _kprintf_fmtdec
	cmp r0,#'u'  @ Unsigned integer
		beq _kprintf_fmtudec
	cmp r0,#'x'  @ Hex uint (lower)
		beq _kprintf_fmthexl
	cmp r0,#'X'  @ Hex uint (upper)
		beq _kprintf_fmthexu
	cmp r0,#'o'  @ Octal uint
		beq _kprintf_fmtoct
	cmp r0,#'b'  @ Binary uint
		beq _kprintf_fmtbin
	cmp r0,#'s'  @ String
		beq _kprintf_fmtstr
	cmp r0,#'c'  @ Character
		beq _kprintf_fmtchr
	cmp r0,#'p'  @ Pointer
		beq _kprintf_fmtptr
	cmp r0,#'n'  @ Store number of chars printed
		beq _kprintf_fmtsto

	@ no cases matched; handle char normally and stop processing
	@ %% case falls through to here
	bl _kprintf_putc
	b 1b
0:
	add sp,sp,#4  @ move stack pointer to beginning of locals
	pop {r4-r8, fp, lr}
	pop {r1-r3}
	bx lr

@ %s
_kprintf_fmtstr:
	ldr r1,[r7],#4  @ Load address of string and increment ptr to next arg

50:
	ldrb r0,[r1],#1  @ Load character

	cmp r0,#0
	beq 1b  @ Stop at null terminator
	
	bl _kprintf_putc  @ print char then loop
	b 50b

@ %c
_kprintf_fmtchr:
	@ NOTE: if kprintf starts misbehaving this could be the cause
	@ NOTE: since i assumed that chars are padded to 4 bytes, but i cant find
	@ NOTE: it formally stated anywhere (also this seems to work reliably)
	ldrb r0,[r7],#4  @ Read character then increment ptr
	bl _kprintf_putc
	b 1b

@ %x
_kprintf_fmthexl:
	ldr r3,=_kprintf_hexlower
	b 50f
@ %X, %p
_kprintf_fmthexu:
_kprintf_fmtptr:  @ TODO: make pointers have leading zeroes
	ldr r3,=_kprintf_hexupper

50:
	ldr r0,[fp,#4]  @ r0 = length
	ldr r1,[r7],#4  @ Load word and increment ptr
	mov r2,#28  @ r2 = shift amount

	cmp r1,#0  @ skip loop if 0
	bne 51f
	mov r0,#'0'
	bl _kprintf_putc
	b 1b

51:
	mov r0,r1
	lsrs r0,r0,r2  @ r0 >>= r2
	beq 52f  @ dont print anything if this is a leading zero
	and r0,r0,#0xF  @ clear high bits
	ldrb r0,[r3,r0]  @ load offset from array of hex chars (r0 = [r3+r0])
	bl _kprintf_putc

52:
	subs r2,r2,#4  @ decrease shift amount and update flags
	bpl 51b  @ loop while r2 > 0

	b 1b

@ %o
_kprintf_fmtoct:
	ldr r1,[r7],#4  @ Load word and increment ptr
	mov r2,#30  @ r2 = shift amount (highest octal digit is truncated to 2 bits in an multiple of 8 word)

	cmp r1,#0  @ skip loop if 0
	bne 50f
	mov r0,#'0'
	bl _kprintf_putc
	b 1b

50:
	mov r0,r1
	lsrs r0,r0,r2  @ r0 >>= r2
	beq 51f  @ dont print anything if this is a leading zero
	and r0,r0,#7  @ clear high bits
	orr r0,r0,#0x30  @ convert to ascii
	bl _kprintf_putc

51:
	subs r2,r2,#3  @ decrease shift amount and update flags
	bpl 50b  @ loop while r2 > 0
	b 1b

@ %b
_kprintf_fmtbin:
	ldr r1,[r7],#4  @ Load word and increment ptr
	mov r2,#31

50:
	mov r0,r1
	lsrs r0,r0,r2  @ r0 >>= r2
	beq 51f  @ dont print anything if this is a leading zero
	and r0,r0,#1  @ clear high bits
	orr r0,r0,#0x30  @ convert to ascii
	bl _kprintf_putc

51:
	subs r2,r2,#1  @ decrease shift amount and update flags
	bpl 50b  @ loop while r2 > 0
	b 1b

@ %n
_kprintf_fmtsto:
	ldr r0,[r7],#4  @ load addr and increment ptr
	str r6,[r0]
	b 1b

@ %d, %i
_kprintf_fmtdec:
	@ set zero flag
	movs r0,#0
	b 48f
@ %u
_kprintf_fmtudec:
	@ clear zero flag
	movs r0,#1
48:
	@ use r3:r2:r1 as a 72-bit register for double dabble
	ldr r1,[r7],#4  @ Load word and increment ptr
	mov r2,#0
	mov r3,#0

	@ if zero flag is set then check sign
	bne 49f  @ skip if zero flag clear

	@ check if sign bit set, adjusting for length
	@ if (1 << (length * 8) == 1) 
	ldr r4,[fp,#4]
	lsl r4,r4,#3
	sub r4,r4,#1
	mov r5,#1
	lsl r4,r5,r4
	tst r1,r4
	beq 49f
		mov r0,#'-'
		bl _kprintf_putc

		@ negate
		mvn r1,r1
		add r1,r1,#1
		
		sub r4,r4,#1
		and r1,r1,r4
49:
	mov r0,#32  @ r0 = number of shifts left
50:
	@ check each BCD nybble to see if its >= 5
	mov r4,#0  @ 2^32 is 10 digits in decimal, so 10 nybbles is the maximum
	push {r0}  @ i desperately need another register

51:
		cmp r4,#8  @ r4 addresses word in r2  (r4 < 8)
		bge 52f
			lsl r5,r4,#2  @ r5 = r4 * 4

			lsr r0,r2,r5  @ r0 = r2 >> r5
			and r0,#0xF  @ clear all bits except for low nybble

			cmp r0,#5  @ if (r5 >= 5) nybble += (3 << r5)
				movge r0,#3
				addge r2,r2,r0,lsl r5

			b 53f
52:		@ else r4 addresses word in r3
			sub r5,r4,#8  @ r5 = (r4 - 8) * 4
			lsl r5,r5,#2

			lsr r0,r3,r5
			and r0,#0xF

			cmp r0,#5  @ if (r5 >= 5) r3 += (3 << r5)
				movge r0,#3
				addge r3,r3,r0,lsl r5

53:

		add r4,r4,#1  @ loop while r4 < 10
		cmp r4,#16
		blt 51b

	@ r3:r2:r1 << 1
	lsl r3,r3,#1
	lsls r2,r2,#1
	adc r3,r3,#0
	lsls r1,r1,#1
	adc r2,r2,#0

	pop {r0}
	subs r0,r0,#1
	bne 50b  @ while r0 != 0

	@ print each nybble
	mov r4,#16

54:
	cmp r4,#8  @ r4 address word in r2
	bge 55f
		lsl r1,r4,#2  @ r1 = r4 * 4

		lsrs r5,r2,r1  @ r5 = r2 >> r1
		cmpeq r3,#0
		
		andne r5,r5,#0xF  @ clear high bits if not leading zero
		orrne r0,r5,#0x30  @ convert to ascii

		b 56f
55:	@ else r4 addresses word in r3
		sub r1,r4,#8
		lsl r1,r1,#2  @ r1 = (r4 - 8) * 4
		
		lsrs r5,r3,r1  @ r5 = r3 >> r1

		andne r5,r5,#0xF  @ clear high bits if not leading zero
		orrne r0,r5,#0x30  @ convert to ascii
		
56:
	blne _kprintf_putc

	@ loop while r4 > 0
	cmp r4,#0
	ble 1b
	sub r4,r4,#1
	b 54b

@ Prints individual character for uart1_printf
_kprintf_putc:
	push {lr}
	bl uart1_putc  @ print character
	add r8,r8,#1  @ increment chars printed counter
	pop {lr}
	bx lr

#undef length

_kprintf_hexupper: .ascii "0123456789ABCDEF"
_kprintf_hexlower: .ascii "0123456789abcdef"

