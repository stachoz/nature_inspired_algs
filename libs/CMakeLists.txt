add_library(common_libs STATIC
        CSVFile.h
        random/RandomGenerator.hpp
        random/RandomGenerator.tpp
)

# Prevents from linker error. Without this CMake fails for header-only files in lib.
set_target_properties(common_libs PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(common_libs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
