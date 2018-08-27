#pragma once

#include <memory>

namespace utils {
template <typename PointerType>
using UniquePtr = std::unique_ptr<PointerType>;

template <typename PointerType, typename... Args>
UniquePtr<PointerType> make_unique(Args&&... args) {
  return std::unique_ptr<PointerType>(
      new PointerType(std::forward<Args>(args)...));
}
}
