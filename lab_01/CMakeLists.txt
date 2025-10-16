add_executable(lab_01
        src/main.cpp
)

target_sources(lab_01 PUBLIC
        src/test.hpp
        src/CircleAreaExp.hpp
        src/RandomGeneratorExample.hpp
)

target_link_libraries(lab_01 PRIVATE
        common_libs
)

target_compile_definitions(lab_01 PRIVATE RESULTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/results")

include(GoogleTest)
