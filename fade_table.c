#include "avr/io.h"
#include "avr/pgmspace.h"
#include "fade_table.h"

// circle slice, centered at (-400,-400) with a radius of 767.48
const uint8_t PROGMEM circular_mod[] = {
255, 254, 254, 253, 253, 252, 251, 251, 250, 249, 249, 248, 248, 247, 246, 246, 
245, 244, 244, 243, 242, 242, 241, 240, 240, 239, 238, 238, 237, 236, 236, 235, 
234, 234, 233, 232, 232, 231, 230, 230, 229, 228, 227, 227, 226, 225, 225, 224, 
223, 222, 222, 221, 220, 220, 219, 218, 217, 217, 216, 215, 214, 214, 213, 212, 
211, 211, 210, 209, 208, 208, 207, 206, 205, 204, 204, 203, 202, 201, 200, 200, 
199, 198, 197, 196, 196, 195, 194, 193, 192, 192, 191, 190, 189, 188, 187, 187, 
186, 185, 184, 183, 182, 181, 181, 180, 179, 178, 177, 176, 175, 174, 174, 173, 
172, 171, 170, 169, 168, 167, 166, 165, 164, 164, 163, 162, 161, 160, 159, 158, 
157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 
141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 
125, 124, 123, 122, 121, 119, 118, 117, 116, 115, 114, 113, 112, 111, 109, 108, 
107, 106, 105, 104, 103, 101, 100, 99,  98,  97,  96,  94,  93,  92,  91,  90, 
88,  87,  86,  85,  84,  82,  81,  80,  79,  77,  76,  75,  74,  72,  71,  70,  
68,  67,  66,  64,  63,  62,  60,  59,  58,  56,  55,  54,  52,  51,  50,  48,  
47,  45,  44,  43,  41,  40,  38,  37,  35,  34,  33,  31,  30,  28,  27,  25,  
24,  22,  21,  19,  17,  16,  14,  13,  11,  10,  8,   6,   5,   3,   2,   0
};

// union of 3 lines with slopes of -1/2, -1, and -1 with intercepts to meet
const uint8_t PROGMEM linear_mod[] = {
255, 255, 254, 254, 253, 253, 252, 252, 251, 251, 250, 250, 249, 249, 248, 248,
247, 247, 246, 246, 245, 245, 244, 244, 243, 243, 242, 242, 241, 241, 240, 240,
239, 239, 238, 238, 237, 237, 236, 236, 235, 235, 234, 234, 233, 233, 232, 232,
231, 231, 230, 230, 229, 229, 228, 228, 227, 227, 226, 226, 225, 225, 224, 224,
223, 223, 222, 222, 221, 221, 220, 220, 219, 219, 218, 218, 217, 217, 216, 216,
215, 215, 214, 214, 213, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203,
202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187,
186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171,
170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155,
154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139,
138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123,
122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107,
106, 105, 104, 103, 102, 101, 100, 99,  98,  97,  96,  95,  94,  93,  92,  91, 
90,  89,  88,  87,  86,  84,  82,  80,  78,  76,  74,  72,  70,  68,  66,  64, 
62,  60,  58,  56,  54,  52,  50,  48,  46,  44,  42,  40,  38,  36,  34,  32, 
30,  28,  26,  24,  22,  20,  18,  16,  14,  12,  10,  8,   6,   4,   2,   0
};

// square root curve reflected across y axis and shifted
const uint8_t PROGMEM quadratic[] = {
255, 254, 254, 253, 253, 252, 252, 251, 251, 250, 250, 249, 249, 248, 248, 247, 
247, 246, 246, 245, 245, 244, 244, 243, 243, 242, 242, 241, 241, 240, 240, 239, 
238, 238, 237, 237, 236, 236, 235, 235, 234, 234, 233, 233, 232, 231, 231, 230, 
230, 229, 229, 228, 228, 227, 226, 226, 225, 225, 224, 224, 223, 222, 222, 221, 
221, 220, 220, 219, 218, 218, 217, 217, 216, 215, 215, 214, 214, 213, 212, 212, 
211, 211, 210, 209, 209, 208, 208, 207, 206, 206, 205, 204, 204, 203, 203, 202, 
201, 201, 200, 199, 199, 198, 198, 197, 196, 196, 195, 194, 194, 193, 192, 192, 
191, 190, 190, 189, 188, 188, 187, 186, 186, 185, 184, 183, 183, 182, 181, 181, 
180, 179, 179, 178, 177, 176, 176, 175, 174, 173, 173, 172, 171, 170, 170, 169, 
168, 167, 167, 166, 165, 164, 164, 163, 162, 161, 160, 160, 159, 158, 157, 156, 
156, 155, 154, 153, 152, 151, 151, 150, 149, 148, 147, 146, 145, 145, 144, 143, 
142, 141, 140, 139, 138, 137, 136, 135, 135, 134, 133, 132, 131, 130, 129, 128, 
127, 126, 125, 124, 123, 122, 121, 119, 118, 117, 116, 115, 114, 113, 112, 111, 
109, 108, 107, 106, 105, 103, 102, 101, 100, 98,  97,  96,  94,  93,  92,  90, 
89,  87,  86,  84,  83,  81,  80,  78,  77,  75,  73,  71,  70,  68,  66,  64,  
62,  60,  58,  55,  53,  50,  48,  45,  42,  39,  36,  32,  28,  23,  16,  0
};

uint8_t get_brightness(enum fade_table table, uint8_t fade_step) {
  const uint8_t* table_ptr = 0;
  switch( table ) {
    case CIRCULAR_MOD:
      table_ptr = circular_mod;
      break;
    case LINEAR_MOD:
      table_ptr = linear_mod;
      break;
    case QUADRATIC:
      table_ptr = quadratic;
      break;
  }
  // AVR compiler magic - YCM doesn't like it!
  return pgm_read_byte(&table_ptr[fade_step]);
}