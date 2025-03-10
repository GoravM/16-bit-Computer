# 16-bit-Computer
Made a 16 bit Computer in HDL, that can execute assembly code.


### Elementary Logic Gates
---
This Section lays the foundations of the project by implementing basic logic gates such as AND, OR, NOT using NAND gates. These gates are used to implement Xor, And16, Not16, Or16, Or8Way, DMux and Mux, Which are further used to implement Mux16, Mux4Way16, Mux8Way16, DMux4Way and Dmux8Way and etc these are very crucial fundamental Chips which will be used frequently in later sections. 


### Arithmetic Logic Unit
---
This section builds upon the Elementary Logic Gates section by first constructing the HalfAdder, FullAdder, and Add16 chips, which are essential for designing the Arithmetic Logic Unit (ALU). These components are then used to implement the complete ALU, which can perform a variety of arithmetic and logical operations, including addition, subtraction, and bitwise functions. The ALU is designed to work efficiently with control signals that determine the operation to be executed.

<p align="center"><img src="https://github.com/GoravM/16-bit-Computer/blob/main/Arithmetic%20Logic%20Unit/ALU%20Circuit.png" width="80%"></p>

<p align="center">Arithmetic Logic Unit Chip Circuit</p>


### Memory System
---
This Section focuses on the implementation of storage components. It began with the creation of elementary storage units such as the Bit, Register, and RAM8 chips. These Chips are then scaled up to form RAM64, RAM512, RAM4K and finally RAM16K. The implementation of the Program Counter (PC) allows sequential execution of instructions by keeping track of the next address to be fetched. These memory components play a crucial role in ensuring smooth execution and data storage within the system.

### 16-bit-Computer
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

