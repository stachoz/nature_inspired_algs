add_executable(lab_05
    src/main.cpp
)

target_link_libraries(lab_05 PRIVATE common_libs)

target_compile_definitions(lab_05 PRIVATE RESULTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/results")

include(GoogleTest)
