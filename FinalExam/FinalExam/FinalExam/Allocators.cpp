#pragma once

#include <inttypes.h>

#include <stdio.h>

#include "MemorySystem.h"

void * __cdecl _malloc(size_t i_size)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	return MemorySystem::my_alloc(i_size);
}

void __cdecl free_(void * i_ptr)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	MemorySystem::my_free(i_ptr);
}

void * operator new(size_t i_size)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	return MemorySystem::my_alloc(i_size);
}

void operator delete(void * i_ptr)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	MemorySystem::my_free(i_ptr);
}

void * operator new[](size_t i_size)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	return MemorySystem::my_alloc(i_size);
}

void operator delete [](void * i_ptr)
{
	// replace with calls to your HeapManager or FixedSizeAllocators
	MemorySystem::my_free(i_ptr);
}