#define NOEXTERN
#include "board.h"
#undef NOEXTERN

const HWData boards[4] = {
	{
		.id = BOARD_RPI1_ID,
		.name = "Raspi1",
		.mmio_base = (void *)0x20000000
	},
	{
		.id = BOARD_RPI2_ID,
		.name = "Raspi2",
		.mmio_base = (void *)0x3F000000
	},
	{
		.id = BOARD_RPI3_ID,
		.name = "Raspi3",
		.mmio_base = (void *)0x3F000000
	},
	{
		.id = BOARD_RPI4_ID,
		.name = "Raspi4",
		.mmio_base = (void *)0x3F000000
	}
};

HWData const *board = NULL;

