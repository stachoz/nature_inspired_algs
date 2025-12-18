add_executable(lab_08
    src/main.cpp
)

target_link_libraries(lab_08 PRIVATE common_libs)

target_compile_definitions(lab_08 PRIVATE RESULTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/results")

include(GoogleTest)
