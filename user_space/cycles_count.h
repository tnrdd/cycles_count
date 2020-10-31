/*
 * SPDX-License-Identifier: LGPL-2.1
 *
 * Copyright (C) 2020 tnrdd
 */

#ifndef CYCLES_COUNT_H
#define CYCLES_COUNT_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define REPETITION_COUNT 100000 
#define HIGH_HALF_BITS 32

struct statistics {
	uint64_t min; 
	uint64_t max;
	uint64_t mean;
        uint64_t variance;
};

#endif /* CYCLES_COUNT_H */
