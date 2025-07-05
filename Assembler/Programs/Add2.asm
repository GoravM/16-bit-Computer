// Program: Add2.asm
// RAM[2] = RAM[0] + RAM[1]
// USAGE: RAM[0] and RAM[1] have preset values

@0      // RAM[0]
D=M     // D=RAM[0]

@1
D=D+M   // D=D+RAM[1]

@2
M=D     // RAM[2] = D
@13
0;JMP   // End loop