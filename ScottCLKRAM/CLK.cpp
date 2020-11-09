#include "Arduino.h"
#include "CLK.h"


bool clk_automatic ;
bool clk_started ;
unsigned long clk_then ;
long clk_qtick ;
byte clk_step ; 

bool buttonState ;
bool lastButtonState ;
unsigned long lastDebounceTime = 0 ;
unsigned long debounceDelay = 50 ;


bool button_pressed(byte button) ;
void qtick() ;


void reset_CLK(){
  clk_started = 0 ;
  clk_then = millis() ;
  clk_qtick = -1 ;
  clk_step = 0 ; 

  if (clk_automatic){
    buttonState = LOW ;
    lastButtonState = LOW ;
  }
  else {
    buttonState = HIGH ;
    lastButtonState = HIGH ;
  }
  
  digitalWrite(CLK_WAITING, HIGH) ;  
  digitalWrite(CLKSTP_ENABLE, HIGH) ; 
}


void setup_CLK(){
  pinMode(CLK_MANUAL, INPUT) ;
  pinMode(CLK_WAITING, OUTPUT) ;

  pinMode(CLKSTP_DATA, OUTPUT) ; 
  pinMode(CLKSTP_LATCH, OUTPUT) ; 
  pinMode(CLKSTP_CLOCK, OUTPUT) ; 
  pinMode(CLKSTP_ENABLE, OUTPUT) ; 

  clk_automatic = 1 ;
  if (digitalRead(CLK_MANUAL) == HIGH){
    // Request to have clock in manual mode
    clk_automatic = 0 ;
  }  
}


void loop_CLK(byte slowdown){
  if (clk_automatic){
    if (! clk_started){
      //if (button_pressed(CLK_MANUAL)){
        //delay(1000) ;
        clk_started = 1 ;
        digitalWrite(CLK_WAITING, LOW) ;
        return ;
      //}
    }
    else {
      // Automatic clock
      unsigned long clk_now = millis() ;
      if ((clk_now - clk_then) >= (1000 / ((CLK_HZ / slowdown) * 4))){
        qtick() ;
        clk_then = clk_now ;
      }
    }
  }
  else {
    // Manual clock
    if (button_pressed(CLK_MANUAL)){
      qtick() ;
    }
  }
}


void qtick(){
  // Update qtick
  clk_qtick++ ;

  bool clk_e = 0 ;
  bool clk_s = 0 ;
  switch (clk_qtick % 4){
    case 0:
      clk_e = 1 ;
      clk_s = 0 ;
      break ;
    case 1:
      clk_e = 1 ;
      clk_s = 1 ;
      break ;
    case 2:
      clk_e = 1 ;
      clk_s = 0 ;
      break ;
    case 3:
      clk_e = 0 ;
      clk_s = 0 ;
      break ;
  }

  if ((clk_qtick % 4) == 0){
    clk_step++ ;
    if (clk_step == 7){
      clk_step = 1 ;
    }
    //Serial.print("STEP = ") ;
    //Serial.println(clk_step) ;
  }
  
  digitalWrite(CLKSTP_ENABLE, HIGH) ;
  digitalWrite(CLKSTP_LATCH, LOW) ;
  shiftOut(CLKSTP_DATA, CLKSTP_CLOCK, MSBFIRST, 
    ((clk_step == 1) << 7) |
    ((clk_step == 2) << 6) |
    ((clk_step == 3) << 5) |
    ((clk_step == 4) << 4) |
    ((clk_step == 5) << 3) |
    ((clk_step == 6) << 2) |
    (clk_e << 1) | 
    clk_s) ;
  digitalWrite(CLKSTP_LATCH, HIGH) ; 
  digitalWrite(CLKSTP_ENABLE, LOW) ;
}


long get_qtick(){
  return clk_qtick ;
}


/*
Generic debouncing code from https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
*/

bool button_pressed(byte button){
  bool ret = 0 ;
  int reading = digitalRead(button) ;
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis() ;
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay){
    if (reading != buttonState){
      buttonState = reading ;
      if (buttonState == HIGH){
        ret = 1 ;
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading ;
   
  return ret ;
}
