#include "CLK.h"
#include "RAM.h"
#include "ASM.h"

byte prog[256] = {
  DATA(R0, 0),
  JMPR(R0),
} ;


void setup() {
  Serial.begin(9600) ;
  Serial.println("RAM unit booting...") ;
  setup_CLK() ;
  setup_RAM(prog) ;
}


void loop() {
  loop_CLK() ;
  loop_RAM() ;
}
