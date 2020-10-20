/*

// Testing instructions
byte prog[256] = {
  DATA(R0, 17),
  DATA(R1, 34),
  DATA(R2, 68),
  DATA(R3, 136),
  ST(R1, R0),       // RAM[34] = 17    
  ST(R2, R1),       // RAM[68] = 34
  ST(R3, R2),       // RAM[136] = 68
  ST(R0, R3),       // RAM[17] = 136
  LD(R0, R1),       // R1 = 136    
  LD(R1, R2),       // R2 = 68
  LD(R2, R3),       // R3 = 34
  LD(R3, R0),       // R0 = 17
} ;

 */
