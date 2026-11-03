#ifndef AES_PROBE_H
#define AES_PROBE_H
#include <stdint.h>
typedef struct {
  uint32_t encrypt_blocks, decrypt_blocks, wrong_key_diff, guards;
  uint32_t first_cipher[4];
} aes_probe_result;
int aes_probe_run(aes_probe_result *result);
#endif
