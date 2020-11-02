/*
*/
#ifndef Program_h
#define Program_h

#include "Arduino.h"


typedef enum Reg { R0 = 0, R1 = 1, R2 = 2, R3 = 3 } ;


class Program {
  public:
    Program(char *name, byte *backing, bool (*halt_test)(byte *RAM)) ;
    char *getName() ;
    byte getSize() ;
    byte* toByteArray() ;
    void push_back(byte b) ;
    bool runHaltTest(byte *RAM) ;
    void reset() ;
    
    void ADD(Reg ra, Reg rb) ;
    void SHL(Reg ra, Reg rb) ;
    void SHR(Reg ra, Reg rb) ;
    void NOT(Reg ra, Reg rb) ;
    void AND(Reg ra, Reg rb) ;
    void OR(Reg ra, Reg rb) ;
    void XOR(Reg ra, Reg rb) ;
    void CMP(Reg ra, Reg rb) ;
    void LD(Reg ra, Reg rb) ;
    void ST(Reg ra, Reg rb) ;
    void DATA(Reg rb, byte data) ;
    void JMPR(Reg rb) ;
    void JMP(byte addr) ;
    void JC(byte addr) ;
    void JA(byte addr) ;
    void JE(byte addr) ;
    void JZ(byte addr) ;
    void CLF() ;
    void HALT() ;
    void IND(Reg rb) ;
    void INA(Reg rb) ;
    void OUTD(Reg rb) ;
    void OUTA(Reg rb) ;        
  private:
    char *_name ;
    byte _idx ;
    byte *_backing ; 
    bool (*_halt_test)(byte *RAM) ;
} ;

#endif

/*
#define JC(addr)          _JMPIF(C), addr
#define JA(addr)          _JMPIF(A), addr
#define JE(addr)          _JMPIF(E), addr
#define JZ(addr)          _JMPIF(Z), addr
#define JCA(addr)         _JMPIF(C|A), addr
#define JCE(addr)         _JMPIF(C|E), addr
#define JCZ(addr)         _JMPIF(C|Z), addr
#define JAE(addr)         _JMPIF(A|E), addr
#define JAZ(addr)         _JMPIF(A|Z), addr
#define JEZ(addr)         _JMPIF(E|Z), addr
#define JCAE(addr)        _JMPIF(C|A|E), addr
#define JCAZ(addr)        _JMPIF(C|A|Z), addr
#define JCEZ(addr)        _JMPIF(C|E|Z), addr
#define JAEZ(addr)        _JMPIF(A|E|Z), addr
#define JCAEZ(addr)       _JMPIF(C|A|E|Z), addr
*/
