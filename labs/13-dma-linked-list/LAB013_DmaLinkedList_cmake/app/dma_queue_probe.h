#ifndef DMA_QUEUE_PROBE_H
#define DMA_QUEUE_PROBE_H
#include <stdint.h>
typedef struct { uint32_t words_ok, guards_ok, completions; } dma_queue_result;
int dma_queue_probe_init(void);
int dma_queue_probe_run(uint32_t sequence, dma_queue_result *result);
#endif
