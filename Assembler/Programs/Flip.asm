// Program: Flip.asm
// flips the values of
// RAM[0] and RAM[1].

@1
D=M  // store value
@18  // temp
M=D  // temp = D = RAM[1]

@0
D=M
@1
M=D  // RAM[1] = Ram[0]

@18
D=M
@0
M=D  // RAM[0] = temp
@19
0;JMP   // end loop