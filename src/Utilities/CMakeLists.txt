smtg_add_subdirectories()
target_sources(${libname}
    PRIVATE
    stb.cpp
)

if(APPLE)
    target_sources(${libname}
        PRIVATE
        metal.cpp
    )
endif()