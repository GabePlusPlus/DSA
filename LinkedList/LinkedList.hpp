/*
 * TODO:
 *	- Replace raw pointers with smart pointers
 *	- Overload the bracket operator (`[]`) for LinkedList
 */

#pragma once

#include <iostream>
#include <limits>
#include <stdexcept>

template <typename T>
class Node {
private:
	T value;
	Node* next;

public:
	Node(T const value = T(), Node* const next = nullptr);
	T getValue() const;
	Node* getNext() const;
	void setValue(T const value);
	void setNext(Node* const next);
};

template <typename T>
class LinkedList {
private:
	Node<T> *head, *tail;
	size_t size;

public:
	LinkedList();
	~LinkedList();
	bool isFull() const;
	void append(T const value);
	void insert(T const value, size_t const index);
	void traverse() const;
	void _delete(size_t const index);
	void clear();
};

/*
 * NODE
 */

template <typename T>
inline Node<T>::Node(T const value /* = T() */,
	Node* const next /* = nullptr */) {
	this->value = value;
	this->next = next;
}

template <typename T>
inline T Node<T>::getValue() const {
	return value;
}

template <typename T>
inline Node<T>* Node<T>::getNext() const {
	return next;
}

template <typename T>
inline void Node<T>::setValue(T const value) {
	this->value = value;
}

template <typename T>
inline void Node<T>::setNext(Node* const next) {
	this->next = next;
}

/*
 * LINKED LIST
 */

template <typename T>
inline LinkedList<T>::LinkedList() {
	head = tail = nullptr;
	size = static_cast<size_t>(0);
}

template <typename T>
inline LinkedList<T>::~LinkedList() {
	if (size != static_cast<size_t>(0)) clear();
}

template <typename T>
inline bool LinkedList<T>::isFull() const {
	return size != std::numeric_limits<size_t>::max() ? false : true;
}

template <typename T>
void LinkedList<T>::append(T const value) {
	if (size == static_cast<size_t>(0)) {
		head = tail = new Node<T>(value);
		size++;
	} else if (isFull()) {
		throw std::length_error("append: linked list is full");
	} else {
		tail->setNext(new Node<T>(value));
		tail = tail->getNext();
		size++;
	}
}

template <typename T>
void LinkedList<T>::insert(T const value, size_t const index) {
	if (isFull()) throw std::length_error("insert: linked list is full");

	if (index > size - static_cast<size_t>(1))
		throw std::out_of_range("insert: index is out of range");

	if (index == static_cast<size_t>(0)) {
		Node<T>* new_node = new Node<T>(value, head);
		head = new_node;
		size++;
		return;
	}

	if (index == size) {
		append(value);
		size++;
		return;
	}

	Node<T>* temp = head;
	for (size_t i = static_cast<size_t>(0);
		i < index - static_cast<size_t>(1); i++) {
		temp = temp->getNext();
	}
	Node<T>* new_node = new Node<T>(value, temp->getNext());
	temp->setNext(new_node);
	size++;
}

template <typename T>
void LinkedList<T>::traverse() const {
	if (size == static_cast<size_t>(0)) {
		std::cout << "(Empty)" << std::endl;
		return;
	}

	Node<T>* temp = head;
	while (temp != nullptr) {
		std::cout << temp->getValue() << std::endl;
		temp = temp->getNext();
	}
}

template <typename T>
void LinkedList<T>::_delete(size_t const index) {
	if (index > size - static_cast<size_t>(1))
		throw std::out_of_range("_delete: index is out of range");

	Node<T>* temp = head;

	if (index == static_cast<size_t>(0)) {
		head = temp->getNext();
		delete temp;
		size--;
		return;
	}

	if (index == size - static_cast<size_t>(1)) {
		while (temp->getNext() != tail) temp = temp->getNext();
		delete tail;
		temp->setNext(nullptr);
		tail = temp;
		size--;
		return;
	}

	for (size_t i = static_cast<size_t>(0);
		i < index - static_cast<size_t>(1); i++) {
		temp = temp->getNext();
	}

	Node<T>* to_delete = temp->getNext();
	temp->setNext(to_delete->getNext());
	delete to_delete;
	size--;
}

template <typename T>
void LinkedList<T>::clear() {
	Node<T>* temp = head;
	while (head != nullptr) {
		head = head->getNext();
		delete temp;
		temp = head;
	}
	tail = head; // == nullptr
	size = static_cast<size_t>(0);
}
