/*
 * A simple, lightweight stack implementation.
 */

#pragma once

#include <memory>
#include <stdexcept>
#include <utility>

template <typename T>
class StackItem {
private:
    T value;
    std::unique_ptr<StackItem> below;

public:
    StackItem(T const value = T(),
        std::unique_ptr<StackItem>&& const below = nullptr);
    T getValue() const;
    std::unique_ptr<StackItem> getBelow();
};

template <typename T>
class Stack {
private:
    std::unique_ptr<StackItem<T>> top;

public:
    Stack() : top(nullptr) {}
    void push(T const value);
    T pop();
    T peek() const;
    bool isEmpty() const;
};

/*
* STACKITEM
*/

template <typename T>
inline StackItem<T>::StackItem(
    T const value /* = T() */,
    std::unique_ptr<StackItem>&& const below /* = nullptr */) {
    this->value = value;
    this->below = std::move(below);
}

template <typename T>
inline T StackItem<T>::getValue() const {
    return value;
}

template <typename T>
inline std::unique_ptr<StackItem<T>> StackItem<T>::getBelow() {
    // Stack<T>::pop > this->getBelow > ownership transfer
    // from this->below to Stack<T>::top via std::move
    return std::move(below);
}

/*
* STACK
*/

template <typename T>
inline bool Stack<T>::isEmpty() const {
    return !top;
}

template <typename T>
inline void Stack<T>::push(T const value) {
    if (!isEmpty())
        top = std::make_unique<StackItem<T>>(value, std::move(top));
    else
        top = std::make_unique<StackItem<T>>(value);
}

template <typename T>
inline T Stack<T>::pop() {
    if (!isEmpty()) {
        T const ret = top->getValue();
        top = top->getBelow();
        return ret;
    } else {
        throw std::out_of_range("pop: stack is empty");
    }
}

template <typename T>
inline T Stack<T>::peek() const {
    if (!isEmpty()) return top->getValue();
    else throw std::out_of_range("peek: stack is empty");
}
