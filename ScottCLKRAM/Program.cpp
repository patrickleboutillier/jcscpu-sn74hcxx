#include "Arduino.h"
#include "Program.h"


#define C   B1000
#define A   B0100
#define E   B0010
#define Z   B0001


Program::Program(char *name, byte *backing, bool (*halt_test)(byte *RAM)){
  // Here we will assume that the name passed is a global constant string.
  _name = name ;
  _backing = backing ;
  _idx = 0 ;

  _halt_test = halt_test ;
}



char *Program::getName(){
  return _name ;
}


byte Program::getSize(){
  return _idx ;
}


byte* Program::toByteArray(){
  return _backing ;
}


bool Program::runHaltTest(byte *RAM){
  if (_halt_test != NULL){
    return (*_halt_test)(RAM) ;
  }

  return true ;
}


void Program::reset(){
  _idx = 0 ;
}


void Program::push_back(byte b){
  _backing[_idx] = b ;
  _idx++ ;
}

void Program::ADD(Reg ra, Reg rb){
  push_back(B10000000 | (ra << 2) | rb) ;
}


void Program::SHR(Reg ra, Reg rb){
  push_back(B10010000 | (ra << 2) | rb) ;
}


void Program::SHL(Reg ra, Reg rb){
  push_back(B10100000 | (ra << 2) | rb) ;
}


void Program::NOT(Reg ra, Reg rb){
  push_back(B10110000 | (ra << 2) | rb) ;
}


void Program::AND(Reg ra, Reg rb){
  push_back(B11000000 | (ra << 2) | rb) ;
}


void Program::OR(Reg ra, Reg rb){
  push_back(B11010000 | (ra << 2) | rb) ;
}


void Program::XOR(Reg ra, Reg rb){
  push_back(B11100000 | (ra << 2) | rb) ;
}


void Program::CMP(Reg ra, Reg rb){
  push_back(B11110000 | (ra << 2) | rb) ;
}


void Program::LD(Reg ra, Reg rb){
  push_back(B00000000 | (ra << 2) | rb) ;
}


void Program::ST(Reg ra, Reg rb){
  push_back(B00010000 | (ra << 2) | rb) ;
}


void Program::DATA(Reg rb, byte data){
  push_back(B00100000 | rb) ;
  push_back(data) ;
}


void Program::JMPR(Reg rb){
  push_back(B00110000 | rb) ;
}


void Program::JMP(byte addr){
  push_back(B01000000) ;
  push_back(addr) ;
}


void Program::JC(byte addr){
  push_back(B01010000 | C) ;
  push_back(addr) ;
}

void Program::JA(byte addr){
  push_back(B01010000 | A) ;
  push_back(addr) ;
}

void Program::JE(byte addr){
  push_back(B01010000 | E) ;
  push_back(addr) ;
}

void Program::JZ(byte addr){
  push_back(B01010000 | Z) ;
  push_back(addr) ;
}


void Program::CLF(){
  push_back(B01100000) ;
}


void Program::HALT(){
  push_back(B01100001) ;
}


void Program::IND(Reg rb){
  push_back(B01110000 | B0000 | rb) ;
}


void Program::INA(Reg rb){
  push_back(B01110000 | B0100 | rb) ;
}


void Program::OUTD(Reg rb){
  push_back(B01110000 | B1000 | rb) ;
}


void Program::OUTA(Reg rb){
  push_back(B01110000 | B1100 | rb) ;
}
