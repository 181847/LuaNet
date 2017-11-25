#pragma once
#include <memory>
//����Debugģʽ����MyStaticAllocator�ռ䲻��ʱ�����׳�SimpleException��
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
	//ͷ��㡣
	DeLinkedElement<ELEMENT> m_root;
	//�������������ռ䲻��ʱ���׳�SimpleException��
	MyStaticAllocator<DeLinkedElement<ELEMENT>> m_pAllocator;

public:
	//�����µĽ�㡣
	DeLinkedElement<ELEMENT> * NewNodeAtHead();
	//ɾ�������㣬���һ����ڴ档
	void Remove(DeLinkedElement<ELEMENT>* link);
	//����б��еĸ����ָ�룬ע�⣬���ָ�������element��û���õġ�
	DeLinkedElement<ELEMENT> * GetHead();
	//�ܵĿɷ���Ľ��������
	UINT GetTotalCount();
	//ʣ��ɷ���Ľ��������
	UINT GetRemainCount();
};

template<typename ELEMENT>
DeLinkedList<ELEMENT>::DeLinkedList(UINT allocatorSize)
	:m_pAllocator(allocatorSize)
{
	//����ͷβ������
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

