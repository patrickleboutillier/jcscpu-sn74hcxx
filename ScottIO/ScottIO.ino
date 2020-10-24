#include <LiquidCrystal.h>


#define IO_e     3
#define IO_s     4
#define IO_mode  3
#define IO_type  4
 
int cur_pos = 0 ;
char buf[33] ;

byte prev_IO_e ;
byte prev_IO_s ;


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void reset_IO(){
  prev_IO_e = LOW ;
  prev_IO_s = LOW ;

  // Clear TTY buffer
  for (byte i = 0 ; i < 32 ; i++){
    buf[i] = ' ' ;
  }
  buf[32] = '\0' ;
  tty_put(buf) ;
}


void setup(){
  // set up the number of columns and rows on the LCD
  lcd.begin(16, 2) ;
  reset_IO() ;  
}


void loop() {
  byte cur_IO_e = digitalRead(IO_e) ;
  if (cur_IO_e != prev_IO_e){
    if (cur_IO_e){
      if (digitalRead(IO_mode) == LOW){ // INPUT
        if (digitalRead(IO_type) == LOW){ // DATA
          byte r = random(256) ;
          // Send a random byte on the BUS.
        }
      }
    }
    prev_IO_e = cur_IO_e ;
  }

  byte cur_IO_s = digitalRead(IO_s) ;
  if (cur_IO_s != prev_IO_s){
    if (cur_IO_s){
      if (digitalRead(IO_mode) == HIGH){ // OUTPUT
        if (digitalRead(IO_type) == LOW){ // DATA
          byte c ;
          // Grab data on the BUS and send it to the LCD.
          tty_put(c) ;
        }
      }
    }
    prev_IO_s = cur_IO_s ;
  }
}


void tty_put(char c){
  if (cur_pos == 32){
    // Scroll up
    for (byte j = 0 ; j < 16 ; j++){
      buf[j] = buf[j+16] ;
      buf[j+16] = ' ' ;
    }
    cur_pos = 16 ;
  }
    
  buf[cur_pos] = c ;
  cur_pos++ ;

  lcd.setCursor(0, 0) ;
  lcd.print(buf) ;
  lcd.setCursor(0, 1) ;
  lcd.print(buf+16) ; 
}
