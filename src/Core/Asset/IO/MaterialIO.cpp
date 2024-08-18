#include "MaterialIO.h"
#include "Utilities/FileHandler.h"

namespace Dwarf
{
  MaterialIO::MaterialIO(std::shared_ptr<IMaterialFactory> materialFactory,
                         std::shared_ptr<IDwarfLogger>     logger)
    : m_MaterialFactory(materialFactory)
    , m_Logger(logger)
  {
  }

  MaterialIO::~MaterialIO() {}

  void
  MaterialIO::SaveMaterial(std::shared_ptr<IMaterial> material)
  {
    // Save the material to a file
  }

  std::shared_ptr<IMaterial>
  MaterialIO::LoadMaterial(std::filesystem::path const& path)
  {
    m_Logger->LogInfo(
      Log(fmt::format("Loading material from file: {}", path.string()),
          "MaterialIO"));

    if (!FileHandler::FileExists(path))
    {
      m_Logger->LogError(Log(
        fmt::format("Material asset file does not exist: {}", path.string()),
        "MaterialIO"));
      throw std::runtime_error("Material asset file does not exist");
    }

    return m_MaterialFactory->CreateMaterial(
      nlohmann::json::parse(FileHandler::ReadFile(path)));
  }
}