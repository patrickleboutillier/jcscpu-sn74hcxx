/*
#define DATA   4
#define LATCH  3
#define CLOCK  2
#define ENABLE A5


unsigned long last_millis = 0 ;


void test_alu(byte op, byte ci){
  long nb = 0 ;
  long nb_failed = 0 ;
      
  Serial.print(F("ALU op is ")) ;
  Serial.print(op) ;
  Serial.print(", carry in is ") ;
  Serial.println(ci) ; 
  for (int a = 0 ; a < 256 ; a++){
    Serial.print(F("Starting B iterations with A=")) ;
    Serial.println(a, BIN) ;
    for (int b = 0 ; b < 256 ; b++){
      set_bytes(op, ci, a, b) ;
      delay(100) ;
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


void set_bytes(byte op, byte ci, byte a, byte b){
  digitalWrite(ENABLE, HIGH) ;
  digitalWrite(LATCH, LOW) ;
  shiftOut(DATA, CLOCK, MSBFIRST, b) ;
  shiftOut(DATA, CLOCK, MSBFIRST, a) ;
  shiftOut(DATA, CLOCK, MSBFIRST, (ci << 3) | op) ;
  digitalWrite(LATCH, HIGH) ; 
  digitalWrite(ENABLE, LOW) ;
}


byte get_byte(){
  byte b = 0 ;
  for (byte pin = 12 ; pin > 4 ; pin--){
    bitWrite(b, pin - 5, digitalRead(pin)) ;
  }
  return b ;
}


void setup(){
  Serial.begin(9600) ;
  Serial.println(F("Setting pin modes and initializing output pins to LOW")) ;
  for (byte pin = 12 ; pin > 4 ; pin--){
    pinMode(pin, INPUT) ; 
  }

  // Flags
  pinMode(A0, INPUT) ; 
  pinMode(A1, INPUT) ; 
  pinMode(A2, INPUT) ; 
  pinMode(A3, INPUT) ; 

  // Shift register control pins 
  pinMode(CLOCK, OUTPUT) ; 
  pinMode(DATA, OUTPUT) ; 
  pinMode(LATCH, OUTPUT) ; 
  pinMode(ENABLE, OUTPUT) ; 
  digitalWrite(ENABLE, LOW) ; 
  
  delay(1000) ;
  Serial.println(F("Starting tests...")) ;
 
  for (int op = 6 ; op >= 0 ; op--){
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
*/
