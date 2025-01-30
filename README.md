# RISC-V_Processor_Simulator
This project implements a 32-bit RISC-V Single-Cycle Processor with the following instructions:
* ADD, SUB, ADDI, XOR, ANDI, SRA
* LW, SW
* BLT, JALR

Instructions are saved in “23instMem-x” (in unsigned decimal format, where each line is one byte and stored in little-endian format). The files “23all.txt”, “23sw.txt”, and “23r-type.txt” show the actual assembly program for each of “instMem-X” files. Each file has 3 columns that show the memory address (in hex), the instruction (in hex), and the actual assembly instruction. The “r-type” file only contains r-type and i-type (except LW) instructions. The “sw” file adds memory instructions, excluding branch. The “all” file that combines all these instructions, so it serves as a comprehensive test file.

This is my design for the controller and datapath:

![Screenshot 2025-01-29 at 5 06 02 PM](https://github.com/user-attachments/assets/8a62d60b-77f0-4da7-a6a4-aa837e8e5e2c)


In my implementation, I encapsulated the control codes with an enum of instructions called Op, whose values include ADD, SRA, BLT, NONE, etc. Through the use of various if-statements, I implemented the MUXes that select between two inputs. Although I didn’t explicitly use control codes, the general idea I had was as follows:

![Screenshot 2025-01-29 at 5 06 21 PM](https://github.com/user-attachments/assets/739227e3-ba8f-46cb-87cf-287040498de5)
