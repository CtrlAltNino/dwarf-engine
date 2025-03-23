#pragma once

#include "Editor/Modules/IGuiModule.h"

namespace Dwarf
{
  /**
   * @brief Factory class that creates desired gui modules
   *
   */
  class IGuiModuleFactory
  {
  public:
    virtual ~IGuiModuleFactory() = default;

    /**
     * @brief Creates a gui module based on the specified type
     *
     * @param type Gui module type
     * @return Unique pointer to the created gui module
     */
    [[nodiscard]] virtual auto
    CreateGuiModule(MODULE_TYPE type) const -> std::unique_ptr<IGuiModule> = 0;

    /**
     * @brief Creates a gui module based on a serialized structures
     *
     * @param serializedModule Serialized module json
     * @return Unique pointer to the created gui module
     */
    [[nodiscard]] virtual auto
    CreateGuiModule(SerializedModule serializedModule) const
      -> std::unique_ptr<IGuiModule> = 0;
  };
} // namespace Dwarf