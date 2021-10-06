.section ".text.boot"
.globl _start

.org 0x80000

.globl MMIO_BASE

// halt the processor
halt:
    wfe
    bl halt

_start:
    // firmware *does not* ensure that this will only run on cpu0 >:(
    // set stack pointer to address of _start
    ldr x5, =_start
    mov sp, x5

    // zero bss for crt
    ldr x5, =__bss_start
    ldr x6, =__bss_size
3:
    cbz w6, 4f
    str xzr, [x5], #8
    sub w6, w6, #1
    cbnz w6, 3b

    // she/her
    // they/them
    // he/him
    // it/its
    // xe/xyr

    // pronouns in bios
    // ~~yes i know its technically not a bios but shhh~~

4:
    // load address of MMIO_BASE into x10
    ldr x10, =MMIO_BASE

    // detect board type
    mrs x9, midr_el1            // read main id register into x9
    lsr x9, x9, #4              // turn off all bits except 4:15
    and x9, x9, #0xFFF
    // compare and jump to corresponding branch
    cmp x9, #0xD03
    beq 5f
    cmp x9, #0xD08
    beq 6f
    // no match, halt
    bl halt

5:
    // raspberry pi 3
    mov x9, #0x3F000000
    b 7f

6:
    // raspberry pi 4
    mov x9, #0xFE000000

7:
    str x9, [x10]               // set MMIO_BASE to proper value
    // start kernel
    bl kernel_main              // jump to kernel_main
    bl halt                     // halt if kernel returns (it shouldnt)
