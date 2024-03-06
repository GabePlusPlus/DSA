/*
 * TODO:
 * - Replace raw pointers with smart pointers
 * - Implement a peek function
 */

#pragma once

#include <stdexcept>

template <typename T>
class StackItem {
	private:
		T value;
		StackItem* below;
	public:
		StackItem(T const value = T(), StackItem* const below = nullptr);
		T getValue() const;
		StackItem* getBelow() const;
		void setValue(T const value);
		void setBelow(StackItem* const below);
};

template <typename T>
class Stack {
	private:
		StackItem<T>* top;
	public:
		Stack();
		~Stack();
		void push(T const value);
		T pop();
		void clear();
		bool isEmpty() const;
};

/*
 * STACKITEM
 */

template <typename T>
inline StackItem<T>::StackItem(T const value /* = T() */,
StackItem* const below /* = nullptr */) {
	this->value = value;
	this->below = below;
}

template <typename T>
inline StackItem<T>* StackItem<T>::getBelow() const {
	return below;
}

template <typename T>
inline T StackItem<T>::getValue() const {
	return value;
}

template <typename T>
inline void StackItem<T>::setBelow(StackItem* const below) {
	this->below = below;
}

template <typename T>
inline void StackItem<T>::setValue(T const value) {
	this->value = value;
}

/*
 * STACK
 */

template <typename T>
Stack<T>::Stack() : top(nullptr) {}

#ifdef _MSC_VER
template <typename T>
__forceinline bool Stack<T>::isEmpty() const {
	return top == nullptr ? true : false;
}
#else
template <typename T>
inline bool Stack<T>::isEmpty() const {
	return top == nullptr ? true : false;
}
#endif

template <typename T>
Stack<T>::~Stack() {
	if (!isEmpty()) clear();
}

template <typename T>
void Stack<T>::push(T const value) {
	StackItem<T>* const new_item = new StackItem<T>(value);
	if (!isEmpty())
		new_item->setBelow(top);
	top = new_item;
}

template <typename T>
T Stack<T>::pop() {
	if (!isEmpty()) {
		T const ret = top->getValue();
		StackItem<T>* temp = top;
		top = top->getBelow();
		delete temp;
		return ret;
	} else {
		throw std::out_of_range("pop: stack is empty");
	}
}

template <typename T>
void Stack<T>::clear() {
	if (!isEmpty()) {
		StackItem<T>* temp = top;
		while (temp != nullptr) {
			top = top->getBelow();
			delete temp;
			temp = top;
		}
	}
}
