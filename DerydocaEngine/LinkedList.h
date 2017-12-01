#pragma once
#include "LinkedListNode.h"

template <class T>
class LinkedList
{
public:
	LinkedList() {
	}
	~LinkedList() {
	}

	void add(LinkedListNode<T>* node) {
		// If the head is set, find the tail and add the node to it
		if (m_head != NULL) {
			// Get the tail
			LinkedListNode<T>* tail = m_head;
			while (tail->next() != NULL) {
				tail = tail->next();
			}

			// Add on to the tail
			tail->addNext(node);
		}
		// Otherwise, this is the new head
		else {
			m_head = node;
		}
	}

	void add(T* nodeData) {
		LinkedListNode<T>* node = new LinkedListNode<T>(nodeData);
		add(node);
	}

private:
	LinkedListNode<T>* m_head;
};

