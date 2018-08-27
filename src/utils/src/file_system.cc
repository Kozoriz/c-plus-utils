#include "utils/file_system.h"

#include <iostream>

#include "utils/logger.h"

CREATE_LOGGER("Utils")

namespace utils {
namespace file_system {

File::File(const String& name) : file_name_(name), mode_(OpenMode::None) {}

void File::Open(const File::OpenMode mode) {
  assert(OpenMode::None == mode_ && OpenMode::None != mode);
  mode_ = mode;

  std::ios_base::openmode std_mode =
      (OpenMode::Read == mode_) ? std::ios::in : std::ios::out;
  if (OpenMode::Append == mode_) {
    std_mode |= std::ios::app;
  }

  std_file_stream_.open(file_name_, std_mode);
  LOG_DEBUG("File : " + file_name_ + " opened with mode " +
            utils::ConvertToString(static_cast<int>(mode_)));
}

void File::Close() {
  assert(OpenMode::None != mode_);
  mode_ = OpenMode::None;

  std_file_stream_.close();
}

bool File::IsOpened() const {
  return OpenMode::None != mode_;
}

String File::ReadLine() {
  assert(OpenMode::None != mode_);
  String line;
  std::getline(std_file_stream_, line);
  return line;
}

void File::WriteLine(const String& string) {
  assert(OpenMode::Append == mode_ || OpenMode::Write == mode_);
  std_file_stream_ << string << std::endl;
}

}  // namespace file_system
}  // namespace utils
