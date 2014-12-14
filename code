#include <stdio.h>
int _C = 0;
unsigned long long R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14 = 0;
unsigned long long R15 = 3;
unsigned long long MEM[1000];

int f() {
_C = MEM[ R14 + 0 ] - 0;
if (_C == 0) { R0 = 1; }
if (_C == 0) { int __obp = MEM[R15 - 1];
R15 = R14;
R14 = __obp;
return 0; }
R0 = MEM[ R14 + 0 ] - 1;
MEM[ R15++ ] = R0;
MEM[ R15++ ] = R14;
R14 = R15 - 1 - 1;
f();
R0 = MEM[ R14 + 0 ] * R0;
int __obp = MEM[R15 - 1];
R15 = R14;
R14 = __obp;
return 0;
}

int func() {
R0 = MEM[ R14 + 0 ] + MEM[ R14 + 1 ];
int __obp = MEM[R15 - 1];
R15 = R14;
R14 = __obp;
return 0;
}

int main(int argc, char* argv[]) {
MEM[ R15++ ] = (unsigned long long)"%d %d %d\n";
MEM[ R15++ ] = 5;
MEM[ R15++ ] = 6;
MEM[ R15++ ] = 7;
R15 = R15 - 4;
printf(MEM[ R15 + 0 ], MEM[ R15 + 1 ], MEM[ R15 + 2 ], MEM[ R15 + 3 ]);
MEM[ R15++ ] = 3;
MEM[ R15++ ] = 4;
MEM[ R15++ ] = R14;
R14 = R15 - 2 - 1;
func();
MEM[ R15++ ] = (unsigned long long)"%d\n";
MEM[ R15++ ] = R0;
R15 = R15 - 2;
printf(MEM[ R15 + 0 ], MEM[ R15 + 1 ]);
MEM[ R15++ ] = 4;
MEM[ R15++ ] = R14;
R14 = R15 - 1 - 1;
f();
MEM[ R15++ ] = (unsigned long long)"%d\n";
MEM[ R15++ ] = R0;
R15 = R15 - 2;
printf(MEM[ R15 + 0 ], MEM[ R15 + 1 ]);
int __obp = MEM[R15 - 1];
R15 = R14;
R14 = __obp;
return 0;
}

