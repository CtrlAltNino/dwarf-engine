smtg_add_subdirectories()

target_sources(${targetName}
    PRIVATE
    AssetDirectoryListener.cpp
    AssetDatabase.cpp
    MaterialSerializer.cpp
    ModelImporter.cpp
)