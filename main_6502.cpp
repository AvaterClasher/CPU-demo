#include <stdio.h>
#include <stdlib.h>

using namespace std;

using Byte = unsigned char; 
using Word = unsigned short;

using u32 = unsigned int;
using u32 = unsigned short;

struct Mem // 6502 Memory
{
    static constexpr u32 MAX_MEM = 1024 * 64; // Initialise the memory with 64KB
    Byte Data[MAX_MEM]; // 64KB of memory

    void Initialise() // Initialise the memory with zero
    {
        for (u32 i = 0; i < MAX_MEM; i++)
        {
            Data[i] = 0;
        }
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
        PC = 0xFFFC; // Program Counter initialised With zero
        SP = 0x0100; // Stack Pointer initialised With zero
        D = 0; // Decimal Mode initialised With zero
        A = X = Y = 0; // All Registers initialised With zero
        C = Z = I = D = B = V = N = 0; // All Flags initialised With zero
        memory.Initialise();
    }

    void Execute(u32 Ticks, Mem &memory) // Execute the instructions in the memory
    {

    }
};

int main()
{
    Mem mem; // Initialise the memory
    CPU cpu; // Initialise the CPU
    cpu.Reset(mem); // Reset the CPU
    cpu.Execute(2, mem); // Execute the instructions in the memory
    return 0;
}