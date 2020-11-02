#include "genprog.h"


Reg rmap[] = {R0, R1, R2, R3} ;

#define ALU_INSTS   0b1000, 0b1001, 0b1010, 0b1011, 0b1100, 0b1101, 0b1110, 0b1111, 0b0110
#define BUS_INSTS   0b0000, 0b0001, 0b0010
#define JMP_INSTS   0b0011, 0b0100, 0b0101

byte insts[] = {ALU_INSTS, BUS_INSTS, JMP_INSTS} ;
// byte insts[] = {0b0101} ;

// Used to print debug info
char buf[64] ;

  
// We assume RAM has been cleared just before calling this function
void gen_test_prog(byte *RAM, Program *p){  
  // Generate random values between 64 and 248 to testing purposes
  // Above 64 to ensure we don't write over our program
  // Below 248 to leave place at the end to store our state
  byte minval = 128 ;
  byte maxval = 248 ;
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
  RAM[248] = r0 ;
  RAM[249] = r1 ;
  RAM[250] = r2 ;
  RAM[251] = r3 ;
  for (byte i = 0 ; i < 4 ; i++){
      p->DATA(R1, 248+i) ;
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
  RAM[252] = c ;
  RAM[253] = a ;
  RAM[254] = e ;
  RAM[255] = z ;
  for (byte i = 0 ; i < 4 ; i++){
      p->DATA(R1, 252+i) ;
      p->DATA(R0, f[i]) ;
      p->ST(R1, R0) ;
  }

  // Run the instructions to set the FLAGS properly
  set_flags(p, flags) ;


  // Load the registers just before running the instruction
  for (byte i = 0 ; i < 4 ; i++){
      p->DATA(rmap[i], r[i]) ;
  }
  RAM[r0] = r0 ;
  RAM[r1] = r1 ;
  RAM[r2] = r2 ;
  RAM[r3] = r3 ;
  // Generate the equivalent instructions
  for (byte i = 0 ; i < 4 ; i++){
    p->ST(rmap[i], rmap[i]) ;
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
  byte inst = insts[random(sizeof(insts))] ;
  byte jinst = random(1, 16) ;
  Serial.print("inst=") ;
  Serial.print(inst, BIN) ;
  Serial.print(", jinst=") ;
  Serial.println(jinst, BIN) ;  
  simulate_instruction(p, RAM, inst, jinst, flags, ra, rb, rx, data, c) ;
  do_instruction(p, inst, jinst, flags, ra, rb, rx, data) ;
  

  // Now that the instruction is done, we need to save the register and flags state to RAM.
  p->DATA(rmap[rx], 248+ra) ;
  p->ST(rmap[rx], rmap[ra]) ;
  p->DATA(rmap[rx], 248+rb) ;
  p->ST(rmap[rx], rmap[rb]) ;

  // Now we need to store the resulting flags to RAM.
  // Start by setting all the flags locations
  p->DATA(R0, 1) ;
  for (byte i = 0 ; i < 4 ; i++){
    p->DATA(R1, 252+i) ;
    p->ST(R1, R0) ;
  }
  store_flags(p) ;
  p->HALT() ;

  // Copy Program p to RAM
  byte *backing = p->toByteArray() ;
  for (byte i = 0 ; i < p->getSize() ; i++){
    RAM[i] = backing[i] ;
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
  p->DATA(R1, 252) ;
  p->ST(R1, R0) ;
  p->JA(p->getSize() + 5) ;
  p->DATA(R1, 253) ;
  p->ST(R1, R0) ;
  p->JE(p->getSize() + 5) ;
  p->DATA(R1, 254) ;
  p->ST(R1, R0) ;
  p->JZ(p->getSize() + 5) ;
  p->DATA(R1, 255) ;
  p->ST(R1, R0) ;
}



void simulate_instruction(Program *p, byte *RAM, byte inst, byte jinst, byte flags, byte ra, byte rb, byte rx, byte data, byte ci) {
  int set_flags = 0 ;
  byte c = 0 ;
  byte a = RAM[248+ra] > RAM[248+rb] ;
  byte e = RAM[248+ra] == RAM[248+rb] ;
  int z = -1 ;

  switch (inst){
    case 0b0000:       // LD
      RAM[248+rb] = RAM[RAM[248+ra]] ;
       break ;
    case 0b0001:       // ST
      RAM[RAM[248+ra]] = RAM[248+rb] ;
      break ;
    case 0b0010:       // DATA
      RAM[248+rb] = data ;
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
        RAM[RAM[248+ra]] = RAM[248+rb] ;
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
      int sum = RAM[248+ra] + RAM[248+rb] + ci ;
      c = sum > 255 ;
      RAM[248+rb] = sum % 256 ;
      set_flags = 1 ;
      break ;
    }
    case 0b1001:        // SHR
      c = RAM[248+ra] % 2 ;
      RAM[248+rb] = (RAM[248+ra] + 256*ci) >> 1 ;
      set_flags = 1 ;
      break ;
    case 0b1010:        // SHL
      c = RAM[248+ra] >= 128 ;
      RAM[248+rb] = ((RAM[248+ra] << 1) % 256) + ci ;
      set_flags = 1 ;
      break ;
    case 0b1011:        // NOT
      RAM[248+rb] = (~ RAM[248+ra]) % 256 ;
      set_flags = 1 ;
      break ;
    case 0b1100:        // AND
      RAM[248+rb] = RAM[248+ra] & RAM[248+rb] ;
      set_flags = 1 ;
      break ;
    case 0b1101:        // OR
      RAM[248+rb] = RAM[248+ra] | RAM[248+rb] ;
      set_flags = 1 ;
      break ;
    case 0b1110:        // XOR
      RAM[248+rb] = RAM[248+ra] ^ RAM[248+rb] ;
      set_flags = 1 ;
      break ;
    case 0b1111:        // CMP
      set_flags = -1 ;
      break ;
  }
  
  if (set_flags){
    RAM[252] = c ;
    RAM[253] = a ;
    RAM[254] = e ;
    RAM[255] = ((z != -1) ? z : ! RAM[248+rb]) ;
    if (set_flags == -1){
      RAM[255] = 1 ;
    }
  }
}



void do_instruction(Program *p, byte inst, byte jinst, byte flags, byte ra, byte rb, byte rx, byte data){
  switch (inst){
    case 0b0000:     // LD
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
      //warn sprintf("flags: %04b", $flags) ;
      p->push_back(0b01010000 | jinst) ;
      p->push_back(addr) ;
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
