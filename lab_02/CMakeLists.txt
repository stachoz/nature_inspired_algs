add_executable(lab_02
    src/main.cpp
        src/LocalSearch.hpp
        src/FirstImprovementLocalSearch.hpp
)

target_link_libraries(lab_02 PRIVATE common_libs)

target_compile_definitions(lab_01 PRIVATE RESULTS_DIR="/results")

include(GoogleTest)
