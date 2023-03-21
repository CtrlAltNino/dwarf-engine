smtg_add_subdirectories()

target_precompile_headers(${targetName}
    PRIVATE
        dpch.h
)

target_sources(${targetName}
    PRIVATE
        EntryPoint.cpp
)