#pragma once

#include <fstream>

#include "utils/containers/vector.h"
#include "utils/containers/string.h"

namespace utils {
namespace file_system {

class File {
 public:
  enum class OpenMode { None, Read, Write, Append };
  File(const String& name);

  void Open(const OpenMode mode);
  void Close();

  String ReadLine();
  void WriteLine(const String& string);
  bool IsOpened() const;

 private:
  const String file_name_;
  OpenMode mode_;
  std::fstream std_file_stream_;
};

}  // namespace file_system
}  // namespace utils
