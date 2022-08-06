#pragma once

#include <cstdint>

class CUtlMemoryPool
{
public:
	int Count( ) const
	{
		return m_nBlocksAllocated;
	}

	int PeakCount( ) const
	{
		return m_nPeakAlloc;
	}

	int BlockSize( ) const
	{
		return m_nBlockSize;
	}
protected:
	class CBlob
	{
	public:
		CBlob* m_pPrev;
		CBlob* m_pNext;
		int m_nNumBytes;
		char m_Data[ 1 ];
		char m_Padding[ 3 ];
	};

	int m_nBlockSize;
	int m_nBlocksPerBlob;
	int m_nGrowMode;
	int m_nBlocksAllocated;
	int m_nPeakAlloc;

	unsigned short m_nAlignment;
	unsigned short m_nNumBlobs;

	void* m_pHeadOfFreeList;
	void* m_pAllocOwner;
	CBlob m_BlobHead;
};