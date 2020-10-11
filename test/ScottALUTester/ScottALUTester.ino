

#include "Extension.h"

#define TMP_s 11
#define ACC_s 10
#define ACC_e 12


Extension SA('A' - '0') ;


unsigned long last_millis = 0 ;


int test_alu(byte op, byte ci){
  long nb = 0 ;
  long nb_failed = 0 ;

  SA.digitalWrite(4, op & B100) ;
  SA.digitalWrite(3, op & B010) ;
  SA.digitalWrite(2, op & B001) ;
  SA.digitalWrite(5, ci) ;
      
  Serial.print(F("ALU op is ")) ;
  Serial.print(op) ;
  Serial.print(", carry in is ") ;
  Serial.println(ci) ; 
  for (int a = 0 ; a < 256 ; a++){
    // Serial.print(F("Starting B iterations with A=")) ;
    // Serial.println(a, BIN) ;
    for (int b = 0 ; b < 256 ; b++){
      enable() ;
      set_byte(b) ;
      digitalWrite(TMP_s, HIGH) ;
      digitalWrite(TMP_s, LOW) ;
      set_byte(a) ;
      int expected = soft_alu(op, a, b, ci) ;
      // Save result to ACC
      digitalWrite(ACC_s, HIGH) ;
      digitalWrite(ACC_s, LOW) ;
      // Save flags
      byte carry_out = digitalRead(A0) ;
      byte alarger = digitalRead(A1) ;
      byte eq = digitalRead(A2) ;
      byte zero = digitalRead(A3) ;
      disable() ; 
      // Enable ACC and read result
      digitalWrite(ACC_e, HIGH) ;              

      byte c = get_byte() ;
      if (op == 7){
        c = 0 ;
        zero = 1 ;
      }
      
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
        return 0 ;
      }
      nb++ ;

      digitalWrite(ACC_e, LOW) ;  
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

  return 1 ;
}


void enable(){
  for (byte pin = 9 ; pin > 1 ; pin--){
    pinMode(pin, OUTPUT) ;
  } 
}


void disable(){
  for (byte pin = 9 ; pin > 1 ; pin--){
    pinMode(pin, INPUT) ;
  } 
}


void set_byte(byte b){
  for (byte pin = 9 ; pin > 1 ; pin--){
    digitalWrite(pin, bitRead(b, pin - 2)) ;
  }
}


byte get_byte(){
  byte b = 0 ;
  for (byte pin = 9 ; pin > 1 ; pin--){
    bitWrite(b, pin - 2, digitalRead(pin)) ;
  }
  return b ;
}


void setup(){
  Serial.begin(9600) ;
  Serial.println(F("Setting pin modes and initializing output pins to LOW")) ;
  SA.enableDigitalCache() ;
  for (byte pin = 9 ; pin > 1 ; pin--){
    pinMode(pin, INPUT) ; 
  }

  // Flags
  pinMode(A0, INPUT) ; 
  pinMode(A1, INPUT) ; 
  pinMode(A2, INPUT) ; 
  pinMode(A3, INPUT) ; 

  // Control pins
  pinMode(TMP_s, OUTPUT) ;
  pinMode(ACC_s, OUTPUT) ;
  pinMode(ACC_e, OUTPUT) ;

  // OP + CI
  SA.pinMode(4, OUTPUT) ;
  SA.pinMode(3, OUTPUT) ;
  SA.pinMode(2, OUTPUT) ; 
  SA.pinMode(5, OUTPUT) ;  
  
  delay(1000) ;
  Serial.println(F("Starting tests...")) ;

  while (1){
    for (byte op = 0 ; op < 8 ; op++){
      for (byte ci = 0 ; ci < 2 ; ci++){
        if (! test_alu((byte)op, ci)){
          Serial.println("HALTING!") ;
          return ;
        }
      }
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
