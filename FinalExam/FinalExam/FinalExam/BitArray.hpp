#include<stdint.h>
#include <intrin.h>
#include<cstring>
#include<math.h>
#include "Manager.hpp"

#ifdef _WIN32
typedef uint32_t bitData;
#define ScanForward(index, mask) _BitScanForward(index, mask)
#else
typedef uint64_t bitData;
#define ScanForward(index, mask) _BitScanForward64(index, mask)
#endif // _WIN64


class BitArray
{
public:

//mem variables
	bitData* bits;
	static const size_t bitsPerElement = sizeof(bitData) * 8;
	size_t sizeArray;

//mem functions
	static BitArray* Create(size_t numBits, bool InitToZero, HeapManager* hm);
	bool isBitSet(size_t bitNumber) const;
	void SetBit(size_t bitNumber);
	void ClearBit(size_t bitNumber);
	bool GetFirstSetBit(size_t& bitNumber) const;
	void SetAllBits();
	void ClearAllBits();
	void Destroy(HeapManager* hm);
};

inline BitArray*
BitArray::Create(size_t numBits, bool InitToZero, HeapManager* hm)
{
	BitArray* bitArray = (BitArray*)(hm->alloc(sizeof(BitArray)));

	bitArray->bits = (bitData*)(hm->alloc((ceil(numBits / (bitsPerElement))) * sizeof(bitData)));
	bitArray->sizeArray = (ceil(numBits / bitsPerElement));
	memset(bitArray->bits, InitToZero ? 0 : 1, numBits / bitsPerElement);

	return bitArray;
}

inline
bool BitArray::isBitSet(size_t bitNumber) const
{
	size_t arrayIndex;
	size_t bitIndex;

	arrayIndex = bitNumber / bitsPerElement;
	bitIndex = bitNumber % bitsPerElement;

	bitData Bit = bits[arrayIndex];
	Bit = Bit >> (bitsPerElement - bitIndex - 1);

	return Bit;
}

inline void BitArray::SetBit(size_t bitNumber)
{
	size_t arrayIndex;
	size_t bitIndex;

	arrayIndex = bitNumber / bitsPerElement;
	bitIndex = bitNumber % bitsPerElement;

	bitData Bit = bits[arrayIndex];

	Bit |= (1 << (bitIndex));

	bits[arrayIndex] = Bit;
}

inline void BitArray::ClearBit(size_t bitNumber)
{
	size_t arrayIndex;
	size_t bitIndex;

	arrayIndex = bitNumber / bitsPerElement;
	bitIndex = bitNumber % bitsPerElement;

	bitData Bit = bits[arrayIndex];

	Bit &= ~(1 << (bitIndex));

	bits[arrayIndex] = Bit;
}


inline bool BitArray::GetFirstSetBit(size_t& bitNumber) const
{
	size_t arrayIndex = 0;

	bitData data = bits[arrayIndex];

	while (data == 0 && arrayIndex < sizeArray - 1 )
	{
		 data = bits[arrayIndex];
		 arrayIndex++;
	}

	unsigned long mask = data;
	unsigned long index;

	
	if (!ScanForward(&index, mask))
		return false;

	bitNumber = (bitsPerElement * arrayIndex) + index;

		return true;
	
}

inline void BitArray::SetAllBits() //set all bits to 1
{
	size_t arrayIndex = 0;
	bitData size = sizeof(BitArray) / sizeof(bits[arrayIndex]);

	memset(bits, 1, size);
}

inline void BitArray::ClearAllBits()//all bits to 0
{
	size_t arrayIndex = 0;
	bitData size = sizeof(BitArray) / sizeof(bits[arrayIndex]);

	memset(bits, 0, size);
}

inline void BitArray::Destroy(HeapManager* hm)
{
	hm->free(bits);
}
