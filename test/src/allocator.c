#include "allocator.h"

struct MemAllocator allocator_init(size_t bytes) {
	/* global reference counter */
	malloc_ref_counter++;
	struct MemAllocator allocator = {0};
	
	allocator.mem = malloc(bytes);

	/* only set fields if malloc was successful */
	if(allocator.mem) {
		allocator.size = bytes;
		allocator.used = 0;
		allocator.base = allocator.mem;
	}
	
	return allocator;
}

void *mem_alloc(struct MemAllocator *allocator, size_t amount) {
	void *result = 0;

	if(amount <= 0) {
		return NULL;
	}
	
	if(amount > allocator->size - allocator->used) {
		/*
		 * When there is not enough memory we need to realloc the memory of the allocator.
		 * But instead of just reallocating enough memory for this allocation
		 * we just realloc a bigger block. 
		 * Otherwise we end up with alot of small allocations again.
		 */
		size_t realloc_size = amount * 2;
		void *realloc_result = realloc(allocator->mem, realloc_size);

		/* 
		 * NOTE: realloc returns null pointer if an error occured,
		 * so before assigning the return value check if the pointer is not a null pointer 
		 */
		if(realloc_result) {
			allocator->mem = realloc_result;
			allocator->base = (uint8_t *)allocator->mem + allocator->used;
			allocator->size += realloc_size;
		}
	}
	/* pointer to new allocated memory */
	result = allocator->base;
	allocator->base += amount;
	allocator->used += amount;

	return result;
}

void print_allocator_info(struct MemAllocator *allocator) {
	printf("allocator info: \n");
	printf("current base address: 0x%p\n", allocator->base);
	printf("size: %zd\n", allocator->size);
	printf("used: %zd\n", allocator->used);
}
