#pragma once

#include <fstream>

#include "utils/containers/vector.h"
#include "utils/containers/string.h"

namespace utils {
namespace file_system {

class TreeElement
{
public:
  enum class Type
  {
    None = -1,
    File = 0,
    Directory
  };

  TreeElement(const utils::String& path);
  virtual Type GetType() const = 0;
  virtual const String &GetFullPath() const;

protected:
  const String m_full_path;
}; // class TreeElement

class File : public TreeElement {
 public:
  enum class OpenMode { None, Read, Write, Append };
  File(const String& name);

  void Open(const OpenMode mode);
  void Close();

  String ReadLine();
  void WriteLine(const String& string);
  bool IsOpened() const;

  static bool IsExists(const String& path);
  static String GetFileName(const String& path);
  static String GetDirectory(const String& path);
  static void Copy(const String& from, const String& to);
  static String GetExt(const String& path);

  Type GetType() const override;
 private:
  OpenMode mode_;
  std::fstream std_file_stream_;
}; // class File

class Directory : public TreeElement
{
public:
  typedef utils::Vector<File*> FilesList;
  Directory(const utils::String& path);
  FilesList GetFilesList(const bool recursive = false) const;

  Type GetType() const override;

private:
  utils::Vector<TreeElement*> m_child_list;
}; // class Directory

}  // namespace file_system
}  // namespace utils
