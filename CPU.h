#include <iostream>
#include <bitset>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <stdint.h>
using namespace std;


class instruction {
public:
    bitset<32> instr;//instruction
    instruction(bitset<32> fetch); // constructor

};

class CPU {
public:
    enum Op {
        ADD,
        SUB,
        ADDI,
        XOR,
        ANDI,
        SRA,
        LW,
        SW,
        BLT,
        JALR,
        ZERO
    };
    
    CPU();
    unsigned long readPC();
    Op getInstr();
    void UpdatePC(unsigned long newPC);
    void UpdateClock();
    int32_t getRS1();
    int32_t getRS2();
    int32_t getRD();
    int32_t getImm();
    int readClock();
    int32_t readReg(int32_t reg);
    int32_t getMem(int32_t address);
    void setMem(int32_t address, int32_t val);
    bitset<32> Fetch(bitset<8> *instmem);
    bool Decode(instruction* instr);
    int32_t Execute(Op cur_instr, int32_t rs1, int32_t rs2, int32_t imm);
    void WriteBack(int32_t val);
    
private:
    int dmemory[4096]; //data memory byte addressable in little endian fashion;
    unsigned long PC; //pc
    unsigned long clock;
    int regfile[32];
    Op cur_instr;
    int32_t rs1, rs2, rd;
    int32_t imm;
};
