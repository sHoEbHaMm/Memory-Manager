#include<Windows.h>


class HeapManager
{
	struct mem_block {
		void* mem_addr;
		size_t size;
		mem_block* nextBlock;
		mem_block* prev;
		bool isFree;
	};

	byte* endLocation;
	mem_block* FreeBlocks;
	mem_block* outBlocks;
public:

	void* alloc(size_t size);
	void* _alloc(size_t size, unsigned int numDes);
	bool IsAllocated(void* memory);
	bool Contains(void* memory);
	bool free(void* ptr);
	void Collect();
	void destroy();
	size_t getLargestFreeBlock();
	size_t getTotalFreeMemory();
	void ShowFreeBlocks();

	static HeapManager* CreateHeapManager(void* memory, size_t size, unsigned int numDes);
};

HeapManager*
HeapManager::CreateHeapManager(void* memory, size_t size, unsigned int numDes)
{
	HeapManager* hm;
	byte* buff;

	buff = (byte*)memory; //pointing to the first byte of given memory
	hm = *((HeapManager**)&buff); //accessing the first few bytes of heapmemory
	size -= sizeof(HeapManager); //reducing the size of available memory because heapmanager consumed it

	buff += sizeof(hm); //moving the buff* ahead
	hm->endLocation = buff + size;

	hm->FreeBlocks = (mem_block*)buff; //creates a memBlock to store freeblock members
	size -= sizeof(mem_block); //reduced size of available heap because freeblocks consumed it
	buff += sizeof(mem_block); //moved buff by size of freeblock which is mem_block

	hm->FreeBlocks->mem_addr = buff; //now the freelist starts from buff
	hm->FreeBlocks->size = size;//rest of the available memory is for freeblocks
	hm->FreeBlocks->nextBlock = nullptr;//ofcourse
	hm->FreeBlocks->prev = nullptr;//ofcourse

	hm->outBlocks = nullptr;//no allocations yet

	return hm;
}

void*
HeapManager::alloc(size_t size)
{
	mem_block* freeBlock; //used to traverse the LL
	mem_block* newBlock;

	freeBlock = FreeBlocks;

	while (freeBlock)
	{
		if (freeBlock->size >= size)
		{
			break;
		}
		freeBlock = freeBlock->nextBlock;
	}
	if (freeBlock == nullptr)
	{
		return nullptr;
	}

	newBlock = freeBlock;

	if (newBlock)
	{
		newBlock->size = size;
		freeBlock->size -= size + sizeof(mem_block);
		freeBlock->mem_addr = (byte*)freeBlock->mem_addr + sizeof(mem_block);
	}

	outBlocks = freeBlock;

	return newBlock->mem_addr;
}

inline void* HeapManager::_alloc(size_t size, unsigned int numDes)
{
	//Implement
	return nullptr;
}

inline bool
HeapManager::IsAllocated(void* memory)
{
	mem_block* freeList;
	mem_block* outList;
	byte* buff;
	mem_block* uBlock;

	freeList = FreeBlocks;
	outList = outBlocks;

	buff = (byte*)memory;
	buff -= sizeof(mem_block);
	uBlock = (mem_block*)buff;
	
	while (outList)
	{
		if (outList == uBlock)
		{
			return true;
		}
		outList = outList->nextBlock;
	}

	return false;
}

inline bool HeapManager::Contains(void* memory)
{
	mem_block* Free;
	mem_block* Out;
	byte* buffer;
	mem_block* testBlock;


	buffer = (byte*)memory;
	buffer = buffer - sizeof(mem_block);
	testBlock = (mem_block*)buffer;

	Free = FreeBlocks;
	Out = outBlocks;

	//Search free blocks
	while (Free)
	{
		if (Free == testBlock)
		{
			return true;
		}
		Free = Free->nextBlock;
	}
	
	//Search outstanding blocks
	while (Out)
	{
		if (Out == testBlock)
		{
			return true;
		}
		Out = Out->nextBlock;
	}

	//Lol!
	return false;
}

inline bool HeapManager::free(void* ptr)
{
	mem_block* outBlock;
	mem_block* uBlock;
	byte* buff;

	buff = (byte*)ptr;
	buff -= sizeof(mem_block);
	uBlock = (mem_block*)buff;

	outBlock = outBlocks;

	while (outBlock)
	{
		if (outBlock == uBlock)
		{
			if (outBlock->prev) //When already few allocations are made
			{
				outBlock->prev->nextBlock = outBlock->nextBlock;
			}
			else
			{
				outBlocks = outBlock->nextBlock;
			}
			
			if (outBlock->nextBlock)
			{
				outBlock->nextBlock->prev = outBlock->prev;
			}

			outBlock->nextBlock = FreeBlocks;
			outBlock->prev = nullptr;
			outBlock->isFree = true;
			FreeBlocks->prev = outBlock;
			FreeBlocks = outBlock;

			return true;
		}
		outBlock = outBlock->nextBlock;
	}

	return false;
}

inline void HeapManager::Collect()
{
	mem_block* freeBlock;
	mem_block* nextBlock;
	byte* nextBlockLocation;

	freeBlock = FreeBlocks;

	while (freeBlock)
	{
		nextBlockLocation = (byte*)freeBlock->mem_addr + freeBlock->size + sizeof(mem_block);
		nextBlock = *(mem_block**)(&nextBlockLocation);

		if ((nextBlock->isFree) && nextBlockLocation < endLocation)
		{
			freeBlock->size = nextBlock->size + sizeof(mem_block);

			if (nextBlock->prev)
			{
				nextBlock->prev->nextBlock = nextBlock->nextBlock;
			}

			if (nextBlock->nextBlock)
			{
				nextBlock->nextBlock->prev = nextBlock->prev;
			}
		}
		freeBlock = freeBlock->nextBlock;
	}
}

inline void HeapManager::destroy()
{
}

inline size_t HeapManager::getLargestFreeBlock()
{
	return size_t();
}

inline size_t HeapManager::getTotalFreeMemory()
{
	return size_t();
}

inline void HeapManager::ShowFreeBlocks()
{

}
