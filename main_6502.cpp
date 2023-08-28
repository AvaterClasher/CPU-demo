#include <stdio.h>
#include <stdlib.h>

using namespace std;

using Byte = unsigned char;
using Word = unsigned short;


struct Mem{
    
}

struct CPU
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

    void Reset(){
        PC = 0xFFFC;
        SP = 0x0100;
        D = 0;
        A = X = Y = 0;
        C = Z = I = D = B = V = N = 0;
    }
};

int main()
{
    CPU cpu;
    cpu.Reset();
    return 0;
}