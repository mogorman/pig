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
  small_totp(const uint8_t *text_secret, const uint8_t length);
  uint32_t code(uint32_t time);
 
 private:
  sha1nfo secret;

  void sha1_init(sha1nfo *s);
  uint32_t sha1_rol32(uint32_t number, uint8_t bits);
  void sha1_hashBlock(sha1nfo *s);
  void sha1_initHmac(sha1nfo *s, const uint8_t* key, int keyLength);
  void sha1_writebyte(sha1nfo *s, uint8_t data);
  void sha1_addUncounted(sha1nfo *s, uint8_t data);
  uint8_t* sha1_result(sha1nfo *s);
  void sha1_pad(sha1nfo *s);
  void sha1_write(sha1nfo *s, const byte *data, size_t len);
  uint8_t* sha1_resultHmac(sha1nfo *s);
  void copy_sha1_object(sha1nfo *s, sha1nfo *t);
};
