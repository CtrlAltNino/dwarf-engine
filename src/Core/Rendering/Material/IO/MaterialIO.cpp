#include "MaterialIO.h"
#include "Utilities/FileHandler.h"
#include <string>

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
  MaterialIO::SaveMaterial(std::shared_ptr<IMaterial>   material,
                           std::filesystem::path const& path)
  {
    // Save the material to a file (Overwrite if it already exists)
    m_Logger->LogInfo(Log(
      fmt::format("Saving material to file: {}", path.string()), "MaterialIO"));
    std::string serializedMaterial = material->Serialize().dump(2);
    m_Logger->LogInfo(
      Log(fmt::format("Serialized material: {}", serializedMaterial),
          "MaterialIO"));
    FileHandler::WriteToFile(path, serializedMaterial);
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

    std::string serializedMaterial = FileHandler::ReadFile(path);
    m_Logger->LogInfo(
      Log(fmt::format("Serialized material: {}", serializedMaterial),
          "MaterialIO"));

    return m_MaterialFactory->FromSerialized(
      nlohmann::json::parse(serializedMaterial));
  }
}