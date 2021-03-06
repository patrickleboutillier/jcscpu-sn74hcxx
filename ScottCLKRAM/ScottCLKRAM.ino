#include "CLK.h"
#include "RAM.h"
#include "PROGS.h"
#include "gentest.h"


#define HALT_PIN    A3
#define RESET_PIN    2


bool halted = 0 ;
Program* cur_prog = NULL ;
byte slowdown = 1 ;
bool display = false ;


Program *test(){
  static int i = 0 ;
  static byte insts[16] ;
  static Program p("test", insts, NULL) ;
  if (p.getSize() == 0){
    p.CLF() ;
    p.DATA(R0, 127) ;
    p.DATA(R1, 129) ;
    p.ADD(R0, R1) ;
    p.HALT() ;
  }

  Program *ret = (!i ? &p : NULL) ;
  i++ ;
  return ret ;
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


byte RAM_sim[256] ;
Program* testProgGen() {
  static long n = 0 ;
  static char name[33] ;
  static byte insts[128] ;
  static Program p(name, insts, [](byte *RAM) -> bool { return compare_RAMS(RAM_sim, RAM) ; }) ;

  sprintf(name, "test-%ld", n) ;
  p.reset() ;
  for (int i = 0 ; i < 128 ; i++){
    insts[i] = 0 ;
  }
  for (int i = 0 ; i < 256 ; i++){
    RAM_sim[i] = 0 ;
  }
     
  if ((n % 100) == 0){
    display = !display ;
  }
  else {
    display = false ;
  }

  if (display){
    // Generate the display program 
    p.reset() ;
    p.DATA(R0, 0) ;
    p.OUTA(R0) ;
    p.DATA(R0, 253) ; 
    p.OUTD(R0) ;
    p.DATA(R0, 255) ; 
    p.OUTD(R0) ;
    for (byte i = 0 ; i < strlen(name) ; i++){
      p.DATA(R0, name[i]) ; 
      p.OUTD(R0) ;
    }
    p.DATA(R0, 254) ; 
    p.OUTD(R0) ;
    p.HALT() ;
    
    slowdown = 2 ;    
    sprintf(name, "display-%ld", n) ;

    return &p ;
  }
  
  // Generate the new program
  gen_test_prog(RAM_sim, &p) ;  
  n++ ;
  slowdown = 1 ;
  
  return &p ;
}



Program* (*prog)() = testProgGen ;
unsigned long start = 0 ;

void run_program(Program *prog, bool reset){
    Serial.print("Program '") ;
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

    start = millis() ;
}


void setup() {
  Serial.begin(9600) ;
  randomSeed(analogRead(A6)) ;
  Serial.println("RAM unit booting...") ;
  
  pinMode(RESET_PIN, OUTPUT) ; 
  
  setup_CLK() ;
  setup_RAM() ;
  run_program((*prog)(), true) ; 
}


void loop() {
  if (halted){
    return ;
  }
  
  loop_CLK(slowdown) ;
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
  bool ok = 1 ;

  if (! display){
    if (! cur_prog->runHaltTest(get_RAM())){
     ok = 0 ;
     halted = 1 ;
     return ;
    }

    Serial.print("Program '") ;
    Serial.print(cur_prog->getName()) ;
    Serial.print("' halted with ") ;
    Serial.print(ok ? "SUCCESS." : "FAILURE!!!") ;
    Serial.print(" after ") ;
    Serial.print(millis() - start) ;
    Serial.println(" ms.") ;  
  }
    
  //Serial.println("Sleeping 3 seconds...") ;
  //delay(3000) ;
  Program *next_prog = (*prog)() ;
  if (next_prog != NULL){
    run_program(next_prog, true) ;
  }
  else {
    Serial.println(F("HALTING (No more programs to run!)")) ;
    halted = 1 ;
  }
}
