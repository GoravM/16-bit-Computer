// compute 1+2+...+n
// RAM[1] = 1+2+...+RAM[0]
// USAGE: Ram[0] already has value.

@0	// D = n = RAM[0]
D=M
@16	// i = 1
M=1
D=M
@17	// sum = 0
M=0

// (LOOP) flag
@0
D=M
@16
D=M-D	// D=i-n
@29
D;JGT	// D > 0 -> i-n > 0 -> i > n
@16
D=M	// D=i
@17
M=D+M 	// sum += i
@16
M=D+1	// i++
@13
0;JMP

// (STOP) flag
@17
D=M
@1
M=D
@16
M=0
@17
M=0
@38     // END loop
0;JMP