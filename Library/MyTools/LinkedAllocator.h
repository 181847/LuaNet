#pragma once
#include "DeLinkedList.h"
template<typename ELEMENT>
class LinkedAllocator : public DeLinkedList<ELEMENT>
{
private:
	//从元素到DeLinkedElement的指针偏移量，有了这个偏移量，
	//我们就可以把一个元素的指针变成一个DeLinkedElement指针，
	//然后作为DeLinkedElement回收内存。
	int offset_from_Element_to_DeLinkedElement;

public:
	//allocatorSize--内存池中初始的元素数量。
	LinkedAllocator(UINT allocatorSize);
	LinkedAllocator(const LinkedAllocator&) = delete;
	LinkedAllocator& operator =(const LinkedAllocator&) = delete;
	~LinkedAllocator();

	//申请一个元素指针，这个元素的处于内部的一个链表的结点中，
	//当内存池元素不足时，在DeBug模式下，会抛出SimpleException。
	ELEMENT* Malloc();
	//将通过当前这个LinkedAllocator获取的一个ELEMENT的指针指向的内存回收到内存池中，
	//如果这个指针不是内存池中的，在DeBug模式下，会抛出SimpleException。
	void Free(ELEMENT* pElement);
};

template<typename ELEMENT>
inline LinkedAllocator<ELEMENT>::LinkedAllocator(UINT allocatorSize)
:DeLinkedList<ELEMENT>(allocatorSize)
{
	//计算从元素到DeLinkedElement的指针偏移量。
	offset_from_Element_to_DeLinkedElement = 
		(int)(	(char*)&this->m_root - 
				(char*)&m_root.element);
}

template<typename ELEMENT>
inline LinkedAllocator<ELEMENT>::~LinkedAllocator()
{
}

template<typename ELEMENT>
inline ELEMENT * LinkedAllocator<ELEMENT>::Malloc()
{
	DeLinkedElement<ELEMENT>* pNewNode = this->NewNodeAtHead();
	return &pNewNode->element;
}

template<typename ELEMENT>
inline void LinkedAllocator<ELEMENT>::Free(ELEMENT * pElement)
{
	//首先将这个元素指针偏移到DeLinkedElement。
	auto* pLinkedNode = (DeLinkedElement<ELEMENT>*)
		((int)pElement + offset_from_Element_to_DeLinkedElement);

	this->Remove(pLinkedNode);
}
