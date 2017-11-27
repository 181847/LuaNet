#pragma once
#include "MyTools.h"
#include "MyAssert.h"
//�����Ԫ�ص������һ������HEAD��
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
	//����һ��DataType���͵�ָ�롣
	DataType* Malloc();
	//�ͷ�һ��DataType���͵�ָ�룬�Ż�StaticAllocator�С�
	void Free(DataType* data);
	UINT GetTotalCount();
	UINT GetRemainCount();

protected:
	UINT totalCount;
	UINT remainCount;

private:
	//�洢�����п��õ�LinkableUnit��������飬
	//�涨0��Ԫ���ǲ�����Ԫ�أ�
	//����Dataָ���ʱ���0��Ԫ��ָ�����һ��Ԫ�ؿ�ʼ���롣
	LinkableUnit* memoryLocation;
};

template<typename DataType>
MyStaticAllocator<DataType>::MyStaticAllocator(UINT initializeUnitCount)
{
	ASSERT(sizeof(DataType) > sizeof(void*) && "��Ϊ������е��ڴ棬ָ�������Ϳռ䲻�㣬�޷���ΪUnion����һ��ָ�롣");
	remainCount = totalCount = initializeUnitCount;

	//�����ʼ�ռ��ʱ�������һ����������ʾͷ��㡣
	memoryLocation = 
		new LinkableUnit[totalCount + 1];

	//�ڴ������Ϊһ������
	for (UINT i = 0; i < totalCount; ++i) 
	{
		memoryLocation[i].next = & memoryLocation[i + 1];
	}

	//�����ĩβ���Ϊnull����ʾ���������
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
	ASSERT(remainCount >1 && "����ؿ��ÿռ�Ϊ0");
	--remainCount;

	LinkableUnit* returnPointer = memoryLocation[HEAD].next;
	//������ͷ���ĺ���ɾ�����Ҫ���ͷų�ȥ�Ľ��,
	//ͷ������ӵ���һ������ϡ�
	memoryLocation[HEAD].next = returnPointer->next;
	return reinterpret_cast<DataType*>(returnPointer);
}

template<typename DataType>
inline void MyStaticAllocator<DataType>::Free(DataType * data)
{
	LinkableUnit* freeNode = reinterpret_cast<LinkableUnit*>(data);

	//����������������������е���ţ�������Ӧ�ô���0��С��totalCount��
	UINT indexLocation = freeNode - memoryLocation;
	ASSERT(indexLocation > 0 && indexLocation < totalCount && "���ͷŵĽ�㲻������������");

	//�����ͷŵĽ�����ͷ���֮��
	freeNode->next = memoryLocation[HEAD].next;
	memoryLocation[HEAD].next = freeNode;

	//ʣ��ռ�������һ��
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
