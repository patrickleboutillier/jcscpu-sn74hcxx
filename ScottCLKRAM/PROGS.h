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
  DATA(R0, 1),
  OUTA(R0),
  OUTD(R1),
  DATA(R0, 100),
  ST(R0, R1),
  HALT,
} ;
Prog prog42 = PROG("42", insts42, []() -> bool { return (get_RAM(100) == 42) ; }) ;

byte insts5x5[] = {
  DATA(R0, 5),
  DATA(R1, 5),
  DATA(R3, 1),
  XOR(R2, R2),
  CLF,
  SHR(R0, R0),
  JC(13),
  JMP(15),
  CLF,
  ADD(R1, R2),
  CLF,
  SHL(R1, R1),
  SHL(R3, R3),
  JC(22),
  JMP(7),
  DATA(R3, 100),
  ST(R3, R2),
  DATA(R0, 1),
  OUTA(R0),
  OUTD(R2),
  HALT
} ;
Prog prog5x5 = PROG("5x5", insts5x5, []() -> bool { return (get_RAM(100) == 25) ; }) ;

byte insts10print[] = {
  DATA(R0, 2),
  OUTA(R0),
  IND(R0),
  CLF,
  SHR(R0, R0),
  JC(12),
  DATA(R1, 47),
  JMP(14),
  DATA(R1, 92),
  DATA(R0, 0),
  OUTA(R0),
  OUTD(R1),
  JMP(0),
  HALT
} ;
Prog prog10print = PROG("10print", insts10print, []() -> bool { return 1 ; }) ;
