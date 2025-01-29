#include "CPU.h"

instruction::instruction(bitset<32> fetch)
{
    //cout << fetch << endl;
    instr = fetch;
    //cout << instr << endl;
}

CPU::CPU()
{
    PC = 0; //set PC to 0
    clock = 0;
    cur_instr = ZERO;
    rs1 = 0;
    rs2 = 0;
    rd = 0;
    imm = 0;
    for (int i = 0; i < 4096; i++) //copy instrMEM
    {
        dmemory[i] = (0);
    }
    for (int i = 0; i < 32; i++)
    {
        regfile[i] = (0);
    }
}

void CPU::UpdatePC(unsigned long newPC)
{
    PC = newPC;
}

void CPU::UpdateClock()
{
    clock++;
}

unsigned long CPU::readPC()
{
    return PC;
}

CPU::Op CPU::getInstr()
{
    return cur_instr;
}

int32_t CPU::getRS1()
{
    return rs1;
}

int32_t CPU::getRS2()
{
    return rs2;
}

int32_t CPU::getRD()
{
    return rd;
}

int32_t CPU::getImm()
{
    return imm;
}

int CPU::readClock()
{
    return clock;
}

int32_t CPU::readReg(int32_t reg)
{
    return regfile[reg];
}

int32_t CPU::getMem(int32_t address)
{
    int val = 0;
    val |= dmemory[address];
    val |= (dmemory[address + 1] << 8);
    val |= (dmemory[address+ 2] << 16);
    val |= (dmemory[address + 3] << 24);
    return val;
}

void CPU::setMem(int32_t address, int32_t val)
{
    cerr << "ADDRESS: " << address << ", VAL: " << val << endl;
    dmemory[address] = (val & 0xff);
    dmemory[address + 1] = ((val >> 8) & 0xff);
    dmemory[address + 2] = ((val >> 16) & 0xff);
    dmemory[address + 3] = ((val >> 24) & 0xff);
}

bitset<32> CPU::Fetch(bitset<8> *instmem) {
    bitset<32> instr = ((((instmem[PC + 3].to_ulong()) << 24)) + ((instmem[PC + 2].to_ulong()) << 16) + ((instmem[PC + 1].to_ulong()) << 8) + (instmem[PC + 0].to_ulong()));  //get 32 bit instruction
    return instr;
}

bool CPU::Decode(instruction* curr)
{
    cerr << "DECODE" << endl;
    cerr << curr->instr << endl;

    int32_t instr_str = (curr->instr).to_ulong();
    int32_t opcode = instr_str & 0x7f;
    int32_t funct3 = (instr_str >> 12) & 0x7;
    rs1 = (instr_str >> 15) & 0x1f;

    // R-type
    if (opcode == 0b0110011) {
        cerr << "R-type" << endl;
        int32_t funct7 = (instr_str >> 25) & 0x7f;
        rs2 = (instr_str >> 20) & 0x1f;
        rd = (instr_str >> 7) & 0x1f;

        if (funct3 == 0x0) {
            // ADD
            if (funct7 == 0x0) {
                cur_instr = ADD;
                cerr << "ADD" << endl;
            }
            // SUB
            else if (funct7 == 0x20) {
                cur_instr = SUB;
                cerr << "SUB" << endl;
            }
        }
        // XOR
        else if (funct3 == 0x4) {
            cur_instr = XOR;
            cerr << "XOR" << endl;
        }
        // SRA
        else if (funct3 == 0x5) {
            cur_instr = SRA;
            cerr << "SRA" << endl;
        }
    }
    // I-type
    else if (opcode == 0b0010011) {
        cerr << "I-type" << endl;
        rd = (instr_str >> 7) & 0x1f;
        imm = instr_str >> 20;

        // sign extension
        if (((instr_str >> 31) & 0x1) == 0x1) {
            imm |= 0xfffff000;
        }

        // ADDI
        if (funct3 == 0x0) {
            cur_instr = ADDI;
            cerr << "ADDI" << endl;
        }
        // ANDI
        else if (funct3 == 0x7) {
            cur_instr = ANDI;
            cerr << "ANDI" << endl;
        }
    }
    // I-type
    else if (opcode == 0b0000011) {
        // LW
        rd = (instr_str >> 7) & 0x1f;
        imm = instr_str >> 20;

        // sign extension
        if (((instr_str >> 31) & 0x1) == 0x1) {
            imm |= 0xfffff000;
        }

        cur_instr = LW;
        cerr << "LW" << endl;
    }
    // I-type
    else if (opcode == 0b1100111) {
        // JALR
        rd = (instr_str >> 7) & 0x1f;
        imm = instr_str >> 20;
        
        // sign extension
        if (((instr_str >> 31) & 0x1) == 0x1) {
            imm |= 0xfffff000;
        }
        
        cur_instr = JALR;
        cerr << "JALR" << endl;
    }
    // S-type
    else if (opcode == 0b0100011) {
        // SW
        rs2 = (instr_str >> 20) & 0x1f;
        imm = ((instr_str >> 25) << 5) | ((instr_str >> 7) & 0x1f);
        
        // sign extension
        if (((instr_str >> 31) & 0x1) == 0x1) {
            imm |= 0xfffff000;
        }
        
        cur_instr = SW;
        cerr << "SW" << endl;
    }
    // B-type
    else if (opcode == 0b1100011) {
        // BLT
        rs2 = (instr_str >> 20) & 0x1f;
        imm = ((instr_str >> 31) << 11) | (((instr_str >> 7) & 0x1) << 10) | (((instr_str >> 25) & 0x3f) << 4) | ((instr_str >> 8) & 0xf);
        imm <<= 1;
        
        // sign extension
        if (((instr_str >> 31) & 0x1) == 0x1) {
            imm |= 0xffffe000;
        }
        
        cur_instr = BLT;
        cerr << "BLT" << endl;
    }
    // ZERO
    else {
        cur_instr = ZERO;
        cerr << "ZERO" << endl;
        return false;
    }

    return true;
}

int32_t CPU::Execute(Op cur_instr, int32_t rs1, int32_t rs2, int32_t imm)
{
    cerr << "EXECUTE" << endl;

    int32_t op1 = readReg(rs1);
    int32_t op2;

    cerr << "Select between RS2 and immediate" << endl;
    // Select between RS2 and immediate
    switch (cur_instr) {
        case ADD:
        case SUB:
        case XOR:
        case SRA:
        case BLT:
            op2 = readReg(rs2);
            break;
//        case SW:
//            op1 = imm;
//            op2 = readReg(rs2);
//            break;
        case SW:
        case ADDI:
        case ANDI:
        case LW:
        case JALR:
            op2 = imm;
            break;
        default:
            return 0;
    }

    cerr << "ALU" << endl;
    cerr << "op1: " << op1 << endl;
    cerr << "op2: " << op2 << endl;

    switch (cur_instr) {
        case ADD:
        case ADDI:
        case LW:
        case SW:
        case JALR:
            return op1 + op2;
        case SUB:
        case BLT:
            return op1 - op2;
        case XOR:
            return op1 ^ op2;
        case ANDI:
            return op1 & op2;
        case SRA:
            return op1 >> op2;
        default:
            return 0;
    }
}

void CPU::WriteBack(int32_t val) {
    if (rd != 0x0) {
        regfile[rd] = val;
        cerr << "rd: " << regfile[rd] << endl;
    }
}
