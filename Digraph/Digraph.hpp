/*
* A digraph (directed graph) implementation where each node is uniquely
* identified by a UID (unique identifier). Designed for use with arithmetic
* types; delete line 57 for use with other types (functionality not guaranteed).
*/

#pragma once

#include <cstddef>
#include <functional>
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
    std::unordered_set<T> in; // contains in-neighbour UIDs
                              // (in-neighbour -> node)
    std::unordered_set<T> out; // contains out-neighbour UIDs
                               // (node -> out-neighbour)

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
    //       ^  ^^^^^^^^^^^^^^^^^^^^^^^^
    //     UID  pointer to node with matching UID

    std::map<T, std::unique_ptr<Node<T>>>::iterator getNode(T const uid,
        std::string const caller);

public:
    Digraph() = default;
    void addNode(T const uid);
    void deleteNode(T const uid);
    void addArc(T const from_uid, T const to_uid);
    void deleteArc(T const from_uid, T const to_uid);
    bool checkArc(T const from_uid, T const to_uid) const;
    std::size_t outDegree(T const uid);
    std::size_t inDegree(T const uid);
    void bfSearch(T const root_uid, std::vector<T>& const vec);
    void dfSearch(T const root_uid, std::vector<T>& const vec);
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
    T const uid, std::string const caller) {
    /*
     * Returns an iterator `itr` associated with Digraph<T>::nodes
     * where itr->second points to the node with a UID equal to the
     * `uid` argument (or throws an exception if no such node exists)
     */
    auto const itr = nodes.find(uid);
    if (itr == nodes.end()) throw node_not_found(caller, uid);
    return itr;
}

template <typename T>
inline void Digraph<T>::addNode(T const uid) {
    if (nodes.find(uid) == nodes.end())
        // node with matching UID does not exist--proceed
        nodes[uid] = std::unique_ptr<Node<T>>(new Node<T>(uid));
    else
        // node with matching UID exists--abort
        throw node_uniqueness_violation(uid);
}

template <typename T>
inline void Digraph<T>::deleteNode(T const uid) {
    auto const to_delete = getNode(uid, FUNC_NAME);
    // erase the deleted node's UID from its in-neighbours'
    // out-neighbour set ("we are no longer connected
    // to this node")
    for (auto const in_neighbour : to_delete->second->in)
        getNode(in_neighbour, FUNC_NAME)->second->out.erase(uid);
    // erase the deleted node's UID from its out-neighbours'
    // in-neighbour set ("this node is no longer connected
    // to us")
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
void Digraph<T>::bfSearch(T const root_uid, std::vector<T>& const vec) {
    std::unordered_set<T> visited; // prevents cycle loops
    vec.push_back(root_uid);
    visited.insert(root_uid);
    for (std::size_t index = 0; index != vec.size(); index++) {
    // vec.size() will increase until all nodes have been visited
        auto curr_uid = *(vec.begin() + index);
        auto curr_node = getNode(curr_uid, FUNC_NAME);
        // push out-neighbours to vec
        // they will be visited over subsequent for-loop iterations
        for (auto out_neighbour : curr_node->second->out) {
            if (visited.find(out_neighbour) == visited.end()) {
                vec.push_back(out_neighbour);
                visited.insert(out_neighbour);
            }
        }
    }
}

template <typename T>
void Digraph<T>::dfSearch(T const root_uid, std::vector<T>& const vec) {
    std::unordered_set<T> traversed; // prevents cycle loops
    std::function<void(T)> dfs = [&](T const curr_uid) {
        traversed.insert(curr_uid);
        auto curr_node = getNode(curr_uid, FUNC_NAME);
        // iterate through out-neighbours before...
        for (auto out_neighbour : curr_node->second->out)
            if (traversed.find(out_neighbour) == traversed.end())
                dfs(out_neighbour);
        vec.push_back(curr_uid); // ...visiting this node
    };
    dfs(root_uid);
}
