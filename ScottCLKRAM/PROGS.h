#include "ASM.h"


#define PROG(n, p, f)  {n, p, sizeof(p), f}


typedef struct Prog {
  char *name ;
  byte *insts ;
  byte len ; 
  byte (*halt)() ;
} ;


byte insts42[] = {
  DATA(R0, 20),
  DATA(R1, 22),
  ADD(R0, R1),
  DATA(R0, 100),
  ST(R0, R1),
  HALT,
  /*
00100000 # DATA  R0, 00000000 (0)
00000000 # ...   0
01111100 # OUTA  R0
01111001 # OUTD  R1
01100001 # HALT   
  */
} ;
Prog prog42 = PROG("42", insts42, []() -> bool { return (get_RAM(100) == 42) ; }) ;
