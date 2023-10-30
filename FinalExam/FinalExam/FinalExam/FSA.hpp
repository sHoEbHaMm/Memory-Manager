#pragma once

#include "BitArray.hpp"
#include "Manager.hpp"

struct FSAInitData
{

	size_t blockSize;
	size_t numBlocks;
};


class FSA
{
public:

	BitArray* array;
	byte* mem;

	size_t sizeBlock;
	size_t blockNums;

	static FSA* CreateFSA( size_t numBlocks, size_t blockSize, HeapManager* hm);
	void* _alloc();
	void _free(void* ptr);
	bool Contains(void* memory);
	void Destroy(HeapManager* hm);
};


inline FSA*
FSA::CreateFSA(size_t numBlocks, size_t blockSize, HeapManager* hm)
{
	FSA* fsa = (FSA*)hm->alloc(sizeof(FSA));
	fsa->mem = (byte*)hm->alloc(numBlocks * blockSize);
	fsa->array = BitArray::Create(numBlocks, false, hm);
	fsa->sizeBlock = blockSize;
	fsa->blockNums = numBlocks;
	return fsa;
}

inline void* FSA::_alloc()
{
	size_t firstAvailable;

	if (array->GetFirstSetBit(firstAvailable))
	{
		array->ClearBit(firstAvailable);

		return mem + (firstAvailable * sizeBlock);
	}
	else
	{
		return nullptr;
	}
}

inline void FSA::_free(void* ptr)
{
	void* temp = nullptr;
	size_t i;
	for (i = 0; i <= blockNums; i++)
	{
		if (ptr != temp)
		{
			temp = mem + sizeBlock;
		}
	}

	array->SetBit(i);
}

inline bool FSA::Contains(void* memory)
{
	void* temp = mem;

	for (size_t i = 0; i < blockNums; i++)
	{
		if (memory != temp)
		{
			temp = mem + sizeBlock;
		}
		else
			return true;
	}
	return false;
}

inline void FSA::Destroy(HeapManager* hm)
{
	hm->free(mem);
	hm->free(array);
}


