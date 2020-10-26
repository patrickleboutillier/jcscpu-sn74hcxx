#ifndef CLK_H
#define CLK_H


#define CLK_MANUAL    A4
#define CLK_WAITING   A5

#define CLKSTP_DATA   3
#define CLKSTP_LATCH  4
#define CLKSTP_CLOCK  5
#define CLKSTP_ENABLE 6

#define CLK_HZ 64


void setup_CLK() ;
void reset_CLK() ;
void loop_CLK() ;
long get_qtick() ;


#endif
