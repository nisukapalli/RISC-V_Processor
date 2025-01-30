# RISC-V_Processor
This project implements a 32-bit RISC-V Single-Cycle Processor with the following instructions:
* ADD, SUB, ADDI, XOR, ANDI, SRA
* LW, SW
* BLT, JALR

Instructions are saved in “23instMem-x” (in unsigned decimal format, where each line is one byte and stored in little-endian format). The files “23all.txt”, “23sw.txt”, and “23r-type.txt” show the actual assembly program for each of “instMem-X” files. Each file has 3 columns that show the memory address (in hex), the instruction (in hex), and the actual assembly instruction. The “r-type” file only contains r-type and i-type (except LW) instructions. The “sw” file adds memory instructions, excluding branch. The “all” file that combines all these instructions, so it serves as a comprehensive test file.

This is my design for the controller and datapath:
https://github.com/nisukapalli/RISC-V_Processor/issues/1#issue-2819673783
