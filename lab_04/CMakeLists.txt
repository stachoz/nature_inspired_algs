add_executable(lab_04
    src/main.cpp
)

target_link_libraries(lab_04 PRIVATE common_libs)

target_compile_definitions(lab_04 PRIVATE RESULTS_DIR="/results")

include(GoogleTest)
