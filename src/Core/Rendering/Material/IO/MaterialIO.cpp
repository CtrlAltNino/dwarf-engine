#include "MaterialIO.h"
#include <fmt/format.h>
#include <string>

namespace Dwarf
{
  MaterialIO::MaterialIO(std::shared_ptr<IMaterialFactory> materialFactory,
                         std::shared_ptr<IDwarfLogger>     logger,
                         std::shared_ptr<IFileHandler>     fileHandler)
    : mMaterialFactory(materialFactory)
    , mLogger(logger)
    , mFileHandler(fileHandler)
  {
  }

  MaterialIO::~MaterialIO() {}

  void
  MaterialIO::SaveMaterial(IMaterial&                   material,
                           const std::filesystem::path& path) const
  {
    // Save the material to a file (Overwrite if it already exists)
    mLogger->LogDebug(Log(
      fmt::format("Saving material to file: {}", path.string()), "MaterialIO"));
    std::string serializedMaterial = material.Serialize().dump(2);
    mLogger->LogDebug(
      Log(fmt::format("Serialized material: {}", serializedMaterial),
          "MaterialIO"));
    mFileHandler->WriteToFile(path, serializedMaterial);
  }

  std::unique_ptr<IMaterial>
  MaterialIO::LoadMaterial(const std::filesystem::path& path) const
  {
    mLogger->LogDebug(
      Log(fmt::format("Loading material from file: {}", path.string()),
          "MaterialIO"));

    if (!mFileHandler->FileExists(path))
    {
      mLogger->LogError(Log(
        fmt::format("Material asset file does not exist: {}", path.string()),
        "MaterialIO"));
      throw std::runtime_error("Material asset file does not exist");
    }

    std::string serializedMaterial = mFileHandler->ReadFile(path);
    mLogger->LogDebug(
      Log(fmt::format("Serialized material: {}", serializedMaterial),
          "MaterialIO"));

    return mMaterialFactory->FromSerialized(
      nlohmann::json::parse(serializedMaterial));
  }
}