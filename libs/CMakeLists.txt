add_library(common_libs STATIC
        CSVFile.h
        random/RandomGenerator.hpp
        random/RandomGenerator.tpp
        evaluation/Evaluation.h
        evaluation/Test1EvaluationFunction.cpp
        evaluation/Test1EvaluationFunction.h
        evaluation/Test2EvaluationFunction.cpp
        evaluation/Test2EvaluationFunction.h
        evaluation/GeneralizedRosenbrock.cpp
        evaluation/GeneralizedRosenbrock.h
        evaluation/Salomon.cpp
        evaluation/Salomon.h
        evaluation/Whitley.cpp
        evaluation/Whitley.h
        solutions/BinarySolution.cpp
        solutions/BinarySolution.h
        solutions/RealSolution.cpp
        solutions/RealSolution.h
)

# Prevents from linker error. Without this CMake fails for header-only files in lib.
set_target_properties(common_libs PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(common_libs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
