// for (i=0; i<n; i++){
//    arr[i] = -1
// }

// suppose arr=100 && n=10.

// arr=100
@100
D=A
@16 // memory for arr, RAM[16] = arr = 100
M=D

// n = 10
@10
D=A
@17 // memory for n, RAM[17] = n = 10
M=D

// init i = 0
@18 // memory for i, RAM[18] = i = 0
M=0

// loop flag
// if (i==n) goto End
@18
D=M
@17
D=D-M
@48 // goto END
D;JEQ   // if D == 0, (i - n == 0)

// else
// RAM[arr+i] = -1
@16
D=M
@18
A=D+M
M=-1

// i++
@18
M=M+1

@23     // loop line 23
0;JMP   // go loop

// END flag
@48
0;JMP   // END loop