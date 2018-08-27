#pragma once

#include <atomic>

namespace utils {
namespace synchronization {

template <typename T>
using Atomic = std::atomic<T>;

typedef Atomic<bool> AtomicBool;
}
}
