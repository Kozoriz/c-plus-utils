#pragma once

#include <unordered_set>

namespace utils {
template <typename Key, typename Hash = std::hash<Key>>
using UnorderedSet = std::unordered_set<Key, Hash>;

template <typename HashableType>
using HashFunc = std::hash<HashableType>;
}
