#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


#define SHA1_K0  0x5a827999
#define SHA1_K20 0x6ed9eba1
#define SHA1_K40 0x8f1bbcdc
#define SHA1_K60 0xca62c1d6

#define HMAC_IPAD 0x36
#define HMAC_OPAD 0x5c

#define HASH_LENGTH 20
#define BLOCK_LENGTH 64

typedef struct sha1nfo {
	uint32_t buffer[BLOCK_LENGTH/4];
	uint32_t state[HASH_LENGTH/4];
	uint32_t byteCount;
	uint8_t bufferOffset;
	uint8_t keyBuffer[BLOCK_LENGTH];
	uint8_t innerHash[HASH_LENGTH];
} sha1nfo;


class small_totp {

 public:
  small_totp(const uint8_t *secret, const uint8_t length);
  uint32_t code(uint32_t time);
 
 private:
  sha1nfo s;

  void sha1_init();
  uint32_t sha1_rol32(uint32_t number, uint8_t bits);
  void sha1_hashBlock();
  void sha1_initHmac(const uint8_t* key, int keyLength);
  void sha1_writebyte(uint8_t data);
  void sha1_addUncounted(uint8_t data);
  uint8_t* sha1_result();
  void sha1_pad();
  void sha1_write(const byte *data, size_t len);
  uint8_t* sha1_resultHmac();
};
