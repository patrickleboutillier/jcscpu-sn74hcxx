#include "CLK.h"
#include "RAM.h"
#include "PROGS.h"


#define HALT_PIN    A3
#define RESET_PIN    2


bool halted = 0 ;
Program* cur_prog = NULL ;


Program *test(){
  static byte insts[16] ;
  static Program p("test", insts, NULL) ;
  if (p.getSize() == 0){
    p.DATA(R0, B10101010) ;
    p.XOR(R0, R0) ;
    p.CLF() ;
    //p.JCAEZ(11) ;
    p.DATA(R0, 20) ;
    p.DATA(R1, 100) ;
    p.ST(R1, R0) ;
    p.HALT() ;
  }
  
  return &p ;
}


Program* progGen() {
  static int i = 0 ;
  static Program* progs[] = {
    prog42(),
    prog5x5(),
    prog10print(),
  } ;

  int n = sizeof(progs) / sizeof(progs[0]) ;
  Program *ret = (i < n ? progs[i] : NULL) ;
  i++ ;

  return ret ;
}


void run_program(Program *prog, bool reset){
    Serial.print("Program: '") ;
    Serial.print(prog->getName()) ;
    Serial.print("' (size=") ;
    Serial.print(prog->getSize()) ;
    Serial.println(" bytes)") ;

    cur_prog = prog ;
    reset_CLK() ;
    reset_RAM(prog->toByteArray(), prog->getSize()) ;

    if (reset){
      digitalWrite(RESET_PIN, HIGH) ;
      delay(100) ;
      digitalWrite(RESET_PIN, LOW) ;
    }
}


void setup() {
  Serial.begin(9600) ;
  Serial.println("RAM unit booting...") ;
  
  pinMode(RESET_PIN, OUTPUT) ; 
  
  setup_CLK() ;
  setup_RAM() ;
  run_program(progGen(), true) ; 
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
  Serial.print(cur_prog->getName()) ;
  Serial.print("' halted with ") ;
  if (! cur_prog->runHaltTest(get_RAM())){
    Serial.println("FAILURE!!!") ;
  }
  else {
    Serial.println("SUCCESS.") ;     
  }

  Serial.println("Sleeping 3 seconds...") ;
  delay(3000) ;
  Program *next_prog = progGen() ;
  if (next_prog != NULL){
    run_program(next_prog, true) ;
  }
  else {
    Serial.println(F("HALTING (No more programs to run!)")) ;
    halted = 1 ;
  }
}
