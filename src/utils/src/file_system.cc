#include "utils/file_system.h"

#include <iostream>

#include "utils/logger.h"
#include "utils/containers/converters.h"

CREATE_LOGGER("Utils")

namespace utils {
namespace file_system {

File::File(const String& name) : TreeElement (name), mode_(OpenMode::None) {}

void File::Open(const File::OpenMode mode) {
  assert(OpenMode::None == mode_ && OpenMode::None != mode);
  mode_ = mode;

  std::ios_base::openmode std_mode =
      (OpenMode::Read == mode_) ? std::ios::in : std::ios::out;
  if (OpenMode::Append == mode_) {
    std_mode |= std::ios::app;
  }

  std_file_stream_.open(m_full_path, std_mode);
  LOG_DEBUG("File : " + m_full_path + " opened with mode " +
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

bool File::IsExists(const String &path) // TODO
{
  return false;
}

String File::GetFileName(const String &path) // TODO
{
  return "";
}

String File::GetDirectory(const String &path) // TODO
{
  return "";
}

void File::Copy(const String &from, const String &to) // TODO
{
  LOG_DEBUG("Copying " << from << " to " << to);
}

String File::GetExt(const String &path) // TODO
{
 return "";
}

TreeElement::Type File::GetType() const
{
  return TreeElement::Type::File;
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

Directory::Directory(const String &path) // TODO recursive tree
  : TreeElement (path)
{}

Directory::FilesList Directory::GetFilesList(const bool recursive) const
{
  LOG_TRACE("Finding files " << GetFullPath());
  FilesList files;
  for(auto element : m_child_list)
  {
    switch(element->GetType())
    {
    case Type::File :
    {
      files.push_back(dynamic_cast<File*>(element));
      break;
    }
    case Type::Directory :
    {
      if(recursive)
      {
        const FilesList& child_files = dynamic_cast<Directory*>(element)->GetFilesList(recursive);
        files.assign(child_files.begin(), child_files.end());
      }
      break;
    }
    default:
    {
      LOG_ERROR("Unsupported element type.");
    }
    }

  }
}

TreeElement::Type Directory::GetType() const
{
  return TreeElement::Type::Directory;
}

const String &TreeElement::GetFullPath() const
{
  return m_full_path;
}

TreeElement::TreeElement(const utils::String& path)
  : m_full_path(path)
{}

}  // namespace file_system
}  // namespace utils
