/***************************************************************************
 * @file   testbench_can_data.h
 * @author Ryuta Itabashi (@h2so4, ri1g22@soton.ac.uk)
 * @date   2023-03-07
 * @brief  Dummy data lookup table for simulating CAN bus communication
 ***************************************************************************/

#ifndef TESTBENCH_CAN_DATA_H
#define TESTBENCH_CAN_DATA_H

#include <stdint.h>
#include "config.h"

#define DEBUG_LOOKUP_SIZE 128
#define DEBUG_LOOKUP_DATA_CELL_SIZE 8

const uint8_t testbench_can_get_data(uint32_t index);
const uint8_t* testbench_can_get_data_ptr(uint32_t index);

#endif