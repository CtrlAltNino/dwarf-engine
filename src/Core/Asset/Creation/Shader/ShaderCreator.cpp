#include "pch.h"

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Platform/OpenGL/OpenGLUtilities.h"
#include "ShaderCreator.h"

namespace Dwarf
{
  ShaderCreator::ShaderCreator(const AssetDirectoryPath&     assetDirectoryPath,
                               std::shared_ptr<IFileHandler> fileHandler)
    : mAssetDirectoryPath(assetDirectoryPath)
    , mFileHandler(std::move(fileHandler))
  {
  }

  void
  ShaderCreator::CreateShaderAsset(
    ShaderType                           type,
    ShaderSource                         source,
    std::optional<std::filesystem::path> directoryPath)
  {
    std::filesystem::path shaderSourceDirectory =
      OpenGLUtilities::GetDefaultShaderPath();
    std::filesystem::path shaderTargetDirectory =
      directoryPath.has_value() ? directoryPath.value() : mAssetDirectoryPath.t;
    std::filesystem::path shaderSourcePath;
    std::filesystem::path shaderTargetPath;
    std::string           fileName;
    std::string           fileExtension;

    switch (source)
    {
      using enum ShaderSource;
      case PbrSource:
        fileName = "pbr";
        shaderSourceDirectory = OpenGLUtilities::GetPbrShaderPath();
        break;
      case UnlitSource:
        fileName = "unlit";
        shaderSourceDirectory = OpenGLUtilities::GetUnlitShaderPath();
        break;
    }

    switch (type)
    {
      using enum ShaderType;
      case VertexType: fileExtension = ".vert"; break;
      case FragmentType: fileExtension = ".frag"; break;
    }

    shaderSourcePath = shaderSourceDirectory / (fileName + fileExtension);
    shaderTargetPath = shaderTargetDirectory / (fileName + fileExtension);
    int counter = 0;
    while (mFileHandler->FileExists(shaderTargetPath))
    {
      shaderTargetPath =
        shaderTargetDirectory / (fileName.append("(")
                                   .append(std::to_string(counter))
                                   .append(")")
                                   .append(fileExtension));
      counter++;
    }

    if (mFileHandler->FileExists(shaderSourcePath))
    {
      mFileHandler->Copy(shaderSourcePath, shaderTargetPath);
    }
  }
} // namespace Dwarf