
#include "MemorySystem.h"

FSAInitData myData[3] = { {16, 100}, {32, 200}, {96, 400} };

HeapManager* MemorySystem::hm = nullptr;
FSA** MemorySystem::allocators = nullptr;

bool MemorySystem::Init(void* i_pHeapMemory, size_t i_sizeHeapMemory, unsigned int i_OptionalNumDescriptors)
{
	FSA* fsa;

	hm = hm->CreateHeapManager(i_pHeapMemory, i_sizeHeapMemory, i_OptionalNumDescriptors);
	allocators = (FSA**)hm->alloc(3 * sizeof(FSA*));

	for (int i = 0; i < 3; i++)
	{
		fsa = fsa->CreateFSA(myData[i].numBlocks, myData[i].blockSize, hm);

		allocators[i] = fsa;
	}
	return true;
}

void* MemorySystem::my_alloc(size_t size)
{
	FSA* pFSA;

	pFSA = FindFSA(size);
	void* pReturn = nullptr;

	if (pFSA)
	{
		pReturn = pFSA->_alloc();
	}
	
	if (pFSA == nullptr)
	{
		pReturn = hm->alloc(size);
	}

	return pReturn;
}

void MemorySystem::my_free(void* ptr)
{
	size_t FSAs = 3;

	for (size_t i = 0; i < FSAs; i++)
	{
		if (allocators[i]->Contains(ptr))
		{
			allocators[i]->_free(ptr);
			allocators[i]->array->SetBit(i);
			return;
		}
		else if (hm->Contains(ptr))
		{
			hm->free(ptr);
			return;
		}
	}
}

FSA* MemorySystem::FindFSA(size_t size)
{
	if (size <= 16)
	{
		return allocators[0];
	}
	else if (size > 16 && size < 33)
	{
		return allocators[1];
	}
	else if (size > 32 && size < 97)
	{
		return allocators[2];
	}
	return nullptr;
}

bool InitializeMemorySystem(void* pHeapMemory, size_t sizeHeap, unsigned int numDescriptors)
{
	return MemorySystem::Init(pHeapMemory, sizeHeap, numDescriptors);
}



void MemorySystem::destroyMemorySystem()
{
	size_t FSAs = sizeof(FSA**) / sizeof(allocators[0]);
	for (size_t i = 0; i < FSAs; i++)
	{
		allocators[i]->Destroy(hm);
		hm->free(allocators[i]);
	}
}

void MemorySystem::collect()
{
	hm->Collect();
}

void DestroyMemorySystem()
{
	MemorySystem::destroyMemorySystem();
}
void Collect()
{
	MemorySystem::collect();
}
