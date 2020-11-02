#ifndef genprog_h
#define genprog_h

#include "Program.h"
#include "Arduino.h"

void gen_test_prog(byte *RAM, Program *p) ;
void set_flags(Program *p, byte flags) ;
void store_flags(Program *p) ;
bool compare_RAMS(byte *RAM_sim, byte *RAM_cpu) ;
void do_instruction(Program *p, byte inst, byte jinst, byte flags, byte ra, byte rb, byte rx, byte data) ;
void simulate_instruction(Program *p, byte *RAM, byte inst, byte jinst, byte flags, byte ra, byte rb, byte rx, byte data, byte ci) ;

#endif
