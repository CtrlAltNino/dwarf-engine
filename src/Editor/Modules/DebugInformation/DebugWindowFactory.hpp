#pragma once

#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Editor/Modules/DebugInformation/IDebugWindowFactory.hpp"
#include "Logging/IDwarfLogger.hpp"
#include <boost/di.hpp>

namespace Dwarf
{
  class DebugWindowFactory : public IDebugWindowFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>   mLogger;
    std::shared_ptr<IAssetDatabase> mAssetDatabase;

  public:
    DebugWindowFactory(std::shared_ptr<IDwarfLogger>   logger,
                       std::shared_ptr<IAssetDatabase> assetDatabase);
    ~DebugWindowFactory() override;

    /**
     * @brief Creates a default DebugWindow instance
     *
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] auto
    Create() const -> std::unique_ptr<DebugWindow> override;

    /**
     * @brief Creates a DebugWindow instance based off serialized data
     *
     * @param serializedModule Serialized data of a DebugWindow
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] auto
    Create(SerializedModule serializedModule) const
      -> std::unique_ptr<DebugWindow> override;
  };
}