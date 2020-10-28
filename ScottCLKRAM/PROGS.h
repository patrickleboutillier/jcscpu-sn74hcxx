#include "Program.h"
#include "RAM.h" 


Program *prog42(){
  static byte insts[32] ;
  static Program p("42", insts, [](byte *RAM) -> bool { return (RAM[100] == 42) ; }) ;
  if (p.getSize() == 0){
    p.CLF() ;
    p.DATA(R0, 20) ;
    p.DATA(R1, 22) ;
    p.ADD(R0, R1) ;
    p.DATA(R0, 1) ;
    p.OUTA(R0) ;
    p.OUTD(R1) ;
    p.DATA(R0, 100) ;
    p.ST(R0, R1) ;
    p.HALT() ;
  }
  
  return &p ;
}


Program *prog5x5(){
  static byte insts[64] ;
  static Program p("5x5", insts, [](byte *RAM) -> bool { return (RAM[100] == 25) ; }) ;
  if (p.getSize() == 0){
    p.CLF() ;
    p.DATA(R0, 5) ;
    p.DATA(R1, 5) ;
    p.DATA(R3, 1) ;
    p.XOR(R2, R2) ; 
    p.CLF() ;
    p.SHR(R0, R0) ;
    p.JC(14) ;
    p.JMP(16) ;
    p.CLF() ;
    p.ADD(R1, R2) ;
    p.CLF() ;
    p.SHL(R1, R1) ;
    p.SHL(R3, R3) ;
    p.JC(23) ;
    p.JMP(8) ;
    p.DATA(R3, 100) ;
    p.ST(R3, R2); 
    p.DATA(R0, 1) ;
    p.OUTA(R0) ;
    p.OUTD(R2) ; 
    p.HALT() ;
  }

  return &p ;
} 


Program *prog10print(){
  static byte insts[32] ;
  static Program p("10print", insts, NULL) ;
  if (p.getSize() == 0){
    p.DATA(R0, 2) ;
    p.OUTA(R0) ;
    p.IND(R0) ;
    p.CLF() ;
    p.SHR(R0, R0) ;
    p.JC(12) ;
    p.DATA(R1, 47) ;
    p.JMP(14) ;
    p.DATA(R1, 92) ;
    p.DATA(R0, 0) ;
    p.OUTA(R0) ;
    p.OUTD(R1) ;
    p.JMP(0) ;
    p.HALT() ;
  }

  return &p ;
} ;
