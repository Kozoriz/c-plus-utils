#include "utils/file_system.h"

#include <iostream>
#include <memory>
#include <dirent.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sendfile.h>
#include <fcntl.h>

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

  if(!std_file_stream_.get())
  {
    std_file_stream_ = std::make_unique<std::fstream>();
  }

  std_file_stream_->open(m_full_path, std_mode);

  if(OpenMode::Read == mode_)
  {
    std_file_stream_->seekg(0, std::ios_base::beg);
    std_file_stream_->clear();
  }
  LOG_DEBUG("File : " + m_full_path + " opened with mode " +
            utils::ConvertToString(static_cast<int>(mode_)));
}

void File::Close() {
  assert(OpenMode::None != mode_);
  mode_ = OpenMode::None;

  std_file_stream_->flush();
  std_file_stream_->close();
//  std_file_stream_.release();
}

bool File::IsOpened() const {
  return OpenMode::None != mode_;
}

String File::GetExt()
{
  return File::GetExt(m_full_path);
}

bool File::IsEof() const
{
  return std_file_stream_->eof();
}

bool File::IsExists(const String &path)
{
  struct stat buffer;
  return (stat(path.c_str(), &buffer) == 0);
}

String File::GetFileName(const String &path)
{
  const size_t last_dot = path.find_last_of('.');
  const size_t last_dash = path.find_last_of('/');
  return path.substr(last_dash + 1, last_dot - last_dash - 1);
}

String File::GetDirectory(const String &path)
{
  return path.substr(0, path.find_last_of('/') - 1);
}

void File::Copy(const String &from, const String &to)
{
  LOG_DEBUG("Copying " << from << " to " << to);

  int source = open(from.c_str(), O_RDONLY, 0);
  int dest = open(to.c_str(), O_WRONLY | O_CREAT, 0644);

  // struct required, rationale: function stat() exists also
  struct stat stat_source;
  fstat(source, &stat_source);

  sendfile(dest, source, nullptr, static_cast<size_t>(stat_source.st_size));

  close(source);
  close(dest);
}

String File::GetExt(const String &path)
{
  return path.substr(path.find_last_of('.') + 1);
}

TreeElement::Type File::GetType() const
{
  return TreeElement::Type::File;
}

String File::ReadLine() {
  assert(OpenMode::None != mode_);
  String line;
  std::getline(*std_file_stream_, line);
  return line;
}

void File::WriteLine(const String& string) {
  assert(OpenMode::Append == mode_ || OpenMode::Write == mode_);
  (*std_file_stream_) << string << std::endl;
}

Directory::Directory(const String &path)
  : TreeElement (path)
{
  LOG_AUTO_TRACE();
  LOG_TRACE(path);
  DIR *dp;
  struct dirent *dirp;
  if((dp  = opendir(path.c_str())) == nullptr) {
      LOG_ERROR("Error(" << errno << ") opening " << path);
      return;
  }

  while ((dirp = readdir(dp)) != nullptr) {
    switch (dirp->d_type) {
      case DT_REG: {
        LOG_TRACE("found file : " << path + dirp->d_name);
        m_child_list.push_back(new File(ExtendPath(path, dirp->d_name)));
        break;
      }
      case DT_DIR :
      {
        if (0 == strcmp(dirp->d_name, "..") || 0 == strcmp(dirp->d_name, "."))
        {
          break;
        }
        m_child_list.push_back(new Directory(ExtendPath(path, dirp->d_name)));
        break;
      }
      default:
      {
        LOG_ERROR("Unsupported element : " << dirp->d_name << " type : " << (int)dirp->d_type);
      }
    }
  }
  closedir(dp);
}

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
        files.insert(files.end(), child_files.begin(), child_files.end());
      }
      break;
    }
    default:
    {
      LOG_ERROR("Unsupported element type.");
    }
    }

  }
  return files;
}

TreeElement::Type Directory::GetType() const
{
  return TreeElement::Type::Directory;
}

void Directory::Create(const String &path)
{
  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void Directory::RecursiveCreate(const String &path)
{
  Vector<String> levels = utils::SplitCSV(path, "/");
  String cur_path = "/";
  for(auto l : levels)
  {
    cur_path = ExtendPath(cur_path, l);
    Create(cur_path);
  }
}

const String &TreeElement::GetFullPath() const
{
  return m_full_path;
}

TreeElement::TreeElement(const utils::String& path)
  : m_full_path(path)
{}

String ExtendPath(const String &path, const String &add)
{
  return path + "/" + add;
}

}  // namespace file_system
}  // namespace utils
