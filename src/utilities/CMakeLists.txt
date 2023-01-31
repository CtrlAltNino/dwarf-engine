smtg_add_subdirectories()

target_sources(${targetName}
    PRIVATE
        stb.cpp
        metal.cpp
        metalHelper.mm
)

target_precompile_headers(${targetName}
    PRIVATE
        dpch.h
)