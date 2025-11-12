add_executable(lab_04
    src/main.cpp
    src/genetic_genetic.h
)

target_link_libraries(lab_04 PRIVATE common_libs)

target_compile_definitions(lab_04 PRIVATE RESULTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/results")

include(GoogleTest)
