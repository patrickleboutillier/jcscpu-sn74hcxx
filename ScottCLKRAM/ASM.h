typedef enum Reg { R0 = 0, R1 = 1, R2 = 2, R3 = 3 } ;


byte _ADD(Reg ra, Reg rb) ;
byte _LD(Reg ra, Reg rb) ;
byte _ST(Reg ra, Reg rb) ;
byte _DATA(Reg rb) ;
byte _JMPR(Reg rb) ;
byte _JMP() ;

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
