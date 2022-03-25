/*
 * Support for the mailbox interface
 *
 * Useful Links:
 *  https://github.com/raspberrypi/firmware/wiki/Mailboxes
 *  https://patchwork.kernel.org/project/linux-arm-kernel/patch/1430857666-18877-2-git-send-email-eric@anholt.net/
 *  https://forums.raspberrypi.com/viewtopic.php?t=165529
 */
//
#ifndef _MAILBOX_H_
#define _MAILBOX_H_

#include <stdint.h>

#include "board.h"

/*
 * Mailbox channels
 */
#define MAIL_CH_POWER			0
#define MAIL_CH_FRAMEBUF		1
#define MAIL_CH_VUART			2
#define MAIL_CH_VCHIQ			3
#define MAIL_CH_LEDS			4
#define MAIL_CH_BTNS			5
#define MAIL_CH_TOUCH			6
// unclear what channel 7 is
#define MAIL_CH_PROP_TO_VC		8
#define MAIL_CH_PROP_FROM_FC	9

/* Base offset of mailbox from mmio base */
#define MAIL_BASE		0xB880

/* Mailbox 0 offsets */
#define MAIL0_READ		(MAIL_BASE + 0x00)
#define MAIL0_POLL		(MAIL_BASE + 0x10)
#define MAIL0_SENDER	(MAIL_BASE + 0x14)
#define MAIL0_STATUS	(MAIL_BASE + 0x18)
#define MAIL0_CONFIG	(MAIL_BASE + 0x1C)

#define MAIL1_WRITE		(MAIL_BASE + 0x20)
#define MAIL1_POLL		(MAIL_BASE + 0x30)
#define MAIL1_SENDER	(MAIL_BASE + 0x34)
#define MAIL1_STATUS	(MAIL_BASE + 0x38)
#define MAIL1_CONFIG	(MAIL_BASE + 0x3c)

#define MAIL_FULL		0x80000000
#define MAIL_EMPTY		0x40000000

#ifdef __ASSEMBLER__

.extern mbox_read
.extern mbox_write

#else  /* C */

/*
 * Reads a word from the requested mailbox channel. Blocks until a word has
 * been read.
 *
 * channel		The channel to read from
 * Returns the value read.
 */
uint32_t mbox_read(uint8_t channel);

/*
 * Writes a word to the requested mailbox channel. Blocks until a word has been
 * read.
 *
 * channel		The channel to write to
 * value		The value to write
 */
void mbox_write(uint8_t channel, uint32_t value);

#endif  // __ASSEMBLER__

#endif  // _MAILBOX_H_
