/*
 * A simple linked list implementation.
 * Assumes that the type T used with LinkedList
 * has an ostream overload (see LinkedList<T>::traverse).
 *
 * Represents my first exposure to move semantics;
 * possible updates to come for more appropriate
 * use of std::move.
 */

#pragma once

#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>

template <typename T>
class Node {
private:
    T value;
    std::shared_ptr<Node> next;

public:
    Node(T const value = T(), std::shared_ptr<Node>&& const next = nullptr);
    void setNext(std::shared_ptr<Node>&& const next);
    T getValue() const;
    std::shared_ptr<Node> getNext() const;
};

template <typename T>
class LinkedList {
private:
    std::shared_ptr<Node<T>> head, tail;
    std::size_t size;

public:
    LinkedList();
    bool isFull() const;
    void append(T const value);
    void insert(T const value, std::size_t const index);
    void traverse() const;
    void remove(std::size_t const index);
    T operator[](std::size_t index);
};

/*
* NODE
*/

template <typename T>
inline Node<T>::Node(T const value, std::shared_ptr<Node>&& const next) {
    this->value = value;
    this->next = std::move(next);
}

template <typename T>
void Node<T>::setNext(std::shared_ptr<Node<T>>&& const next) {
    this->next = std::move(next);
}

template <typename T>
inline T Node<T>::getValue() const {
    return value;
}

template <typename T>
inline std::shared_ptr<Node<T>> Node<T>::getNext() const {
    return next;
}

/*
* LINKED LIST
*/

template <typename T>
inline LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

template <typename T>
inline bool LinkedList<T>::isFull() const {
    return size == std::numeric_limits<std::size_t>::max();
}

template <typename T>
void LinkedList<T>::append(T const value) {
    if (size == 0) {
        tail = head = std::make_shared<Node<T>>(value);
        size++;
    } else if (isFull()) {
        throw std::length_error("append: linked list is full");
    } else {
        tail->setNext(std::make_shared<Node<T>>(value));
        tail = std::move(tail->getNext());
        size++;
    }
}

template <typename T>
void LinkedList<T>::insert(T const value, std::size_t const index) {
    if (isFull()) throw std::length_error("insert: linked list is full");

    if (index > size) throw std::out_of_range("insert: index is out of range");

    if (index == 0) {
        // prepend
        head =
            std::make_shared<Node<T>>(value, std::make_shared<Node<T>>(*head));
        size++;
        return;
    }

    std::shared_ptr<Node<T>> temp = head;
    for (std::size_t i = 0; i < index - 1; i++)
        temp = temp->getNext();
    temp->setNext(std::make_shared<Node<T>>(value, temp->getNext()));
    size++;
}

template <typename T>
void LinkedList<T>::traverse() const {
    if (size == 0) {
        std::cout << "(Empty)\n";
        return;
    }

    std::shared_ptr<Node<T>> temp = head;
    while (temp) {
        std::cout << temp->getValue() << '\n';
        temp = temp->getNext();
    }
}

template <typename T>
void LinkedList<T>::remove(std::size_t const index) {
    if (index >= size) throw std::out_of_range("remove: index is out of range");

    std::shared_ptr<Node<T>> temp = head;

    if (index == 0) {
        head = std::move(temp->getNext());
        size--;
        return;
    }

    if (index == size - 1) {
        while (temp->getNext() != tail) temp = temp->getNext();
        temp->setNext(nullptr);
        tail = temp;
        size--;
        return;
    }

    for (std::size_t i = 0; i < index - 1; i++)
        temp = temp->getNext();

    temp->setNext(std::move(temp->getNext()->getNext()));
    size--;
}

template <typename T>
inline T LinkedList<T>::operator[](std::size_t index) {
    if (index >= size)
        throw std::out_of_range("operator[]: index is out of range");
    std::shared_ptr<Node<T>> temp = head;
    for (std::size_t i = 0; i < index; i++)
        temp = temp->getNext();
    return temp->getValue();
}
