/*
 * SPDX-License-Identifier: LGPL-2.1
 *
 * Copyright (C) 2020 tnrdd
 */

#include "cycles_count.h"

static inline void count_cycles(uint64_t *cycles)
{ 	
	uint32_t i;
	uint64_t start_low;
	uint64_t start_high;
	uint64_t end_low;
	uint64_t end_high;

	asm volatile ("rdtscp\n\t"
	     	      "lfence\n\t"		
		      : "=d" (start_high), "=a" (start_low)
		      :
		      : "%rbx", "%rcx");

	asm volatile ("rdtscp\n\t"
		      "lfence\n\t"
		      : "=d" (end_high), "=a" (end_low)
		      :
		      : "%rbx", "%rcx");
	
	for (i = 0; i < REPETITION_COUNT; i++) {        
		asm volatile ("rdtscp\n\t"
			      "lfence\n\t"
			      : "=d" (start_high), "=a" (start_low)
			      :
		              : "%rbx", "%rcx");

	/*
         **************************************************
         *Function to benchmark here
         **************************************************
         */

		asm volatile ("rdtscp\n\t"
		  	      "lfence\n\t"		
			      : "=d" (end_high), "=a" (end_low)
			      :
			      : "%rbx", "%rcx");
		
		cycles[i] = ((end_high - start_high) << LOWER_HALF_BITS) + (end_low  - start_low); 
	}
}

uint64_t calc_mean(uint64_t *cycles, uint32_t occurrencies)
{
	uint32_t i;
	uint64_t sum;

	sum = 0;
	
	for (i = 0; i < occurrencies; i++) {
		sum += cycles[i];
	}

	return sum / occurrencies;
}

uint64_t calc_variance(uint64_t *cycles, uint64_t mean, uint32_t occurrencies) 
{ 
	uint32_t i; 
	uint64_t variance;
	
	variance = 0; 
	
	for (i = 0; i < occurrencies; i++) { 
		variance += (cycles[i] - mean) * (cycles[i] - mean);
	} 

	variance /= (occurrencies - 1);
	return variance;
}

int main(int argc, char **argv) 
{ 
	uint32_t i;
	struct statistics stats = {0};
	uint64_t *cycles;

	cycles = malloc(REPETITION_COUNT * sizeof(uint64_t));

	if (!cycles) {
		return -1;
	}

	count_cycles(cycles);
	
	stats.min = cycles[0];
	stats.max = cycles[0];
	
	for (i = 1; i < REPETITION_COUNT; i++) { 

		if (cycles[i] < stats.min) {
			stats.min = cycles[i]; 
		
		} else if (cycles[i] > stats.max) { 
			stats.max = cycles[i]; 
		} 
	}
	
	stats.mean = calc_mean(cycles, REPETITION_COUNT);
	stats.var = calc_variance(cycles, stats.mean, REPETITION_COUNT);
	
	printf("min: %lu cycles\n", stats.min); 
	printf("max: %lu cycles\n", stats.max); 
	printf("mean: %lu cycles\n", stats.mean); 
	printf("variance: %lu cycles\n", stats.variance); 
	printf("standard deviation: %.3g cycles\n", __builtin_sqrtf(stats.variance));

	free(cycles);
	return EXIT_SUCCESS;
} 
