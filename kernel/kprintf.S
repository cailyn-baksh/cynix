@ Helper functions for kprintf

.global doubledabble_shiftregister

@ Shifts a multibyte register for double dabble.
@ The register should use big-endian byte ordering
@ void doubledabble_shiftregister(void *register, uint32_t size)
doubledabble_shiftregister:
	@ r0 = ptr to register (readptr)
	@ r1 = size

	add r2,r0,r1	@ r2 = ptr to end of register
	mov r3,r0		@ r3 = writeptr

	@ r4, r5 are shifted words
	@ r6, r7 are scratch registers

	sub r6,r2,r0	@ r6 = number of bytes remaining after readptr
	cmp r6,#4		@ 4 or more bytes after readptr
		ldrge r4,[r0],#4
		revge r4,r4
		bge 1f
	cmp r6,#2		@ 2 or more bytes after readptr
		@ TODO: FIX THIS!!
		@ TODO: ldrhge is not valid
		@ldrhge r4,[r0],#2
		rev16ge r4,r4
		bge 1f
	@ else
		ldrb r4,[r0],#1

1:
	lsl r4,r4,#1

2:  @ while r0 < r2
	cmp r0,r2
	ble 3f
		@ read next word
		sub r6,r2,r0	@ r6 = number of bytes remaining after readptr
		cmp r6,#4		@ 4 or more bytes after readptr
			ldrge r5,[r0],#4
			revge r5,r5  @ swap endianness
			mov r7,#0
			bge 10f
		cmp r6,#2		@ 2 or more bytes after readptr
			@ TODO: FIX!!
			@ldrhge r5,[r0],#2
			rev16ge r5,r5  @ swap endianness
			movge r7,#16
			lslge r5,r5,r7  @ move bits so carry flag will be set properly
			bge 10f
		@ else
			ldrb r5,[r0],#1
			mov r7,#24
			lsl r5,r5,r7  @ move bits so carry flag will be set properly
10:

		lsl r5,r5,#1
		orrcs r4,r4,#1  @ carry into previous word

		@ write previous word
		sub r6,r3,r0	@ r6 = number of bytes remaining after writeptr
		cmp r6,#4		@ 4 or more bytes after writeptr
			revge r4,r4  @ swap endianness
			strge r4,[r3],#4
			bge 11f
		cmp r6,#2		@ 2 or more bytes after readptr
			rev16ge r4,r4  @ swap endianness
			@ TODO: FIX!!
			@strhge r4,[r3],#2
			bge 11f
		@ else
			strb r4,[r3],#1
11:

		@ move current word to previous word position
		@ r4 = r5 >> r7
		lsr r4,r5,r7
		b 2b
3:

	@ write last word
		sub r6,r3,r0	@ r6 = number of bytes remaining after writeptr
		cmp r6,#4		@ 4 or more bytes after writeptr
			revge r4,r4  @ swap endianness
			strge r4,[r3],#4
			@bge 11f
		cmp r6,#2		@ 2 or more bytes after readptr
			rev16ge r4,r4  @ swap endianness
			@ TODO: FIX!!
			@strhge r4,[r3],#2
			@bge 11f
		@ else
			strb r4,[r3],#1
