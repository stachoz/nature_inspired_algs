add_executable(lab_03
    src/main.cpp
)

target_link_libraries(lab_03 PRIVATE common_libs)

target_compile_definitions(lab_03 PRIVATE RESULTS_DIR="/results")

include(GoogleTest)
