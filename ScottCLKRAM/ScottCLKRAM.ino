#include "CLK.h"
#include "RAM.h"
#include "ASM.h"
#include "PROGS.h"


#define HALT_PIN    A3

bool halted = 0 ;
byte nb_progs = 0 ;
byte cur_prog = 0 ;


byte prog[] = {
  DATA(R0, B10101010),
  XOR(R0, R0),
  CLF,
  JCAEZ(11),
  DATA(R0, 20),
  DATA(R1, 100),
  ST(R1, R0),
  HALT,
} ;


Prog progs[] = {
  // PROG("test CLF", prog, []() -> bool { return (get_RAM(100) == 20) ; }),
  prog42,
  prog5x5,
  prog10print,  
} ;


void run_prog(byte n){
    Serial.print("Program #") ;
    Serial.print(n) ;
    Serial.print(": '") ;
    Serial.print(progs[n].name) ;
    Serial.print("' (size=") ;
    Serial.print(progs[n].len) ;
    Serial.println(" bytes)") ;

    reset_CLK() ;
    reset_RAM(progs[n].insts, progs[n].len) ;
}


void setup() {
  Serial.begin(9600) ;
  Serial.println("RAM unit booting...") ;
  nb_progs = sizeof(progs) / sizeof(progs[0]) ;
  Serial.print(nb_progs) ;
  Serial.println(" programs in sequence") ;

  setup_CLK() ;
  setup_RAM() ;
  run_prog(cur_prog) ; 
}


void loop() {
  if (halted){
    return ;
  }
  
  loop_CLK() ;
  loop_RAM() ;

  long qtick = get_qtick() ;
  if (qtick >= 24){
    // Only HALT starting on the second instruction, since on a reset the IR may still be spitting out the previous HALT
    // instruction...
    if (digitalRead(HALT_PIN) == HIGH){
      halt() ;
    }
  }
}


void halt(){
  Serial.print("Program '") ;
  Serial.print(progs[cur_prog].name) ;
  Serial.print("' halted with ") ;
  if (! (*progs[cur_prog].halt)()){
    Serial.println("FAILURE!!!") ;
  }
  else {
    Serial.println("SUCCESS.") ;     
  }
  
  cur_prog++ ;
  if (cur_prog < nb_progs){
    run_prog(cur_prog) ;
  }
  else {
    Serial.println(F("HALTING (No more programs to run!)")) ;
    halted = 1 ;
  }
}
