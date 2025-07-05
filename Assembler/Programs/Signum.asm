// Program: Signum.asm
// Computes:  if R0>0
//                R1=1
//            else
//                R1=0
// USAGE: R0 has a preset value inside it

@0
D=M   
@15     // @positive
D;JGT
@1
M=0     // RAM[1] = 0 since its not positive
@14     // end loop
0;JMP
@1      // positive
M=1
@18     // end loop
0;JMP