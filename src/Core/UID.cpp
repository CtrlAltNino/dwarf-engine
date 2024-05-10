

#include <random>
#include "Core/UID.h"

namespace Dwarf
{

  static std::random_device                      randomDevice;
  static std::mt19937_64                         engine(randomDevice());
  static std::uniform_int_distribution<uint64_t> uniformDistribution;

  UID::UID()
    : m_Uuid(uniformDistribution(engine))
  {
  }

  UID::UID(uint64_t uuid)
    : m_Uuid(uuid)
  {
  }
}