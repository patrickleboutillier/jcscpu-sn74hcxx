#ifndef ASM_H
#define ASM_H


typedef enum Reg { R0 = 0, R1 = 1, R2 = 2, R3 = 3 } ;


byte _ADD(Reg ra, Reg rb) ;
byte _SHL(Reg ra, Reg rb) ;
byte _SHR(Reg ra, Reg rb) ;
byte _NOT(Reg ra, Reg rb) ;
byte _AND(Reg ra, Reg rb) ;
byte _OR(Reg ra, Reg rb) ;
byte _XOR(Reg ra, Reg rb) ;
byte _CMP(Reg ra, Reg rb) ;
byte _LD(Reg ra, Reg rb) ;
byte _ST(Reg ra, Reg rb) ;
byte _DATA(Reg rb) ;
byte _JMPR(Reg rb) ;
byte _JMP() ;
byte _JMPIF(byte flags) ;
byte _IO(byte mode, Reg rb) ;


#define C   B1000
#define A   B0100
#define E   B0010
#define Z   B0001

#define IN_DATA   B0000
#define IN_ADDR   B0100
#define OUT_DATA  B1000
#define OUT_ADDR  B1100    

// ALU instructions
#define ADD(ra, rb)       _ADD(ra, rb)
#define SHL(ra, rb)       _SHL(ra, rb)
#define SHR(ra, rb)       _SHR(ra, rb)
#define NOT(ra, rb)       _NOT(ra, rb)
#define AND(ra, rb)       _AND(ra, rb)
#define OR(ra, rb)        _OR(ra, rb)
#define XOR(ra, rb)       _XOR(ra, rb)
#define CMP(ra, rb)       _CMP(ra, rb)

// Load RB from RAM address in RA
#define LD(ra, rb)        _LD(ra, rb)
// Store RB to RAM address in RA
#define ST(ra, rb)        _ST(ra, rb)
#define DATA(rb, data)    _DATA(rb), data

#define JMPR(rb)          _JMPR(rb)
#define JMP(addr)         _JMP(), addr

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

#define CLF               B01100000
#define HALT              B01100001

#define IND(rb)          _IO(IN_DATA, rb)
#define INA(rb)          _IO(IN_ADDR, rb)
#define OUTD(rb)         _IO(OUT_DATA, rb)
#define OUTA(rb)         _IO(OUT_ADDR, rb)

#endif
