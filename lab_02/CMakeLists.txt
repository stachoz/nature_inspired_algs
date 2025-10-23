add_executable(lab_02
    src/main.cpp
)

target_sources(lab_02 PUBLIC
        src/ILocalSearch.h
        src/FirstImprovementLocalSearch.hpp
)

target_link_libraries(lab_02 PRIVATE common_libs)

target_compile_definitions(lab_02 PRIVATE RESULTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/results")

include(GoogleTest)
