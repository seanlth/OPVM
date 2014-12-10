#include <stdio.h>
int _C = 0;
unsigned long long R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15 = 0;
unsigned long long MEM[1000]; 

int fact() {
_C = MEM[ R15 + 0 ] - 0;
if (_C == 0) { R0 = 1; }
if (_C == 0) { return 0; }
if (_C != 0) { R1 = R1 - 1; }
fact();
return 0;
}

int main(int argc, char* argv[]) {
MEM[ R15 + 0 ] = 4;
fact();
return 0;
}

