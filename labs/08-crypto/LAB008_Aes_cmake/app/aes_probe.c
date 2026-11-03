#include "aes_probe.h"
#include "mx_aes.h"
#include <string.h>

/* PUBLIC NIST ECB-AES128 example key and vectors. These are not device secrets.
 * Source: NIST AES_ModesA_All.pdf, ECB-AES128 Encryption/Decryption sections.
 * Words contain canonical big-endian numeric values; data swapping is disabled.
 */
static const uint32_t test_key[4] = {0x2B7E1516,0x28AED2A6,0xABF71588,0x09CF4F3C};
static const uint32_t plaintext[16] = {
  0x6BC1BEE2,0x2E409F96,0xE93D7E11,0x7393172A,
  0xAE2D8A57,0x1E03AC9C,0x9EB76FAC,0x45AF8E51,
  0x30C81C46,0xA35CE411,0xE5FBC119,0x1A0A52EF,
  0xF69F2445,0xDF4F9B17,0xAD2B417B,0xE66C3710
};
static const uint32_t ciphertext[16] = {
  0x3AD77BB4,0x0D7A3660,0xA89ECAF3,0x2466EF97,
  0xF5D3D585,0x03B9699D,0xE785895A,0x96FDBAAF,
  0x43B1CD7F,0x598ECE23,0x881B00E3,0xED030688,
  0x7B0C785E,0x27E8AD3F,0x82232071,0x04725DD4
};
#define GUARD 0xC5A55AC5u
typedef struct { uint32_t before, words[16], after; } guarded_output;
static guarded_output encrypted, decrypted, wrong_key_output;
static uint32_t wrong_key[4];

static int configure(const uint32_t *key)
{
  hal_aes_handle_t *h = mx_aes_gethandle();
  return HAL_AES_ECB_SetConfig(h) == HAL_OK &&
         HAL_AES_SetDataSwapping(h, HAL_AES_DATA_SWAPPING_NO) == HAL_OK &&
         HAL_AES_SetNormalKey(h, HAL_AES_KEY_SIZE_128BIT, key) == HAL_OK;
}

int aes_probe_run(aes_probe_result *r)
{
  *r = (aes_probe_result){0};
  memset(&encrypted, 0, sizeof(encrypted));
  memset(&decrypted, 0, sizeof(decrypted));
  memset(&wrong_key_output, 0, sizeof(wrong_key_output));
  encrypted.before = encrypted.after = decrypted.before = decrypted.after =
    wrong_key_output.before = wrong_key_output.after = GUARD;
  hal_aes_handle_t *h = mx_aes_gethandle();
  if (!configure(test_key)) { return -1; }
  if (HAL_AES_Encrypt(h, plaintext, sizeof(plaintext), encrypted.words, 100) != HAL_OK) { return -2; }
  if (!configure(test_key)) { return -3; }
  /* Decrypt the independent reference, not the just-computed output. */
  if (HAL_AES_Decrypt(h, ciphertext, sizeof(ciphertext), decrypted.words, 100) != HAL_OK) { return -4; }
  memcpy(wrong_key, test_key, sizeof(wrong_key));
  wrong_key[0] ^= 1u;
  if (!configure(wrong_key)) { return -5; }
  if (HAL_AES_Decrypt(h, ciphertext, sizeof(ciphertext), wrong_key_output.words, 100) != HAL_OK) { return -6; }
  for (unsigned i=0; i<4; ++i) {
    r->encrypt_blocks += memcmp(encrypted.words+4*i, ciphertext+4*i, 16) == 0;
    r->decrypt_blocks += memcmp(decrypted.words+4*i, plaintext+4*i, 16) == 0;
  }
  r->wrong_key_diff = memcmp(wrong_key_output.words, plaintext, sizeof(plaintext)) != 0;
  r->guards = (encrypted.before == GUARD) + (encrypted.after == GUARD) +
    (decrypted.before == GUARD) + (decrypted.after == GUARD) +
    (wrong_key_output.before == GUARD) + (wrong_key_output.after == GUARD);
  memcpy(r->first_cipher, encrypted.words, sizeof(r->first_cipher));
  return r->encrypt_blocks == 4 && r->decrypt_blocks == 4 && r->wrong_key_diff && r->guards == 6 ? 0 : -7;
}
