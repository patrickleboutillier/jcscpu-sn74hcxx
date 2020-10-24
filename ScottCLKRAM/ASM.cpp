#include "Arduino.h"
#include "ASM.h"


byte _ADD(Reg ra, Reg rb){
  return B10000000 | (ra << 2) | rb ;
}


byte _SHL(Reg ra, Reg rb){
  return B10010000 | (ra << 2) | rb ;
}


byte _SHR(Reg ra, Reg rb){
  return B10100000 | (ra << 2) | rb ;
}


byte _NOT(Reg ra, Reg rb){
  return B10110000 | (ra << 2) | rb ;
}


byte _AND(Reg ra, Reg rb){
  return B11000000 | (ra << 2) | rb ;
}


byte _OR(Reg ra, Reg rb){
  return B11010000 | (ra << 2) | rb ;
}


byte _XOR(Reg ra, Reg rb){
  return B11100000 | (ra << 2) | rb ;
}


byte _CMP(Reg ra, Reg rb){
  return B11110000 | (ra << 2) | rb ;
}


byte _LD(Reg ra, Reg rb){
  return B00000000 | (ra << 2) | rb ;
}


byte _ST(Reg ra, Reg rb){
  return B00010000 | (ra << 2) | rb ;
}


byte _DATA(Reg rb){
  return B00100000 | rb ;
}


byte _JMPR(Reg rb){
  return B00110000 | rb ;
}


byte _JMP(){
  return B01000000 ;
}


byte _JMPIF(byte flags){
  return B01010000 | flags ;
}
