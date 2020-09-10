/*
 * SPDX-License-Identifier: LGPL-2.1
 *
 * Copyright (C) 2020 tnrdd
 */

#ifndef CYCLES_COUNT_H
#define CYCLES_COUNT_H

#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/init.h> 
#include <linux/hardirq.h> 
#include <linux/preempt.h> 
#include <linux/sched.h> 
#include <linux/slab.h>

#define REPETITION_COUNT 100000
#define LOWER_HALF_BITS 32

struct statistics {
	uint64_t min; 
	uint64_t max;
	uint64_t mean;
        uint64_t var;
};

static inline void count_cycles(uint64_t *cycles)
{ 	
	uint32_t i;
	unsigned long flags;
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
		preempt_disable();       
		raw_local_irq_save(flags);
      
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

		raw_local_irq_restore(flags);        
		preempt_enable();
		
		cycles[i] = ((end_high - start_high) << LOWER_HALF_BITS) + (end_low  - start_low); 
	}
}

#endif /* CYCLES_COUNT_H */
