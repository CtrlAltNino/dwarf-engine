smtg_add_subdirectories()

target_sources(${testTarget}
    PRIVATE
    AssetDatabaseTests.cpp
    AssetDirectoryListenerTests.cpp
    AssetMetadataTests.cpp
    MaterialSerializerTests.cpp
    ModelImporterTests.cpp
    ShaderRecompilerTests.cpp
)