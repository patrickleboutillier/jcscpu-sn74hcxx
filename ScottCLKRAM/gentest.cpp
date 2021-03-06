#include "gentest.h"


const Reg rmap[] = {R0, R1, R2, R3} ;
const char *rdesc[] = {"R0", "R1", "R2", "R3"} ;


#define REG_STATE   0
#define FLAG_STATE  4
#define PROG_START  8

#define ALU_INSTS   0b1000, 0b1001, 0b1010, 0b1011, 0b1100, 0b1101, 0b1110, 0b1111, 0b0110
#define BUS_INSTS   0b0000, 0b0001, 0b0010
#define JMP_INSTS   0b0011, 0b0100, 0b0101

const byte insts[] = {ALU_INSTS, BUS_INSTS, JMP_INSTS} ;

// Used to print debug info
char buf[64] ;

  
// We assume RAM has been cleared just before calling this function
void gen_test_prog(byte *RAM, Program *p){
  // First we select the instruction we are going to use.
  byte inst = insts[random(sizeof(insts))] ;

  // We are going to store the reg and flags state in RAM slots 1-8, si we must start off
  // with a jump to address 9.
  p->JMP(PROG_START) ;
  for (byte i = 0 ; i < 8 ; i++){
    p->CLF() ;
  }
  
  // Generate random values between 128 and 256 to testing purposes
  // Above 128 to ensure we don't write over our program
  int minval = 128 ;
  int maxval = 256 ;
  if (inst > 7){ // ALU instruction, no addresses involved so we can use the entire RAM range.
    minval = 0 ;
  }
  byte r0 = random(minval, maxval) ;
  byte r1 = random(minval, maxval) ;
  byte r2 = random(minval, maxval) ;
  byte r3 = random(minval, maxval) ;
  byte data = random(minval, maxval) ;
  byte r[] = {r0, r1, r2, r3} ;
  sprintf(buf, "r0=%d, r1=%d, r2=%d, r3=%d", r0, r1, r2, r3) ;
  Serial.println(buf) ;
  
  // Store these values in RAM in the proper reserved slots, and generate the equivalent 
  // instructions
  RAM[REG_STATE] = r0 ;
  RAM[REG_STATE+1] = r1 ;
  RAM[REG_STATE+2] = r2 ;
  RAM[REG_STATE+3] = r3 ;
  for (byte i = 0 ; i < 4 ; i++){
      p->DATA(R1, REG_STATE+i) ;
      p->DATA(R0, r[i]) ;
      p->ST(R1, R0) ;
  }

  // Generate random flag values (C, A, E, Z)
  byte c = random(2) ;
  byte a = random(2) ;
  byte e = random(2) ;
  if (a && e){
    e = 0 ;
  }
  byte z = random(2) ;
  byte f[] = {c, a, e, z} ;
  byte flags = (c << 3) | (a << 2) | (e << 1) | z ;
  sprintf(buf, "c=%d, a=%d, e=%d, z=%d", c, a, e, z) ;
  Serial.println(buf) ;

  // Store them in the proper reserved slots and generate the equivalent instructions.
  RAM[FLAG_STATE] = c ;
  RAM[FLAG_STATE+1] = a ;
  RAM[FLAG_STATE+2] = e ;
  RAM[FLAG_STATE+3] = z ;
  for (byte i = 0 ; i < 4 ; i++){
      p->DATA(R1, FLAG_STATE+i) ;
      p->DATA(R0, f[i]) ;
      p->ST(R1, R0) ;
  }

  // Run the instructions to set the FLAGS properly
  set_flags(p, flags) ;


  // Load the registers just before running the instruction
  for (byte i = 0 ; i < 4 ; i++){
      p->DATA(rmap[i], r[i]) ;
  }
  
  if (inst < 8){  // non-ALU instruction, address contents are needed so we write values to RAM.
    // Copy reg values to equivalent RAM address for address calculations
    RAM[r0] = r0 ;
    RAM[r1] = r1 ;
    RAM[r2] = r2 ;
    RAM[r3] = r3 ;
    // Generate the equivalent instructions
    for (byte i = 0 ; i < 4 ; i++){
      p->ST(rmap[i], rmap[i]) ;
    }
  }


  // So now the initial state is set. The next step is to generate a random test instruction
  // Pick 2 registers at random to use in the instruction. Also choose an unused register.
  byte ra = random(4) ;
  byte rb = random(4) ;
  byte rx ;
  for (byte i = 0 ; i < 4 ; i++){
    if ((i != ra)&&(i != rb)){
      rx = i ;
      break ;
    }
  }
  sprintf(buf, "ra=%d, rb=%d, rx=%d, c=%d", ra, rb, rx, c) ;
  Serial.println(buf) ;


  // Simulate instruction and update RAM

  byte jinst = random(1, 16) ;
  Serial.print("inst=") ;
  Serial.print(inst, BIN) ;
  Serial.print(", jinst=") ;
  Serial.println(jinst, BIN) ;  
  simulate_instruction(p, RAM, inst, jinst, flags, ra, rb, rx, data, c) ;
  do_instruction(p, inst, jinst, flags, ra, rb, rx, data) ;
  

  // Now that the instruction is done, we need to save the register and flags state to RAM.
  p->DATA(rmap[rx], REG_STATE+ra) ;
  p->ST(rmap[rx], rmap[ra]) ;
  p->DATA(rmap[rx], REG_STATE+rb) ;
  p->ST(rmap[rx], rmap[rb]) ;

  // Now we need to store the resulting flags to RAM.
  // Start by setting all the flags locations
  p->DATA(R0, 1) ;
  for (byte i = 0 ; i < 4 ; i++){
    p->DATA(R1, FLAG_STATE+i) ;
    p->ST(R1, R0) ;
  }
  store_flags(p) ;
  p->HALT() ;
  
  // Copy Program p to RAM, skipping the state part.
  byte *backing = p->toByteArray() ;
  for (byte i = 0 ; i < p->getSize() ; i++){
    if (i >= PROG_START){
      RAM[i] = backing[i] ;
    }
  }
}


void set_flags(Program *p, byte flags) {
  p->CLF() ;
  switch (flags){
    case 0b0000:
      // CLF already performed above.
      break ;
    case 0b1000:
      p->DATA(R0, 100) ;
      p->DATA(R1, 200) ;
      p->ADD(R0, R1) ;
      break ;
    case 0b0100:
      p->DATA(R0, 6) ;
      p->DATA(R1, 2) ;
      p->AND(R0, R1) ;
      break ;
    case 0b0010:
      p->DATA(R0, 1) ;
      p->DATA(R1, 1) ;
      p->AND(R0, R1) ;
      break ;
    case 0b0001:
      p->DATA(R0, 1) ;
      p->DATA(R1, 2) ;
      p->AND(R0, R1) ;
      break ;
    case  0b1100:
      p->DATA(R0, 200) ;
      p->DATA(R1, 100) ;
      p->ADD(R0, R1) ;
      break ;
    case 0b1010:
      p->DATA(R0, 200) ;
      p->DATA(R1, 200) ;
      p->ADD(R0, R1) ;
      break ;
    case 0b1001:
      p->DATA(R0, 127) ;
      p->DATA(R1, 129) ;
      p->ADD(R0, R1) ;
      break ;
    case 0b0101:
      p->DATA(R0, 2) ;
      p->DATA(R1, 1) ;
      p->AND(R0, R1) ;
      break ;
    case 0b0011:
      p->DATA(R0, 1) ;
      p->DATA(R1, 1) ;
      p->XOR(R0, R1) ;
      break ;
    case 0b1101:
      p->DATA(R0, 129) ;
      p->DATA(R1, 127) ;
      p->ADD(R0, R1) ;
      break ;
    case 0b1011:
      p->DATA(R0, 128) ;
      p->DATA(R1, 128) ;
      p->ADD(R0, R1) ;
      break ;
  }

  // DEBUG1() ;
}


void store_flags(Program *p) {
  // Insert a series of jump instructions that will set the flags on the correct location.
  p->DATA(R0, 0) ;
  p->JC(p->getSize() + 5) ;
  p->DATA(R1, FLAG_STATE) ;
  p->ST(R1, R0) ;
  p->JA(p->getSize() + 5) ;
  p->DATA(R1, FLAG_STATE+1) ;
  p->ST(R1, R0) ;
  p->JE(p->getSize() + 5) ;
  p->DATA(R1, FLAG_STATE+2) ;
  p->ST(R1, R0) ;
  p->JZ(p->getSize() + 5) ;
  p->DATA(R1, FLAG_STATE+3) ;
  p->ST(R1, R0) ;
}



void simulate_instruction(Program *p, byte *RAM, byte inst, byte jinst, byte flags, byte ra, byte rb, byte rx, byte data, byte ci) {
  int set_flags = 0 ;
  byte c = 0 ;
  byte a = RAM[REG_STATE+ra] > RAM[REG_STATE+rb] ;
  byte e = RAM[REG_STATE+ra] == RAM[REG_STATE+rb] ;
  int z = -1 ;

  switch (inst){
    case 0b0000:       // LD
      RAM[REG_STATE+rb] = RAM[RAM[REG_STATE+ra]] ;
       break ;
    case 0b0001:       // ST
      RAM[RAM[REG_STATE+ra]] = RAM[REG_STATE+rb] ;
      break ;
    case 0b0010:       // DATA
      RAM[REG_STATE+rb] = data ;
      break ;
    case 0b0011:       // JMPR
    case 0b0100:       // JMP
      // Do nothing, as the JUMP has no effect if performed.
      break ;
    case 0b0101:       // JXXX
      // We need to figure out if we will jump or not, base on $jinst and $flags
      // If NOT, we must produce the proper side-effect.
      if (jinst & flags){
        // Jump, do nothing
      }
      else {
        // No jump, produce side-effect.
        RAM[RAM[REG_STATE+ra]] = RAM[REG_STATE+rb] ;
      }
      break ;
    case 0b0110:        // CLF
      c = 0 ;
      a = 0 ;
      e = 0 ;
      z = 0 ;
      set_flags = 1 ;
      break ;
    case 0b1000: {      // ADD
      int sum = RAM[REG_STATE+ra] + RAM[REG_STATE+rb] + ci ;
      c = sum > 255 ;
      RAM[REG_STATE+rb] = sum % 256 ;
      set_flags = 1 ;
      break ;
    }
    case 0b1001:        // SHR
      c = RAM[REG_STATE+ra] % 2 ;
      RAM[REG_STATE+rb] = (RAM[REG_STATE+ra] + 256*ci) >> 1 ;
      set_flags = 1 ;
      break ;
    case 0b1010:        // SHL
      c = RAM[REG_STATE+ra] >= 128 ;
      RAM[REG_STATE+rb] = ((RAM[REG_STATE+ra] << 1) % 256) + ci ;
      set_flags = 1 ;
      break ;
    case 0b1011:        // NOT
      RAM[REG_STATE+rb] = (~ RAM[REG_STATE+ra]) % 256 ;
      set_flags = 1 ;
      break ;
    case 0b1100:        // AND
      RAM[REG_STATE+rb] = RAM[REG_STATE+ra] & RAM[REG_STATE+rb] ;
      set_flags = 1 ;
      break ;
    case 0b1101:        // OR
      RAM[REG_STATE+rb] = RAM[REG_STATE+ra] | RAM[REG_STATE+rb] ;
      set_flags = 1 ;
      break ;
    case 0b1110:        // XOR
      RAM[REG_STATE+rb] = RAM[REG_STATE+ra] ^ RAM[REG_STATE+rb] ;
      set_flags = 1 ;
      break ;
    case 0b1111:        // CMP
      set_flags = -1 ;
      break ;
  }
  
  if (set_flags){
    RAM[FLAG_STATE] = c ;
    RAM[FLAG_STATE+1] = a ;
    RAM[FLAG_STATE+2] = e ;
    RAM[FLAG_STATE+3] = ((z != -1) ? z : ! RAM[REG_STATE+rb]) ;
    if (set_flags == -1){
      RAM[FLAG_STATE+3] = 1 ;
    }
  }
}



void do_instruction(Program *p, byte inst, byte jinst, byte flags, byte ra, byte rb, byte rx, byte data){
  switch (inst){
    case 0b0000:      // LD
      p->LD(rmap[ra], rmap[rb]) ;
      break ;
    case 0b0001:      // ST
      p->ST(rmap[ra], rmap[rb]) ;
      break ;
    case 0b0010:      // DATA
      p->DATA(rmap[rb], data) ;
      break ;
    case 0b0011: {    // JMPR
      byte addr = p->getSize() + 4 ;
      p->DATA(rmap[rx], addr) ;
      p->JMPR(rmap[rx]) ;
      // Create a side-effect if the jump is not performed
      p->ST(rmap[ra], rmap[rb]) ;
      break ;
    }
    case 0b0100:      // JMP
      p->JMP(p->getSize() + 3) ;
      // Create a side-effect if the jump is not performed
      p->ST(rmap[ra], rmap[rb]) ;
      break ;
    case 0b0101: {    // JXXX
      byte addr = p->getSize() + 3 ; 
      insert_jinst(p, jinst, addr) ;
      // Create a side-effect if the jump is not performed
      p->ST(rmap[ra], rmap[rb]) ;     
      break ;
    }
    case 0b0110:      // CLF 
      p->CLF() ;
      break ;
    case 0b1000:      // ADD
      p->ADD(rmap[ra], rmap[rb]) ; 
      break ;
    case 0b1001:      // SHR 
      p->SHR(rmap[ra], rmap[rb]) ;
      break ;
    case  0b1010:     // SHL 
      p->SHL(rmap[ra], rmap[rb]) ;
      break ;
    case 0b1011:      // NOT 
      p->NOT(rmap[ra], rmap[rb]) ;
      break ;
    case 0b1100:      // AND  
      p->AND(rmap[ra], rmap[rb]) ;
      break ;
    case 0b1101:      // OR
      p->OR(rmap[ra], rmap[rb]) ;
      break ;
    case 0b1110:      // XOR 
      p->XOR(rmap[ra], rmap[rb]) ;
      break ;
    case 0b1111:      // CMP
      p->CMP(rmap[ra], rmap[rb]) ;
      break ;
  }
}


void insert_jinst(Program *p, byte jinst, byte addr){
  switch (jinst) {
    case 0b0000:
      p->JX(addr) ;
      break ;
    case 0b0001:
      p->JZ(addr) ;
      break ;
    case 0b0010:
      p->JE(addr) ;
      break ;
    case 0b0011:
      p->JEZ(addr) ;
      break ;
    case 0b0100:
      p->JA(addr) ;
      break ;
    case 0b0101:
      p->JAZ(addr) ;
      break ;
    case 0b0110:
      p->JAE(addr) ;
      break ;
    case 0b0111:
      p->JAEZ(addr) ;
      break ;
    case 0b1000:
      p->JC(addr) ;
      break ;
    case 0b1001:
      p->JCZ(addr) ;
      break ;
    case 0b1010:
      p->JCE(addr) ;
      break ;
    case 0b1011:
      p->JCEZ(addr) ;
      break ;
    case 0b1100:
      p->JCA(addr) ;
      break ;
    case 0b1101:
      p->JCAZ(addr) ;
      break ;
    case 0b1110:
      p->JCAE(addr) ;
      break ;
    case 0b1111:
      p->JCAEZ(addr) ;
      break ;
  }

  return 2 ;
}


bool compare_RAMS(byte *RAM_sim, byte *RAM_cpu){
  int same = 1 ;
  for (int i = 0 ; i < 256 ; i++){
    if (RAM_sim[i] != RAM_cpu[i]){
      sprintf(buf, "RAM_sim[%3d] (%3d) != RAM_cpu[%3d] (%3d)", i, RAM_sim[i], i, RAM_cpu[i]) ;
      Serial.println(buf) ;
      same = 0 ;
    }
  }

  return same ;
}
