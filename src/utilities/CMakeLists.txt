smtg_add_subdirectories()
target_sources(${targetName}
    PRIVATE
        stb.cpp
        FileHandler.cpp
)

IF (APPLE)
    target_sources(${targetName}
        PRIVATE
            metal.cpp
            metalHelper.mm
    )
ENDIF ()