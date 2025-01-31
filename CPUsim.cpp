#include "CPU.h"

#include <iostream>
#include <bitset>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

/*
Add all the required standard and developed libraries here
*/

/*
Put/Define any helper function/definitions you need here
*/
int main(int argc, char* argv[])
{
    /* This is the front end of your project.
    You need to first read the instructions that are stored in a file and load them into an instruction memory.
    */

    /* Each cell should store 1 byte. You can define the memory either dynamically, or define it as a fixed size with size 4KB (i.e., 4096 lines). Each instruction is 32 bits (i.e., 4 lines, saved in little-endian mode).
    Each line in the input file is stored as an unsigned char and is 1 byte (each four lines are one instruction). You need to read the file line by line and store it into the memory. You may need a mechanism to convert these values to bits so that you can read opcodes, operands, etc.
    */

    bitset<8> instMem[4096];


    if (argc < 2) {
        //cout << "No file name entered. Exiting...";
        return -1;
    }

    ifstream infile(argv[1]); //open the file
    if (!(infile.is_open() && infile.good())) {
        cout<<"error opening file\n";
        return 0;
    }
    string line;
    int i = 0;
    while (infile) {
            infile>>line;
            stringstream line2(line);
            int x;
            line2>>x;
            instMem[i] = bitset<8>(x);
            i++;
        }
    int maxPC= i;

    /* Instantiate your CPU object here.  CPU class is the main class in this project that defines different components of the processor.
    CPU class also has different functions for each stage (e.g., fetching an instruction, decoding, etc.).
    */

    CPU myCPU;  // call the approriate constructor here to initialize the processor...
    // make sure to create a variable for PC and resets it to zero (e.g., unsigned int PC = 0);

    /* OPTIONAL: Instantiate your Instruction object here. */
    //Instruction myInst;
    bitset<32> curr;
    instruction instr = instruction(curr);
    bool done = true;
    while (done == true) // processor's main loop. Each iteration is equal to one clock cycle.
    {
        cerr << endl;
        for (int i = 0; i < 32; i++) {
            cerr << "x" << i << ": " << myCPU.readReg(i) << endl;
        }

        cerr << endl;
        cerr << endl;

        // fetch
        curr = myCPU.Fetch(instMem); // fetching the instruction
        instr = instruction(curr);

        // decode
        done = myCPU.Decode(&instr);
        if (done == false) // break from loop so stats are not mistakenly updated
            break;

        cerr << "clock: " << myCPU.readClock() << endl;
        cerr << "pc: " << myCPU.readPC() << endl;
        cerr << "instruction: " << myCPU.getInstr() << endl;
        cerr << "rs1: " << myCPU.getRS1() << endl;
        cerr << "rs2: " << myCPU.getRS2() << endl;
        cerr << "rd: " << myCPU.getRD() << endl;
        cerr << "imm: " << myCPU.getImm() << endl;

        // execute
        int32_t aluResult = myCPU.Execute(myCPU.getInstr(), myCPU.getRS1(), myCPU.getRS2(), myCPU.getImm());

        cerr << "alu result: " << aluResult << endl;

        // BLT
        if (myCPU.getInstr() == CPU::BLT) {
            if (aluResult < 0) {
                myCPU.UpdatePC(myCPU.readPC() + myCPU.getImm());
            }
            else {
                myCPU.UpdatePC(myCPU.readPC() + 4);
            }
            myCPU.UpdateClock();
            continue;
        }

        // JALR
        if (myCPU.getInstr() == CPU::JALR) {
            myCPU.WriteBack(myCPU.readPC() + 4);
            myCPU.UpdatePC(aluResult);
            myCPU.UpdateClock();
            continue;
        }

        // LW
        if (myCPU.getInstr() == CPU::LW) {
            int32_t val = myCPU.getMem(aluResult);
            myCPU.WriteBack(val);
        }
        // SW
        else if (myCPU.getInstr() == CPU::SW) {
            myCPU.setMem(aluResult, myCPU.readReg(myCPU.getRS2()));
            cerr << "MEMORY VALUE: " << myCPU.getMem(aluResult) << endl;
        }
        // ADD, SUB, ADDI, XOR, ANDI, SRA
        else {
            myCPU.WriteBack(aluResult);
        }

        myCPU.UpdatePC(myCPU.readPC() + 4);
        myCPU.UpdateClock();

        // sanity check
        if (myCPU.readPC() > maxPC)
            break;
    }

    cerr << endl;
    cerr << endl;
    cerr << "END" << endl;
    cerr << endl;
    for (int i = 0; i < 32; i++) {
        cerr << myCPU.readReg(i) << endl;
    }
    cerr << endl;

    int32_t a0 = 0b01010;
    int32_t a1 = 0b01011;
    int val0 = myCPU.readReg(a0);
    int val1 = myCPU.readReg(a1);
    
    // print the results (you should replace a0 and a1 with your own variables that point to a0 and a1)
    cout << "(" << val0 << "," << val1 << ")" << endl;
    
    return 0;

}
