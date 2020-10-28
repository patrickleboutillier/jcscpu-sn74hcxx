#ifndef RAM_H
#define RAM_H


#define RAM_e A0
#define RAM_s A1
#define MAR_s A2

#define CLOCK_OUT     12
#define DATA_OUT      11
#define LATCH_OUT     10
#define CLOCK_IN      9
#define DATA_IN       8
#define LATCH_IN      7


void setup_RAM() ;
void reset_RAM(byte prog[], byte prog_size);
void loop_RAM() ;
byte* get_RAM() ;


#endif
