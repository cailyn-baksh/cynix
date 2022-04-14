/*
 * Define constants and macros for GPIO
 */
#ifndef _GPIO_H_
#define _GPIO_H_

/* Offsets */
#define GPFSEL0						0x00200000
#define GPFSEL1						0x00200004
#define GPFSEL2						0x00200008
#define GPFSEL3						0x0020000C
#define GPFSEL4						0x00200010
#define GPFSEL5						0x00200014
#define GPSET0						0x0020001C
#define GPSET1						0x00200020
#define GPCLR0						0x00200028
#define GPCLR1						0x0020002C
#define GPLEV0						0x00200034
#define GPLEV1						0x00200038
#define GPEDS0						0x00200040
#define GPEDS1						0x00200044
#define GPREN0						0x0020004C
#define GPREN1						0x00200050
#define GPFEN0						0x00200058
#define GPFEN1						0x0020005C
#define GPHEN0						0x00200064
#define GPHEN1						0x00200068
#define GPLEN0						0x00200070
#define GPLEN1						0x00200074
#define GPAREN0						0x0020007C
#define GPAREN1						0x00200080
#define GPAFEN0						0x00200088
#define GPAFEN1						0x0020008C

// BCM2711
#define GPIO_PUP_PDN_CNTRL_REG0		0x002000E4
#define GPIO_PUP_PDN_CNTRL_REG1		0x002000E8
#define GPIO_PUP_PDN_CNTRL_REG2		0x002000EC
#define GPIO_PUP_PDN_CNTRL_REG3		0x002000F0

// Other Pis
#define GPPUD						0x00200094
#define GPPUDCLK0					0x00200098
#define GPPUDCLK1					0x0020009C


#define GPPUD_OFF					0b00
#define GPPUD_PDEN					0b01
#define GPPUD_PUEN					0b10

#endif  // _GPIO_H_
