/*
 * __        __                 _        _             ____
 * \ \      / /   ___    _ __  | | __   (_)  _ __     |  _ \   _ __    ___     __ _   _ __    ___   ___   ___
 *  \ \ /\ / /   / _ \  | '__| | |/ /   | | | '_ \    | |_) | | '__|  / _ \   / _` | | '__|  / _ \ / __| / __|
 *   \ V  V /   | (_) | | |    |   <    | | | | | |   |  __/  | |    | (_) | | (_| | | |    |  __/ \__ \ \__ \
 *    \_/\_/     \___/  |_|    |_|\_\   |_| |_| |_|   |_|     |_|     \___/   \__, | |_|     \___| |___/ |___/
 *                                                                            |___/
 *
 * TODO:
 *  - Implement DFS
 *  - Perform unit testing
 *  - Document
 */

#pragma once

#include <cstddef>
#include <map>
#include <memory>
#include <source_location>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <vector>

#define FUNC_NAME std::string(std::source_location::current().function_name())

template <typename T>
class Node {
private:
    T uid;
    std::unordered_set<T> in;
    std::unordered_set<T> out;

    template <typename T>
    friend class Digraph;

public:
    Node(T const uid) : uid(uid) {}
};

template <typename T>
class node_uniqueness_violation : public std::exception {
private:
    std::string exception_message;

public:
    node_uniqueness_violation(T const uid) {
        std::ostringstream oss;
        oss << "Context: Creating new node.\n"
            << "Error: Node uid (" << uid << ") already exists.\n"
            << "Solution: Use new uid or edit existing node.";
        exception_message = oss.str();
    }
    char const * what() const noexcept override {
        return exception_message.c_str();
    }
};

template <typename T>
class Digraph {
    static_assert(std::is_arithmetic<T>::value);

private:
    std::map<T, std::unique_ptr<Node<T>>> nodes;

    std::map<T, std::unique_ptr<Node<T>>>::iterator getNode(T const uid,
        std::string caller);

public:
    Digraph() = default;
    void addNode(T const uid);
    void deleteNode(T const uid);
    void addArc(T const from_uid, T const to_uid);
    void deleteArc(T const from_uid, T const to_uid);
    bool checkArc(T const from_uid, T const to_uid) const;
    std::size_t outDegree(T const uid);
    std::size_t inDegree(T const uid);
    void bfSearch(T const root_uid, std::vector<T> &vec);

    // Not yet implemented
    void dfSearch(T const root_uid, std::vector<T> &vec);
};

template <typename T>
class node_not_found : public std::exception {
private:
    std::string exception_message;

public:
    node_not_found(std::string const caller, T const uid) {
        std::ostringstream oss;
        oss << "Context: Manipulating node via " << caller << ".\n"
            << "Error: Node with desired uid (" << uid << ") not found.\n"
            << "Solution: Use existing uid or create new node.";
        exception_message = oss.str();
    }
    char const * what() const noexcept override {
        return exception_message.c_str();
    }
};

template <typename T>
#ifdef _MSC_VER
__forceinline
#else
inline
#endif
std::map<T, std::unique_ptr<Node<T>>>::iterator Digraph<T>::getNode(
    T const uid, std::string caller) {
    auto const itr = nodes.find(uid);
    if (itr == nodes.end()) throw node_not_found(caller, uid);
    return itr;
}

template <typename T>
inline void Digraph<T>::addNode(T const uid) {
    if (nodes.find(uid) == nodes.end())
        nodes[uid] = std::unique_ptr<Node<T>>(new Node<T>(uid));
    else
        throw node_uniqueness_violation(uid);
}

template <typename T>
inline void Digraph<T>::deleteNode(T const uid) {
    auto const to_delete = getNode(uid, FUNC_NAME);
    for (auto const in_neighbour : to_delete->second->in)
        getNode(in_neighbour, FUNC_NAME)->second->out.erase(uid);
    for (auto const out_neighbour : to_delete->second->out)
        getNode(out_neighbour, FUNC_NAME)->second->in.erase(uid);
    nodes.erase(to_delete);
}

template <typename T>
inline void Digraph<T>::addArc(T const from_uid, T const to_uid) {
    auto const from_itr = getNode(from_uid, FUNC_NAME);
    auto const to_itr = getNode(to_uid, FUNC_NAME);
    from_itr->second->out.insert(to_uid);
    to_itr->second->in.insert(from_uid);
}

template <typename T>
inline void Digraph<T>::deleteArc(T const from_uid, T const to_uid) {
    auto const from_itr = getNode(from_uid, FUNC_NAME);
    auto const to_itr = getNode(to_uid, FUNC_NAME);
    from_itr->second->out.erase(to_uid);
    to_itr->second->in.erase(from_uid);
}

template <typename T>
inline bool Digraph<T>::checkArc(T const from_uid, T const to_uid) const {
    auto const from_itr = getNode(from_uid, FUNC_NAME);
    return from_itr->second->out.find(to_uid) != from_itr->second->out.end();
}

template <typename T>
inline std::size_t Digraph<T>::outDegree(T const uid) {
    auto const itr = getNode(uid, FUNC_NAME);
    return itr->second->out.size();
}

template <typename T>
inline std::size_t Digraph<T>::inDegree(T const uid) {
    auto const itr = getNode(uid, FUNC_NAME);
    return itr->second->in.size();
}

template <typename T>
void Digraph<T>::bfSearch(T const root_uid, std::vector<T> &vec) {
    std::unordered_set<T> visited;
    vec.push_back(root_uid);
    visited.insert(root_uid);
    for (std::size_t index = 0; index != vec.size(); index++) {
        auto curr_uid = *(vec.begin() + index);
        auto curr_node = getNode(curr_uid, FUNC_NAME);
        for (auto out_neighbour : curr_node->second->out) {
            if (visited.find(out_neighbour) == visited.end()) {
                vec.push_back(out_neighbour);
                visited.insert(out_neighbour);
            }
        }
    }
}
