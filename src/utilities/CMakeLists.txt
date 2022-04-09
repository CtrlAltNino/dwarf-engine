smtg_add_subdirectories()

target_sources(${targetName}
    PRIVATE
        stb.cpp
)

target_precompile_headers(${targetName}
    PRIVATE
        dpch.h
)