#pragma once
#include "DeLinkedList.h"
template<typename ELEMENT>
class LinkedAllocator : public DeLinkedList<ELEMENT>
{
private:
	//��Ԫ�ص�DeLinkedElement��ָ��ƫ�������������ƫ������
	//���ǾͿ��԰�һ��Ԫ�ص�ָ����һ��DeLinkedElementָ�룬
	//Ȼ����ΪDeLinkedElement�����ڴ档
	int offset_from_Element_to_DeLinkedElement;

public:
	//allocatorSize--�ڴ���г�ʼ��Ԫ��������
	LinkedAllocator(UINT allocatorSize);
	LinkedAllocator(const LinkedAllocator&) = delete;
	LinkedAllocator& operator =(const LinkedAllocator&) = delete;
	~LinkedAllocator();

	//����һ��Ԫ��ָ�룬���Ԫ�صĴ����ڲ���һ������Ľ���У�
	//���ڴ��Ԫ�ز���ʱ����DeBugģʽ�£����׳�SimpleException��
	ELEMENT* Malloc();
	//��ͨ����ǰ���LinkedAllocator��ȡ��һ��ELEMENT��ָ��ָ����ڴ���յ��ڴ���У�
	//������ָ�벻���ڴ���еģ���DeBugģʽ�£����׳�SimpleException��
	void Free(ELEMENT* pElement);
};

template<typename ELEMENT>
inline LinkedAllocator<ELEMENT>::LinkedAllocator(UINT allocatorSize)
:DeLinkedList<ELEMENT>(allocatorSize)
{
	//�����Ԫ�ص�DeLinkedElement��ָ��ƫ������
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
	//���Ƚ����Ԫ��ָ��ƫ�Ƶ�DeLinkedElement��
	auto* pLinkedNode = (DeLinkedElement<ELEMENT>*)
		((int)pElement + offset_from_Element_to_DeLinkedElement);

	this->Remove(pLinkedNode);
}
