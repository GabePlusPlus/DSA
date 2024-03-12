/*
 * TODO:
 *  - Implement peek method
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
    Stack();
    void push(T const value);
    T pop();
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
    return std::move(below);
}

/*
* STACK
*/

template <typename T>
Stack<T>::Stack() : top(nullptr) {}

#ifdef _MSC_VER
template <typename T>
__forceinline bool Stack<T>::isEmpty() const {
    return !top;
}
#else
template <typename T>
inline bool Stack<T>::isEmpty() const {
    return !top;
}
#endif

template <typename T>
void Stack<T>::push(T const value) {
    if (!isEmpty())
        top = std::make_unique<StackItem<T>>(value, std::move(top));
    else
        top = std::make_unique<StackItem<T>>(value);
}

template <typename T>
T Stack<T>::pop() {
    if (!isEmpty()) {
        T const ret = top->getValue();
        top = top->getBelow();
        return ret;
    } else {
        throw std::out_of_range("pop: stack is empty");
    }
}
