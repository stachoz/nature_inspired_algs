add_executable(lab_03
    src/main.cpp
)

target_sources(lab_03 PRIVATE
    src/AlgorithmExperimentRunner.h
    src/AlgorithmExperimentRunner.cpp
    src/LocalSearch.h
    src/SimulatedAnnealing.cpp
    src/SimulatedAnnealing.h
    src/LocalSearch.cpp
    src/solutions/Solution.h
    src/solutions/BinarySolution.cpp
    src/solutions/BinarySolution.h
    src/solutions/RealSolution.cpp
    src/solutions/RealSolution.h
    src/neighborhood/Neighborhood.h
    src/neighborhood/DefaultBinaryNeighborhood.cpp
    src/neighborhood/DefaultBinaryNeighborhood.h
    src/neighborhood/DefaultRealNeighborhood.h
    src/neighborhood/DefaultRealNeighborhood.cpp
    src/evaluation/Evaluation.h
    src/evaluation/Test1EvaluationFunction.cpp
    src/evaluation/Test1EvaluationFunction.h
    src/evaluation/Test2EvaluationFunction.cpp
    src/evaluation/Test2EvaluationFunction.h
)

target_include_directories(lab_03 PRIVATE src)

target_link_libraries(lab_03 PRIVATE common_libs)

target_compile_definitions(lab_03 PRIVATE RESULTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/results")

include(GoogleTest)
