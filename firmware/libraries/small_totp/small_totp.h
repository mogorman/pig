#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class small_totp {

 public:
  small_totp(const uint8_t *secret, const uint8_t length);
  uint32_t code(uint32_t time);
  // private:

};
