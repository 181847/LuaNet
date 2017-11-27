#pragma once
#include "MyTools.h"
#include "MyAssert.h"
//将零号元素的序号起一个别名HEAD。
#define HEAD 0

typedef unsigned int UINT;


/*
template<typename DataType>
struct LinkableUnit
{
union
{
LinkableUnit<DataType>* next;
DataType data;
};

public:
LinkableUnit();
~LinkableUnit();
};

template<typename DataType>
inline LinkableUnit<DataType>::LinkableUnit()
{
}

template<typename DataType>
inline LinkableUnit<DataType>::~LinkableUnit()
{
}
*/





template<typename DataType>
class MyStaticAllocator
{
	
	struct LinkableUnit 
	{
		union 
		{
			LinkableUnit* next;
			DataType data;
		};
	public:
		LinkableUnit() {}
		~LinkableUnit() {}
	};

public:
	MyStaticAllocator(UINT initializeUnitCount);
	MyStaticAllocator(const MyStaticAllocator&) = delete;
	MyStaticAllocator& operator = (const MyStaticAllocator&) = delete;
	~MyStaticAllocator();
	//申请一个DataType类型的指针。
	DataType* Malloc();
	//释放一个DataType类型的指针，放回StaticAllocator中。
	void Free(DataType* data);
	UINT GetTotalCount();
	UINT GetRemainCount();

protected:
	UINT totalCount;
	UINT remainCount;

private:
	//存储了所有可用的LinkableUnit对象的数组，
	//规定0号元素是不可用元素，
	//申请Data指针的时候从0号元素指向的下一个元素开始申请。
	LinkableUnit* memoryLocation;
};

template<typename DataType>
MyStaticAllocator<DataType>::MyStaticAllocator(UINT initializeUnitCount)
{
	ASSERT(sizeof(DataType) > sizeof(void*) && "作为分配池中的内存，指定的类型空间不足，无法作为Union容纳一个指针。");
	remainCount = totalCount = initializeUnitCount;

	//申请初始空间的时候多申请一个，用来表示头结点。
	memoryLocation = 
		new LinkableUnit[totalCount + 1];

	//内存池链接为一个链表。
	for (UINT i = 0; i < totalCount; ++i) 
	{
		memoryLocation[i].next = & memoryLocation[i + 1];
	}

	//链表的末尾标记为null，表示链表结束。
	memoryLocation[totalCount - 1].next = nullptr;
}

template<typename DataType>
MyStaticAllocator<DataType>::~MyStaticAllocator()
{
	delete[] memoryLocation;
}

template<typename DataType>
inline DataType * MyStaticAllocator<DataType>::Malloc()
{
	ASSERT(remainCount >1 && "分配池可用空间为0");
	--remainCount;

	LinkableUnit* returnPointer = memoryLocation[HEAD].next;
	//从链表头结点的后面删除这个要被释放出去的结点,
	//头结点连接到下一个结点上。
	memoryLocation[HEAD].next = returnPointer->next;
	return reinterpret_cast<DataType*>(returnPointer);
}

template<typename DataType>
inline void MyStaticAllocator<DataType>::Free(DataType * data)
{
	LinkableUnit* freeNode = reinterpret_cast<LinkableUnit*>(data);

	//计算这个结点在整个分配池中的序号，这个序号应该大于0，小于totalCount。
	UINT indexLocation = freeNode - memoryLocation;
	ASSERT(indexLocation > 0 && indexLocation < totalCount && "被释放的结点不属于这个分配池");

	//将被释放的结点插入头结点之后。
	freeNode->next = memoryLocation[HEAD].next;
	memoryLocation[HEAD].next = freeNode;

	//剩余空间数量加一。
	++remainCount;
}

template<typename DataType>
inline UINT MyStaticAllocator<DataType>::GetTotalCount()
{
	return totalCount;
}

template<typename DataType>
inline UINT MyStaticAllocator<DataType>::GetRemainCount()
{
	return remainCount;
}
