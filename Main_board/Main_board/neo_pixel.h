// neo_pixel header

#include <avr/io.h>

void init_neo_pixel();

void static inline volatile send_byte(uint8_t b);
void inline volatile zero(); 
void inline volatile one(); 
void static inline volatile end();
void inline volatile test_neo_pixel(uint8_t r, uint8_t g, uint8_t b);

void inline volatile  reset_neo_pixel();
