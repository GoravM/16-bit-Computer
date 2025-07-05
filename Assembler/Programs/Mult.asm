// ASSIGNMENT 1
// Mult 
// RAM[2] = RAM[1] * RAM[0].

@16
M=0	// init i = 0
@2
M=0

// (LOOP) flag
// if i = RAM[0] END
@0
D=M
@16
D=D-M
@28
D;JEQ	// if RAM[0] - i == 0

@16
M=M+1	// i++
@1
D=M
@2
M=D+M	// RAM[2] = RAM[2] + RAM[1]
@10
0;JMP

@28
0;JMP