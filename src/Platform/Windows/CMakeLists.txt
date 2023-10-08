IF(WIN32)
    target_sources(${targetName}
        PRIVATE
        WindowsWindow.cpp
    )
ENDIF()