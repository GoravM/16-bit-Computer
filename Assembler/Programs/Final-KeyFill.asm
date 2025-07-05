// ASSIGNMENT 2
// Fill
// press and hold any key and the screen turns black
// if nothing is being pressed then the screen turns white
// memory address for screen starts at RAM[16384] ends at RAM[24575], 8191 iterations
// memory address for key is at RAM[24576]

// the binary file has been done without making the comments "0000000000000000".

// pseudo code
// OGLOOP1
//	make screen white loop?
//	if key>0:
//		make screen black loop?
//	else:
//		Do nothing
// endOGLOOP1


// main

@16384	//(MainLOOP flag)
D=A
@17
M=D	// init address (also resets)
@8192
D=A
@16
M=D	// RAM[16]: n = 8192
@24576
D=M	// get key
@32	//(BLACK)
D;JGT
@14	//(WHITE)
0;JMP

@16	//(WHITE)
D=M	// n
@17	//(WLOOP)
D=M
A=D	// A = addr
M=0	// RAM[A] = 0 (white)
D=D+1	// addr++
A=D
@17
M=D	// hold new address in RAM[17]
@16
D=M
D=D-1	// n--
M=D
@0	//(MainLOOP)
D;JEQ	// n==0 -> main
@16	//(WLOOP)
0;JMP

@16	//(BLACK)
D=M	// n
@17	//(BLOOP)
D=M
A=D	// A = addr
M=-1	// RAM[A] = -1 (black)
D=D+1	// addr++
A=D
@17
M=D	// hold new address in RAM[17]
@16
D=M
D=D-1	// n--
M=D
@0	//(MAINLOOP)
D;JEQ	// n==0 -> main
@32	//(BLOOP)
0;JMP