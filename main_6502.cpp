#include <stdio.h>
#include <stdlib.h>

using namespace std;

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;

// using u32 = unsigned short;

struct Mem // 6502 Memory
{
    static constexpr u32 MAX_MEM = 1024 * 64; // Initialise the memory with 64KB
    Byte Data[MAX_MEM];                       // 64KB of memory

    void Initialise() // Initialise the memory with zero
    {
        for (u32 i = 0; i < MAX_MEM; i++)
        {
            Data[i] = 0;
        }
    }

    // Read 1 Byte

    Byte &operator[](u32 Address) /*const*/
    {
        // Assert here Address is < MAX_MEM
        return Data[Address];
    }

    void WriteWord(Word Value, u32 Address, u32 &Cycles) // Write 2 Bytes
    {
        Data[Address] = Value & 0xFF;     // Least Significant Byte
        Data[Address + 1] = (Value >> 8); // Most Significant Byte
        Cycles -= 2;                      // Decrement the Cycles
    }
};

struct CPU // 6502 CPU
{

    Word PC; // Program Counter
    Word SP; // Stack Pointer

    Byte A, X, Y; // Registers

    Byte C : 1; // Carry Flag
    Byte Z : 1; // Zero Flag
    Byte I : 1; // Interrupt Disable
    Byte D : 1; // Decimal Mode
    Byte B : 1; // Break Command
    Byte V : 1; // Overflow Flag
    Byte N : 1; // Negative Flag

    void Reset(Mem &memory)
    {
        PC = 0xFFFC;                   // Program Counter initialised With zero
        SP = 0x0100;                   // Stack Pointer initialised With zero
        D = 0;                         // Decimal Mode initialised With zero
        A = X = Y = 0;                 // All Registers initialised With zero
        C = Z = I = D = B = V = N = 0; // All Flags initialised With zero
        memory.Initialise();
    }

    Byte FetchByte(u32 &Cycles, Mem &memory) // Fetch the instructions from the memory
    {
        Byte Data = memory[PC]; // Fetch the data from the memory
        PC++;                   // Increment the Program Counter
        Cycles--;               // Decrement the Cycles
        return Data;            // Return the data
    }

    Word FetchWord(u32 &Cycles, Mem &memory) // Fetch the instructions from the memory
    {
        // 6502 is little endian so least significant byte first

        Word Data = memory[PC];    // Fetch the data from the memory
        PC++;                      // Increment the Program Counter
        Data |= (memory[PC] << 8); // Shift the data by 8 bits
        PC++;                      // Increment the Program Counter
        Cycles -= 2;               // Decrement the Cycles

        return Data; // Return the data
    }

    Byte ReadByte(u32 &Cycles, Byte Address, Mem &memory) // Read the instructions from the memory
    {
        Byte Data = memory[Address]; // Fetch the data from the memory
        Cycles--;                    // Decrement the Cycles
        return Data;                 // Return the data
    }

    static constexpr Byte
        INS_LDA_IM = 0xA9,  // Instruction for LDA Immediate
        INS_LDA_ZP = 0xA5,  // Instruction for LDA Zero Page
        INS_LDA_ZPX = 0xB5, // Instruction for LDA Zero Page X
        INS_JSR = 0x20;     // Instruction for JSR

    void LDASetStatus()
    {
        Z = (A == 0);             // Set the Zero Flag
        N = (A & 0b10000000) > 0; // Set the Negative Flag
    }

    void Execute(u32 Cycles, Mem &memory) // Execute the instructions in the memory
    {
        while (Cycles > 0)
        {
            Byte Ins = FetchByte(Cycles, memory); // Fetch the instructions from the memory

            switch (Ins) // Switch case for the instructions
            {

            case INS_LDA_IM: // Instruction for LDA Immediate
            {
                Byte Value = FetchByte(Cycles, memory); // Fetch the value from the memory
                A = Value;                              // Load the value into the register A
                LDASetStatus();
            }
            break;

            case INS_LDA_ZP: // Instruction for LDA Zero Page
            {
                Byte ZeroPageAddress = FetchByte(Cycles, memory); // Fetch the address from the memory
                A = ReadByte(Cycles, ZeroPageAddress, memory);    // Load the value into the register A
                LDASetStatus();
            }
            break;

            case INS_LDA_ZPX: // Instruction for LDA Zero Page X
            {
                Byte ZeroPageAddress = FetchByte(Cycles, memory); // Fetch the address from the memory
                ZeroPageAddress += X;                             // Add the value of X to the address
                Cycles--;                                         // Decrement the Cycles
                A = ReadByte(Cycles, ZeroPageAddress, memory);    // Load the value into the register A
                LDASetStatus();
            }
            break;

            case INS_JSR: // Instruction for JSR
            {
                Word SubAddress = FetchWord(Cycles, memory); // Fetch the address from the memory
                memory.WriteWord(PC - 1, SP, Cycles);        // Write the Program Counter to the Stack
                PC = SubAddress;                             // Set the Program Counter to the SubAddress
                Cycles--;                                    // Decrement the Cycles
            }
            break;

            default: // Instruction not handled
            {
                printf("Instruction not handled %d", Ins);
            }
            break;
            }
        }
    };
};

int main()
{
    Mem mem;        // Initialise the memory
    CPU cpu;        // Initialise the CPU
    cpu.Reset(mem); // Reset the CPU
    // Starting a inline program
    mem[0xFFFC] = CPU::INS_LDA_IM;
    mem[0xFFFD] = 0x42;
    // Ending the inline program
    cpu.Execute(2, mem); // Execute the instructions in the memory
    return 0;
}