/* code modified from https://wiki.osdev.org/Raspberry_Pi_Bare_Bones#Pi_3.2C_4 */
.section ".text.boot"
.globl _start

.org 0x80000

.extern MMIO_BASE

// halt the processor
halt:
    wfe
    bl halt

_start:
    // firmware ensures that this will only run on cpu0
    // set stack pointer to address of _start
    ldr x5, =_start             // load address of _start into x5
    mov sp, x5                  // move x5 into sp

    // zero bss for crt
    ldr x5, =__bss_start        // load address of __bss_start into x5
    ldr x6, =__bss_size         // load address of __bss_size into x6

3:
    cbz w6, 4f                  // if w6 == 0, jump forward to local label 4
    str xzr, [x5], #8           // store 64-bit 0 (xzr) into address at x5, then add 8 to x5
    sub w6, w6, #1              // w6 = w6 - 1
    cbnz w6, 3b                 // if w6 != 0, jump backward to local label 3

4:
    // detect board type
    mrs x9, midr_el1            // read main id register into x9
    and x9, x9, #0xFFF0         // turn off all bits except 4:15
    // compare and jump to corresponding branch
    cmp x9, #0xD030             // if x9 == 0xD030 jump forwards to 5
    beq 5f
    cmp x9, #0xD080             // if x9 == 0xD080 jump forwards to 6
    beq 6f
    // no match, halt
    bl halt

5:
    // raspberry pi 3
    mov x9, #0x3F000000
    str x9, =MMIO_BASE
    b 7f

6:
    // raspberry pi 4
    mov x9, #0x3F000000
    str x9, =MMIO_BASE

7:
    // start kernel
    bl kernel_main              // jump to kernel_main
    bl halt                     // halt if kernel returns (it shouldnt)
