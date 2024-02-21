smtg_add_subdirectories()
target_sources(${targetName}
    PRIVATE
    stb.cpp
    FileHandler.cpp
)

if(APPLE)
    target_sources(${targetName}
        PRIVATE
        metal.cpp
    )
endif()