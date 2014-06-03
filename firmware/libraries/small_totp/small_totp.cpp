#include "small_totp.h"
#include <stdlib.h>


// PUBLIC

small_totp::small_totp(const uint8_t *text_secret, const uint8_t length) {
  sha1_initHmac(&secret, text_secret, length);
}

uint32_t small_totp::code(uint32_t time) {
  byte *Big_hash;
  int Offset;
  uint32_t Truncated_hash;
  sha1nfo temp;
  int i;

  unsigned long Google_time = time / 30;
  byte Google_time_array[8] = { 0x00, 0x00, 0x00, 0x00,
				(byte)((Google_time >> 24) & 0xFF),
				(byte)((Google_time >> 16) & 0xFF),
				(byte)((Google_time >> 8) & 0xFF),
				(byte)(Google_time & 0xFF) };

  copy_sha1_object(&secret, &temp);
  sha1_write(&temp, Google_time_array,8);
  Big_hash = sha1_resultHmac(&temp);

  Offset = Big_hash[20-1] & 0x0F;
  Truncated_hash = 0;
  for (i=0; i < 4; ++i) {
    Truncated_hash <<= 8;
    Truncated_hash |= Big_hash[Offset + i];
  }
  Truncated_hash &= 0x7FFFFFFF;
  Truncated_hash %= 1000000;
  return Truncated_hash;
}

//PRIVATE
void small_totp::copy_sha1_object(sha1nfo *s, sha1nfo *t)
{
  int i = 0;
  for( i = 0; i < BLOCK_LENGTH/4; i++) {
    t->buffer[i] = s->buffer[i];
  }
  for( i = 0; i < HASH_LENGTH/4; i++) {
    t->state[i] = s->state[i];
  }
  t->byteCount = s->byteCount;
  t->bufferOffset = s->bufferOffset;
  for( i = 0; i < BLOCK_LENGTH; i++) {
    t->keyBuffer[i] = s->keyBuffer[i];
  }
  for( i = 0; i < HASH_LENGTH; i++) {
    t->innerHash[i] = s->innerHash[i];
  }
}

void small_totp::sha1_init(struct sha1nfo *s) {
	s->state[0] = 0x67452301;
	s->state[1] = 0xefcdab89;
	s->state[2] = 0x98badcfe;
	s->state[3] = 0x10325476;
	s->state[4] = 0xc3d2e1f0;
	s->byteCount = 0;
	s->bufferOffset = 0;
}

uint32_t small_totp::sha1_rol32(uint32_t number, uint8_t bits) {
	return ((number << bits) | (number >> (32-bits)));
}

void small_totp::sha1_hashBlock(struct sha1nfo *s) {
	uint8_t i;
	uint32_t a,b,c,d,e,t;

	a=s->state[0];
	b=s->state[1];
	c=s->state[2];
	d=s->state[3];
	e=s->state[4];
	for (i=0; i<80; i++) {
		if (i>=16) {
			t = s->buffer[(i+13)&15] ^ s->buffer[(i+8)&15] ^ s->buffer[(i+2)&15] ^ s->buffer[i&15];
			s->buffer[i&15] = sha1_rol32(t,1);
		}
		if (i<20) {
			t = (d ^ (b & (c ^ d))) + SHA1_K0;
		} else if (i<40) {
			t = (b ^ c ^ d) + SHA1_K20;
		} else if (i<60) {
			t = ((b & c) | (d & (b | c))) + SHA1_K40;
		} else {
			t = (b ^ c ^ d) + SHA1_K60;
		}
		t+=sha1_rol32(a,5) + e + s->buffer[i&15];
		e=d;
		d=c;
		c=sha1_rol32(b,30);
		b=a;
		a=t;
	}
	s->state[0] += a;
	s->state[1] += b;
	s->state[2] += c;
	s->state[3] += d;
	s->state[4] += e;
}

void small_totp::sha1_addUncounted(struct sha1nfo *s, uint8_t data) {
	uint8_t * const b = (uint8_t*) s->buffer;

	b[s->bufferOffset ^ 3] = data;

	s->bufferOffset++;
	if (s->bufferOffset == BLOCK_LENGTH) {
		sha1_hashBlock(s);
		s->bufferOffset = 0;
	}
}

void small_totp::sha1_writebyte(struct sha1nfo *s, uint8_t data) {
	++s->byteCount;
	sha1_addUncounted(s, data);
}

void small_totp::sha1_write(struct sha1nfo *s, const byte *data, size_t len) {
	for (;len--;) sha1_writebyte(s, (uint8_t) *data++);
}

void small_totp::sha1_pad(struct sha1nfo *s) {
	// Implement SHA-1 padding (fips180-2 §5.1.1)

	// Pad with 0x80 followed by 0x00 until the end of the block
	sha1_addUncounted(s, 0x80);
	while (s->bufferOffset != 56) sha1_addUncounted(s, 0x00);

	// Append length in the last 8 bytes
	sha1_addUncounted(s, 0); // We're only using 32 bit lengths
	sha1_addUncounted(s, 0); // But SHA-1 supports 64 bit lengths
	sha1_addUncounted(s, 0); // So zero pad the top bits
	sha1_addUncounted(s, s->byteCount >> 29); // Shifting to multiply by 8
	sha1_addUncounted(s, s->byteCount >> 21); // as SHA-1 supports bitstreams as well as
	sha1_addUncounted(s, s->byteCount >> 13); // byte.
	sha1_addUncounted(s, s->byteCount >> 5);
	sha1_addUncounted(s, s->byteCount << 3);
}

uint8_t* small_totp::sha1_result(struct sha1nfo *s) {
	// Pad to complete the last block
	sha1_pad(s);

	// Swap byte order back
	int i;
	for (i=0; i<5; i++) {
		s->state[i]=
			  (((s->state[i])<<24)& 0xff000000)
			| (((s->state[i])<<8) & 0x00ff0000)
			| (((s->state[i])>>8) & 0x0000ff00)
			| (((s->state[i])>>24)& 0x000000ff);
	}

	// Return pointer to hash (20 characters)
	return (uint8_t*) s->state;
}

void small_totp::sha1_initHmac(struct sha1nfo *s, const uint8_t* key, int keyLength) {
	uint8_t i;
	memset(s->keyBuffer, 0, BLOCK_LENGTH);
	if (keyLength > BLOCK_LENGTH) {
		// Hash long keys
		sha1_init(s);
		for (;keyLength--;) sha1_writebyte(s, *key++);
		memcpy(s->keyBuffer, sha1_result(s), HASH_LENGTH);
	} else {
		// Block length keys are used as is
		memcpy(s->keyBuffer, key, keyLength);
	}
	// Start inner hash
	sha1_init(s);
	for (i=0; i<BLOCK_LENGTH; i++) {
		sha1_writebyte(s, s->keyBuffer[i] ^ HMAC_IPAD);
	}
}

uint8_t* small_totp::sha1_resultHmac(struct sha1nfo *s) {
	uint8_t i;
	// Complete inner hash
	memcpy(s->innerHash,sha1_result(s),HASH_LENGTH);
	// Calculate outer hash
	sha1_init(s);
	for (i=0; i<BLOCK_LENGTH; i++) sha1_writebyte(s, s->keyBuffer[i] ^ HMAC_OPAD);
	for (i=0; i<HASH_LENGTH; i++) sha1_writebyte(s, s->innerHash[i]);
	return sha1_result(s);
}
