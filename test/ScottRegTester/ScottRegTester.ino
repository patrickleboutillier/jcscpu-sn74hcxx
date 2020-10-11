
#define REG_e 11
#define REG_s 10

byte BUS[] = {9, 8, 7, 6, 5, 4, 3, 2} ;

void setup() {
  Serial.begin(9600) ;
  pinMode(REG_e, OUTPUT) ;
  digitalWrite(REG_e, LOW) ; // Enable off
  pinMode(REG_s, OUTPUT) ;
  digitalWrite(REG_e, HIGH) ; // Set off
  
  readBUS() ;
  
  // Allow time for chips to settle ; 
  delay(1000) ;

  testReg() ;
}


void loop(){
}


void testReg(){
  long nb = 0 ;
  long nb_failed = 0 ;
  for (int b = 0 ; b < 256 ; b++){
    byte expected = b ;
    
    writeBUS(b) ;
    digitalWrite(REG_s, HIGH) ; // Set on
    //delay(1) ;
    digitalWrite(REG_s, LOW) ; // Set off    
    // Stop writing to BUS anbd clear pins
    for (byte i = 0 ; i < 8 ; i++){
      pinMode(BUS[i], INPUT) ;
      digitalWrite(BUS[i], HIGH) ; 
    }
    digitalWrite(REG_e, HIGH) ; // Enable on
    //delay(1) ;
    byte got = readBUS() ; 
    digitalWrite(REG_e, LOW) ; // Enable off
            
    if (got != expected){
      Serial.print(F("FAILED TEST: ")) ;
      Serial.print(", B=") ;
      Serial.print(b, BIN) ;
      Serial.print(", E=") ;
      Serial.print(expected, BIN) ;
      Serial.print(", GOT=") ;
      Serial.println(got, BIN) ;        
      nb_failed++ ;
    }
    nb++ ;
  }
  Serial.print(nb_failed) ;
  Serial.print(" / ") ;
  Serial.print(nb) ;
  Serial.print(F(" tests failed in ")) ; 
  Serial.print(millis() / 1000) ;
  Serial.println(F(" seconds.")) ;
}


byte readBUS() {
  byte ret = 0 ;
  for (byte i = 0 ; i < 8 ; i++){
    pinMode(BUS[i], INPUT) ;
    bitWrite(ret, 7-i, digitalRead(BUS[i])) ; 
  }
  return ret ;
}


void writeBUS(byte b){
  for (byte i = 0 ; i < 8 ; i++){
    pinMode(BUS[i], OUTPUT) ;
    digitalWrite(BUS[i], bitRead(b, 7-i)) ; 
  }
}
