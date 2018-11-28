#include <fstream>

#include "utils/app_settings.h"
#include "utils/logger.h"
#include "utils/numbers.h"

CREATE_LOGGER("AppSettings")

namespace utils {
AppSettings::AppSettings() : options_description_("Ini")
{}

void AppSettings::DebulLogAllProperties(
    const boost::program_options::variables_map& boost_var_map) {
  LOG_DEBUG("Properties read : " << boost_var_map.size());
  for (const auto& pair : boost_var_map) {
    utils::Stringstream debug_string;
    debug_string << "Property : " << pair.first.c_str();
    auto& value = pair.second.value();
    if (auto v = boost::any_cast<UInt>(&value)) {
      debug_string << "= " << *v;
    } else if (auto v = boost::any_cast<utils::String>(&value)) {
      debug_string << "= " << (v->c_str());
    } else if (auto v = boost::any_cast<Float>(&value)) {
      debug_string << "= " << *v;
    } else if (auto v = boost::any_cast<int>(&value)) {
      debug_string << "= " << *v;
    }
    LOG_DEBUG(debug_string.str());
  }
}

void AppSettings::ProcessIniFile(const String& ini_file) {
  LOG_AUTO_TRACE();
  std::ifstream ifs;
  ifs.open(ini_file);
  assert(ifs.is_open());

  boost::program_options::variables_map temp_variableMap;

  boost::program_options::store(
      boost::program_options::parse_config_file(ifs, options_description_, true),
      temp_variableMap);
  boost::program_options::notify(temp_variableMap);

  DebulLogAllProperties(temp_variableMap);
}

}  // namespace utils
