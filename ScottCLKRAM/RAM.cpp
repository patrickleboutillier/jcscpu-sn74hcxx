#include "Arduino.h"
#include "RAM.h"


byte MAR ;
byte RAM[256] ;
bool RAM_set[256] ;

byte prev_RAM_e ;
byte prev_RAM_s ;
byte prev_MAR_s ;



void debug() ;


void reset_RAM(byte prog[], byte prog_size){
  prev_RAM_e = 1 ;
  prev_RAM_s = 0 ;
  prev_MAR_s = 0 ;

  MAR = 0 ;
  for (int i = 0 ; i < 256 ; i++){
    RAM[i] = prog[i] ;
    RAM_set[i] = 0 ;
  }
  
  digitalWrite(CLOCK_OUT, LOW) ;
  digitalWrite(CLOCK_IN, LOW) ;
  digitalWrite(DATA_OUT, LOW) ;
  digitalWrite(LATCH_IN, LOW) ;
  digitalWrite(LATCH_OUT, HIGH) ;

  digitalWrite(RESET, HIGH) ;
  digitalWrite(RESET, LOW) ;
}


void setup_RAM(){
  pinMode(RAM_e, INPUT) ;
  pinMode(RAM_s, INPUT) ;
  pinMode(MAR_s, INPUT) ;
  pinMode(CLOCK_OUT, OUTPUT) ;
  pinMode(CLOCK_IN, OUTPUT) ;
  pinMode(DATA_OUT, OUTPUT) ;
  pinMode(DATA_IN, INPUT) ;
  pinMode(LATCH_OUT, OUTPUT) ;
  pinMode(LATCH_IN, OUTPUT) ;
  pinMode(RESET, OUTPUT) ;  
}


void loop_RAM() {
  byte cur_RAM_e = digitalRead(RAM_e) ;
  if (cur_RAM_e != prev_RAM_e){
    if (cur_RAM_e){
      //Serial.println("Disable RAM") ;
    }
    else {
      //Serial.print("Enable RAM: ") ;
      digitalWrite(LATCH_OUT, LOW) ;
      shiftOut(DATA_OUT, CLOCK_OUT, MSBFIRST, RAM[MAR]) ;
      digitalWrite(LATCH_OUT, HIGH) ; 
      //Serial.println(RAM[MAR]) ;
      //debug() ; 
    }
    prev_RAM_e = cur_RAM_e ;
  }

  byte cur_RAM_s = digitalRead(RAM_s) ;
  if (cur_RAM_s != prev_RAM_s){
    if (cur_RAM_s){
      //Serial.print("Set RAM: ") ;
      digitalWrite(CLOCK_IN, HIGH) ;
      digitalWrite(LATCH_IN, HIGH) ;
      RAM[MAR] = shiftIn(DATA_IN, CLOCK_IN, MSBFIRST) ;
      RAM_set[MAR] = 1 ;
      digitalWrite(LATCH_IN, LOW) ;
      //debug() ;
    }
    else {
      //Serial.println("Unset RAM") ;
    }
    prev_RAM_s = cur_RAM_s ;
  }

  byte cur_MAR_s = digitalRead(MAR_s) ;
  if (cur_MAR_s != prev_MAR_s){
    if (cur_MAR_s){
      //Serial.print("Set MAR: ") ;
      digitalWrite(CLOCK_IN, HIGH) ;
      digitalWrite(LATCH_IN, HIGH) ;
      MAR = shiftIn(DATA_IN, CLOCK_IN, MSBFIRST) ;
      digitalWrite(LATCH_IN, LOW) ;
      //Serial.println(MAR) ; 
      //debug() ;
    }
    else {
      //Serial.println("Unset MAR") ;
    }
    prev_MAR_s = cur_MAR_s ;
  }
}


byte get_RAM(byte addr){
  return RAM[addr] ;
}


void debug(){
  Serial.print("  MAR: ") ;
  Serial.print(MAR) ;
  Serial.print(", RAM[MAR]: ") ;
  Serial.println(RAM[MAR]) ;  
  Serial.println("RAM dump:") ;
  
  for (int i = 0 ; i < 256 ; i++){
    if (RAM_set[i]){
      Serial.print("  RAM[") ;
      Serial.print(i) ;
      Serial.print("] = ") ;
      Serial.println(RAM[i]) ;      
    }
  }
}
