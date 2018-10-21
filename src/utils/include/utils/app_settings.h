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
  void DebulLogAllProperties(const boost::program_options::variables_map& boost_var_map);

protected:
  template <class T>
  void AddOption(const std::string& option_name,
               T& variable,
               const std::string& help_description = "") {
    options_description_.add_options()(option_name.c_str(),
                                      boost::program_options::value<T>(&variable),
                                      help_description.c_str());
  }

  void ProcessIniFile(const String& ini_file);

private:
  boost::program_options::options_description options_description_;
};  // class AppSettings
}  // namespace utils 
