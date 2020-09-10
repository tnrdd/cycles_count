/*
 * SPDX-License-Identifier: LGPL-2.1
 *
 * Copyright (C) 2020 tnrdd
 */

#include "cycles_count.h"

uint64_t calc_mean(uint64_t *cycles, int occurrencies)
{
	uint32_t i;
	uint64_t sum;

	sum = 0;
	
	for (i = 0; i < occurrencies; i++) {
		sum += cycles[i];
	}

	return sum / occurrencies;
}

uint64_t calc_var(uint64_t *cycles, uint64_t mean, uint32_t occurrencies) 
{ 
	uint32_t i; 
	uint64_t variance = 0; 
	
	for (i = 0; i < occurrencies; i++) { 
		variance += (cycles[i] - mean) * (cycles[i] - mean);
	} 

	variance /= (occurrencies - 1);
	return variance;
}

static int __init cycles_count_start(void)
{
	uint32_t i;
	struct statistics stats = {0};
	uint64_t *cycles;

	cycles = kmalloc(REPETITION_COUNT * sizeof(uint64_t), GFP_KERNEL);

	if (!cycles) {
		return -1;
	}

	count_cycles(cycles); 
	
	for (i = 0; i < REPETITION_COUNT; i++) { 

		if ((stats.min > cycles[i]) || (stats.min == 0)) {
			stats.min = cycles[i]; 
		}

		if (stats.max < cycles[i]) { 
			stats.max = cycles[i]; 
		} 
	}
	
	stats.mean = calc_mean(cycles, REPETITION_COUNT);
	stats.var = calc_var(cycles, stats.mean, REPETITION_COUNT);
	
	printk("min: %llu cycles\n", stats.min); 
	printk("max: %llu cycles\n", stats.max); 
	printk("mean: %llu cycles\n", stats.mean); 
	printk("variance: %llu cycles\n", stats.var);
	
	kfree(cycles);
	return 0;
}

static void __exit cycles_count_end(void)
{
	printk("Exited\n");
}

module_init(cycles_count_start);
module_exit(cycles_count_end);
MODULE_LICENSE("GPL v2");
