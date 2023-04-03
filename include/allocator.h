#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int malloc_ref_counter;

struct MemAllocator
{
	void *mem;
	uint8_t *base;
	size_t size;
	size_t used;
};

struct MemAllocator allocator_init(size_t bytes);

void *mem_alloc(struct MemAllocator *allocator, size_t amount);

void print_allocator_info(struct MemAllocator *allocator);

#endif	
