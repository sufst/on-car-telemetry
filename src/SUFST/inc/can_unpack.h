#include "tx_api.h"
#include "telemetry_protocol.h"

#define QUEUE_SIZE 100

TX_QUEUE queue_spi;
static ULONG queue_memory_area[QUEUE_SIZE * sizeof(pdu_t)];