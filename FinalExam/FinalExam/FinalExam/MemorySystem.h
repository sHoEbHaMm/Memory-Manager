#pragma once

#include "FSA.hpp"

class MemorySystem
{
public:
	static HeapManager* hm;

	static FSA** allocators;


	static bool Init(void* i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors);
	static void destroyMemorySystem();

	static void* my_alloc(size_t size);
	static void my_free(void* ptr);
	static void collect();
	static FSA* FindFSA(size_t size);
};

bool InitializeMemorySystem(void* pHeapMemory, size_t sizeHeap, unsigned int numDescriptors);
void DestroyMemorySystem();
void Collect();

void* __cdecl _malloc(size_t i_size);
void __cdecl free_(void* i_ptr);
