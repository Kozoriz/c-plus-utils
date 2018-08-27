#pragma once

#include <boost/program_options.hpp>
#include "utils/containers/string.h"


namespace utils {
/** AppSettings - base class for application profiling with ini file 
* can be used as base for app settings profiler 
*/
class AppSettings {
public:
  AppSettings();
  void DebulLogAllProperties(const boost::program_options::variables_map& boost_var_map) final;

protected:
  void LoadIniFile(const String& ini_file_path);

  template <class T>
  void AddOption(const std::string& option_name,
               T& variable,
               const std::string& help_description = "");

private:
  boost::program_options::options_description options_description_("Ini");

};  // class AppSettings
}  // namespace utils 
