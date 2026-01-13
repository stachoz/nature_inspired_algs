add_executable(lab_09
    src/main.cpp
)

target_link_libraries(lab_09 PRIVATE common_libs)

target_compile_definitions(lab_09 PRIVATE RESULTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/results")

include(GoogleTest)
