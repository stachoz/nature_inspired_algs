add_executable(lab_10
        src/main.cpp
)

target_link_libraries(lab_10 PRIVATE common_libs)

# Zmieniamy "/results" na dynamiczną ścieżkę projektu:
target_compile_definitions(lab_10 PRIVATE RESULTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/results")

include(GoogleTest)