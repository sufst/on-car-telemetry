/***************************************************************************
 * @file   testbench_can_data.h
 * @author Ryuta Itabashi (@h2so4, ri1g22@soton.ac.uk)
 * @date   2023-03-07
 * @brief  Dummy data lookup table for simulating CAN bus communication
 ***************************************************************************/

#include <stdint.h>

#include "config.h"

#ifndef TESTBENCH_CAN_DATA_H
#define TESTBENCH_CAN_DATA_H

#if CAN_DEBUG_MODE == 1
extern const uint8_t debug_lookup[128];
#endif

#endif