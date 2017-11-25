#pragma once
#include <memory>
//开启Debug模式，当MyStaticAllocator空间不足时，会抛出SimpleException。
#define ASSERTIONS_ENABLED 1
#include "MyAssert.h"
#include "MyStaticAllocator.h"

template<typename ELEMENT>
struct DeLinkedElement
{
	DeLinkedElement<ELEMENT>* m_pPrev;
	DeLinkedElement<ELEMENT>* m_pNext;
	ELEMENT element;
};

template<typename ELEMENT>
class DeLinkedList
{

public:
	DeLinkedList(UINT allocatorSize);
	DeLinkedList(const DeLinkedList<ELEMENT>&) = delete;
	DeLinkedList<ELEMENT>& operator = (const DeLinkedList<ELEMENT>&) = delete;
	~DeLinkedList();

protected:
	//头结点。
	DeLinkedElement<ELEMENT> m_root;
	//结点分配器，当空间不足时，抛出SimpleException。
	MyStaticAllocator<DeLinkedElement<ELEMENT>> m_pAllocator;

public:
	//申请新的结点。
	DeLinkedElement<ELEMENT> * NewNodeAtHead();
	//删除这个结点，并且回收内存。
	void Remove(DeLinkedElement<ELEMENT>* link);
	//获得列表中的根结点指针，注意，这个指针里面的element是没有用的。
	DeLinkedElement<ELEMENT> * GetHead();
	//总的可分配的结点数量。
	UINT GetTotalCount();
	//剩余可分配的结点数量。
	UINT GetRemainCount();
};

template<typename ELEMENT>
DeLinkedList<ELEMENT>::DeLinkedList(UINT allocatorSize)
	:m_pAllocator(allocatorSize)
{
	//自身头尾相连。
	m_root.m_pPrev = m_root.m_pNext = &m_root;
}


template<typename ELEMENT>
DeLinkedList<ELEMENT>::~DeLinkedList()
{
}

template<typename ELEMENT>
DeLinkedElement<ELEMENT> * DeLinkedList<ELEMENT>::NewNodeAtHead()
{
	DeLinkedElement<ELEMENT>* pNewNode = m_pAllocator.Malloc();
	pNewNode->m_pNext = m_root.m_pNext;
	m_root.m_pNext = pNewNode;

	pNewNode->m_pPrev = &m_root;
	pNewNode->m_pNext->m_pPrev = pNewNode;
	return pNewNode;
}

template<typename ELEMENT>
inline void DeLinkedList<ELEMENT>::Remove(DeLinkedElement<ELEMENT>* link)
{
	ASSERT(link->m_pNext);
	ASSERT(link->m_pPrev);

	link->m_pPrev->m_pNext = link->m_pNext;
	link->m_pNext->m_pPrev = link->m_pPrev;

	link->m_pNext = link->m_pPrev = nullptr;

	m_pAllocator.Free(link);
}

template<typename ELEMENT>
inline DeLinkedElement<ELEMENT>* DeLinkedList<ELEMENT>::GetHead()
{
	return &m_root;
}

template<typename ELEMENT>
inline UINT DeLinkedList<ELEMENT>::GetTotalCount()
{
	return m_pAllocator.GetTotalCount();
}

template<typename ELEMENT>
inline UINT DeLinkedList<ELEMENT>::GetRemainCount()
{
	return m_pAllocator.GetRemainCount();
}

