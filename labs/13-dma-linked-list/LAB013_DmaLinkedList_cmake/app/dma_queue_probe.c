#include "dma_queue_probe.h"
#include "mx_lpdma1_ch0.h"

#define WORDS 32u
#define GUARD 0xC5C5A55Au
typedef struct { uint32_t before, data[WORDS], after; } guarded_buffer;
static uint32_t source_a[WORDS], source_b[WORDS];
static guarded_buffer temporary, destination_b, destination_c;
static hal_dma_node_t nodes[3] __attribute__((aligned(32)));
static hal_q_t queue;
static volatile uint32_t completed, error;

void HAL_DMA_XferCpltCallback(hal_dma_handle_t *hdma)
{
  if (hdma == mx_lpdma1_ch0_gethandle()) { ++completed; }
}
void HAL_DMA_XferErrorCallback(hal_dma_handle_t *hdma)
{
  if (hdma == mx_lpdma1_ch0_gethandle()) { error = 1; }
}

int dma_queue_probe_init(void)
{
  /* Link addresses share one high-address window, with word-aligned nodes. */
  if (((uintptr_t)&nodes[0] >> 16) != (((uintptr_t)(nodes+3)-1u) >> 16)) { return -1; }
  if (HAL_Q_Init(&queue, &HAL_DMA_LinearAddressing_DescOps) != HAL_OK) { return -2; }
  hal_dma_direct_xfer_config_t config = {0};
  config.request = HAL_DMA_REQUEST_SW;
  config.direction = HAL_DMA_DIRECTION_MEMORY_TO_MEMORY;
  config.src_inc = HAL_DMA_SRC_ADDR_INCREMENTED;
  config.dest_inc = HAL_DMA_DEST_ADDR_INCREMENTED;
  config.src_data_width = HAL_DMA_SRC_DATA_WIDTH_WORD;
  config.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_WORD;
  config.priority = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;
  uint32_t *src[3] = {source_a, source_b, temporary.data};
  uint32_t *dst[3] = {temporary.data, destination_b.data, destination_c.data};
  for (unsigned i=0; i<3; ++i) {
    if (HAL_DMA_FillNodeDirectXfer(&nodes[i], &config, HAL_DMA_NODE_LINEAR_ADDRESSING) != HAL_OK ||
        HAL_DMA_FillNodeData(&nodes[i], (uint32_t)src[i], (uint32_t)dst[i], sizeof(source_a)) != HAL_OK ||
        HAL_Q_InsertNode_Tail(&queue, &nodes[i]) != HAL_OK) { return -3; }
  }
  return 0;
}

int dma_queue_probe_run(uint32_t sequence, dma_queue_result *r)
{
  *r = (dma_queue_result){0};
  for (unsigned i=0; i<WORDS; ++i) {
    source_a[i] = 0xA5000000u ^ (sequence << 8) ^ i;
    source_b[i] = 0x5A000000u ^ (sequence << 8) ^ (i*3u);
    temporary.data[i] = destination_b.data[i] = destination_c.data[i] = 0xFFFFFFFFu;
  }
  temporary.before = temporary.after = destination_b.before = destination_b.after =
    destination_c.before = destination_c.after = GUARD;
  completed = error = 0;
  __DMB();
  if (HAL_DMA_StartLinkedListXfer_IT(mx_lpdma1_ch0_gethandle(), &queue) != HAL_OK) { return -1; }
  uint32_t start = HAL_GetTick();
  while (!completed && !error && HAL_GetTick()-start < 100) { __WFI(); }
  if (!completed || error) {
    (void)HAL_DMA_Abort(mx_lpdma1_ch0_gethandle());
    return error ? -2 : -3;
  }
  __DMB();
  r->completions = completed;
  for (unsigned i=0; i<WORDS; ++i) {
    r->words_ok += temporary.data[i] == source_a[i];
    r->words_ok += destination_b.data[i] == source_b[i];
    r->words_ok += destination_c.data[i] == source_a[i];
  }
  r->guards_ok = (temporary.before == GUARD) + (temporary.after == GUARD) +
    (destination_b.before == GUARD) + (destination_b.after == GUARD) +
    (destination_c.before == GUARD) + (destination_c.after == GUARD);
  return r->words_ok == 96 && r->guards_ok == 6 && r->completions == 1 ? 0 : -4;
}
