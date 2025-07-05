# 16-bit-Computer
Made a 16 bit Computer in HDL, that can execute assembly code.


## Elementary Logic Gates
---
This Section lays the foundations of the project by implementing basic logic gates such as AND, OR, NOT using NAND gates. These gates are used to implement Xor, And16, Not16, Or16, Or8Way, DMux and Mux, Which are further used to implement Mux16, Mux4Way16, Mux8Way16, DMux4Way and Dmux8Way and etc these are very crucial fundamental Chips which will be used frequently in later sections. 


## Arithmetic Logic Unit
---
This section builds upon the Elementary Logic Gates section by first constructing the HalfAdder, FullAdder, and Add16 chips, which are essential for designing the Arithmetic Logic Unit (ALU). These components are then used to implement the complete ALU, which can perform a variety of arithmetic and logical operations, including addition, subtraction, and bitwise functions. The ALU is designed to work efficiently with control signals that determine the operation to be executed.

<p align="center"><img src="https://github.com/GoravM/16-bit-Computer/blob/main/Arithmetic%20Logic%20Unit/ALU%20Circuit.png" width="80%"></p>

<p align="center">Arithmetic Logic Unit Chip Circuit</p>


## Memory System
---
This Section focuses on the implementation of storage components. It began with the creation of elementary storage units such as the Bit, Register, and RAM8 chips. These Chips are then scaled up to form RAM64, RAM512, RAM4K and finally RAM16K. The implementation of the Program Counter (PC) allows sequential execution of instructions by keeping track of the next address to be fetched. These memory components play a crucial role in ensuring smooth execution and data storage within the system.

## 16-bit-Computer
---
This section integrates all previously implemented components—logic gates, ALU, and memory system—into a fully functional 16-bit computer capable of executing assembly instructions. The computer consists of a Central Processing Unit (CPU), Memory (RAM), and ROM (Read-Only Memory), which together form a system capable of handling basic computation, control flow, and data storage. The CPU interacts with memory and executes instructions stored in ROM, while the ALU performs arithmetic and logical operations. Additionally, the system includes allocated memory registers for display output (8K registers) and a dedicated register (1 register) for keyboard inputs, enabling basic input and output functionality.

#### CPU Architecture
---
The CPU is responsible for executing instructions by processing data through various registers, multiplexers, and the ALU. The architecture includes an A register, D register, and the Program Counter (PC), which work together with control signals to manage data flow and instruction execution. The ALU performs arithmetic and logical operations, while the instruction input determines how data is processed, stored and executed.
<p align="center"><img src="https://github.com/GoravM/16-bit-Computer/blob/main/16-bit-Computer/CPU%20Architecture.png" width="50%"></p> 
<p align="center">CPU Architecture Diagram</p>

#### Complete Computer Circuit
---
The 16-bit computer integrates the CPU, Memory, and ROM to create a system capable of executing preloaded programs. The ROM32K stores the program instructions, which are fetched and executed sequentially. The Memory unit handles data storage and retrieval, while the CPU controls execution and updates the Program Counter (PC) accordingly. The interaction between these components enables the execution of complex instructions and supports fundamental computing operations.
<p align="center"><img src="https://github.com/GoravM/16-bit-Computer/blob/main/16-bit-Computer/Computer%20Circuit.png" width="50%"></p> 
<p align="center">Computer Circuit Diagram</p>

#### C-Instruction
---
The C-Instruction is one of the two types of instructions in the Hack assembly language, used for performing computations, storing results, and controlling program flow. The opcode for a C-Instruction is always 1. It follows the syntax: dest = comp ; jump

Where:
- comp specifies the computation to be performed by the ALU.
- dest determines where the computed result is stored (e.g., A register, D register, RAM).
- jump defines the conditions under which the program counter (PC) should jump to a different instruction.


<p align="center"><img src="https://github.com/GoravM/16-bit-Computer/blob/main/16-bit-Computer/C%20Instruction.jpg" width="50%"></p> 
<p align="center">C-Instruction</p>

#### A-Instruction
---
The A-Instruction is the second type of instruction in the Hack assembly language, which is used for performing computations, storing results, and controlling program flow. it follows the syntax: @value

Where value is a 15-bit constant or a label referring to a memory address. The opcode is always 0, distinguishing it from a C-Instruction.

## Assembler
---
The Assembler component translates Hack assembly language into binary machine code compatible with the 16-bit computer architecture. It processes files containing both A-instructions (e.g., `@value`) and C-instructions (e.g., `dest=comp;jump`) and outputs a binary file with corresponding 16-bit machine instructions.

This implementation parses each line by:

1. **Ignoring Comments and Whitespace**:  
   Inline and full-line comments (starting with `//`) are sanitized before translation. To make it easier to write and test code, the assembler inserts a 16-bit `0` binary (`0000000000000000`) into the output for every comment line or empty line. This allows users to write richly documented programs without disrupting the binary instruction flow or when tracing instruction flow when line positions no longer aligns.

2. **Translating A-Instructions**:  
   A-instructions (`@value`) are converted to binary with the opcode bit as `0`, followed by a 15-bit representation of the variable `value`.

3. **Translating C-Instructions**:  
   C-instructions are parsed into their `dest`, `comp`, and `jump` components. Each component is mapped to its binary equivalent, and the full instruction is constructed with a leading `1` opcode.

Example:

```asm
// This is a comment
@1          // A-instruction: @value
D           // C-instruction: comp
D=M+1       // C-instruction: dest=comp
0;JMP       // C-instruction: comp;jump
M=D+A;JMP   // C-instruction: dest=comp;jump
``` 

Produces:

```Binary output
0000000000000000    // Comment line -> binary 0
0000000000000001    // A-instruction: @1
1000001100000000    // C-instruction: D
1001110111010000    // C-instruction: D=M+1
1000101010000111    // C-instruction: 0;JMP
1000000010001111    // C-instruction: M=D+A;JMP
```

This design choice (inserting 0s for comments) ensures the program length remains consistent, aiding in debugging and step-by-step hardware simulation. While the assembler can function without inserting these 0s, omitting them can make debugging more difficult, especially due to the address-dependent nature of A-instructions and the challenge of tracing instruction flow when line positions no longer aligns.
