/*


#include "Extension.h"

Extension SA('A' - '0') ; 
Extension SB('B' - '0') ; 

unsigned long last_millis = 0 ;


void test_alu(byte op, byte ci){
  long nb = 0 ;
  long nb_failed = 0 ;

  digitalWrite(12, op & B100) ;
  digitalWrite(11, op & B010) ;
  digitalWrite(10, op & B001) ;
  digitalWrite(13, ci) ;
      
  Serial.print(F("ALU op is ")) ;
  Serial.print(op) ;
  Serial.print(", carry in is ") ;
  Serial.println(ci) ; 
  for (int a = 0 ; a < 256 ; a++){
    // Serial.print(F("Starting B iterations with A=")) ;
    // Serial.println(a, BIN) ;
    set_byte(&SA, a) ;
    for (int b = 0 ; b < 256 ; b++){
      set_byte(&SB, b) ;
      if (op == 0){
        delay(1) ;
      }
      byte c = get_byte() ;
      byte carry_out = digitalRead(A0) ;
      byte alarger = digitalRead(A1) ;
      byte eq = digitalRead(A2) ;
      byte zero = digitalRead(A3) ;
      int expected = soft_alu(op, a, b, ci) ;
      int got = (carry_out << 11) | (alarger << 10) | (eq << 9) | (zero << 8) | c ;
      if (got != expected){
        Serial.print(F("FAILED TEST: ")) ;
        Serial.print("A=") ;
        Serial.print(a, (op == 0 ? DEC : BIN)) ;
        Serial.print(", B=") ;
        Serial.print(b, (op == 0 ? DEC : BIN)) ;
        Serial.print(", EF=") ;
        Serial.print(expected >> 8, BIN) ;
        Serial.print(", GOTF=") ;
        Serial.print(got >> 8, BIN) ;  
        Serial.print(", E=") ;
        Serial.print(expected & 0xFF, (op == 0 ? DEC : BIN)) ;
        Serial.print(", GOT=") ;
        Serial.println(got & 0xFF, (op == 0 ? DEC : BIN)) ;    
        nb_failed++ ;
      }
      nb++ ;
    }
  }
  Serial.print("  ") ;
  Serial.print(nb_failed) ;
  Serial.print(" / ") ;
  Serial.print(nb) ;
  Serial.print(F(" tests failed in ")) ; 
  unsigned long now = millis() ;
  Serial.print((now - last_millis) / 1000) ;
  last_millis = now ;
  Serial.println(F(" seconds.")) ;
}


void setup(){
  Serial.begin(9600) ;
  Serial.println(F("Setting pin modes and initializing output pins to LOW")) ;
  SA.enableDigitalCache() ;
  SB.enableDigitalCache() ;
  for (byte pin = 9 ; pin > 1 ; pin--){
    SA.pinMode(pin, OUTPUT) ;
    SA.digitalWrite(pin, LOW) ;
    SB.pinMode(pin, OUTPUT) ;
    SB.digitalWrite(pin, LOW) ;
    pinMode(pin, INPUT) ; 
  }

  // Flags
  pinMode(A0, INPUT) ; 
  pinMode(A1, INPUT) ; 
  pinMode(A2, INPUT) ; 
  pinMode(A3, INPUT) ; 

  // OP + CI
  pinMode(12, OUTPUT) ;
  pinMode(11, OUTPUT) ;
  pinMode(10, OUTPUT) ;  
  pinMode(13, OUTPUT) ;  
  
  delay(1000) ;
  Serial.println(F("Starting tests...")) ;
 
  for (int op = 7 ; op >= 0 ; op--){
    for (int ci = 0 ; ci < 2 ; ci++){
      test_alu((byte)op, (byte)ci) ;
    }
  }
}


void loop(){
}


int soft_alu(int op, byte a, byte b, byte carry_in){
  byte c = 0 ;
  byte co = 0 ;
  switch (op){
    case 0: {  // ADD
      int sum = (int)a + (int)b + (int)carry_in ;
      c = (byte)sum ;
      if (sum >= 256){
        c = sum - 256 ;
        co = 1 ;
      }
      break ;
    }
    case 1: // SHR
      if (a % 2){
        co = 1 ;
      }
      c = (carry_in << 7) | (a >> 1) ;
      break ;
    case 2: // SHL
      if (a >= 128){
        co = 1 ;
      }
      c = (a << 1) | carry_in ;
      break ;
    case 3: // NOT
      c = ~a ;
      break ;
    case 4: // AND
      c = a & b ;
      break ;
    case 5: // OR
      c = a | b ;
      break ;
    case 6: // XOR
      c = a ^ b ;
      break ;
    case 7: // CMP
      break ;
  }
 
  byte alarger = (a > b) ;
  byte eq = (a == b) ;
  byte zero = (c == 0) ;
  byte carry_out = co ;
  
  return (carry_out << 11) | (alarger << 10) | (eq << 9) | (zero << 8) | c ; 
}


void set_byte(Extension *S, byte b){
  for (byte pin = 9 ; pin > 1 ; pin--){
    S->digitalWrite(pin, bitRead(b, pin - 2)) ;
  }
}


byte get_byte(){
  byte b = 0 ;
  for (byte pin = 9 ; pin > 1 ; pin--){
    bitWrite(b, pin - 2, digitalRead(pin)) ;
  }
  return b ;
}


*/
