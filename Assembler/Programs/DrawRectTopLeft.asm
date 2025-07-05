// draws a filled rectangle at the screen's top left corner
// the rectangle's width is 16 pixels, and its height is RAM[0]
// input value at RAM[0]=n such that n >= 0.

// pseudo code 
// addr = SCREEN
// n=RAM[0]
// i=0
// LOOP:
// 	 if i > n goto END
// 	 RAM[addr] = -1	// sets it as 1111111111111111 which are 16 pixels
// 	 addr = addr + 32	// next row below
// 	 i += 1
// 	 goto LOOP
// 
// END:
//	 goto END


@0
D=M
@16
M=D	// store height in RAM[16]
@17	
M=0	// RAM[17]: i = 0

@16384	// screen address
D=A
@18
M=D	// base address

// (LOOP) flag
@17
D=M
@16
D=D-M
@55
D;JGT	// if i > n go to end

@18
A=M
M=-1	// RAM[18] = -1 (16 pixels long)

@17
M=M+1	// i++

@32
D=A
@18
M=D+M	// RAM[18] += 32 (goes to the next row of screen)

@32
0;JMP

@55
0;JMP