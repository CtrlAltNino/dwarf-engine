#pragma once

#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  /// @brief Strong typedef for the project path.
  BOOST_STRONG_TYPEDEF(std::filesystem::path, ProjectPath);
}