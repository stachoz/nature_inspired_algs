add_executable(lab_03
    src/main.cpp
)

target_sources(lab_03 PRIVATE
    src/AlgorithmExperimentRunner.h
    src/Evaluation.h
    src/LocalSearch.h
    src/Neighborhood.h
    src/Solution.h
    src/SimulatedAnnealing.cpp
    src/SimulatedAnnealing.h
    src/LocalSearch.cpp
    src/EncodingType.h
    src/Solution.cpp
    src/BinarySolution.cpp
    src/BinarySolution.h
    src/RealSolution.cpp
    src/RealSolution.h
    src/DefaultBinaryNeighborhood.cpp
    src/DefaultBinaryNeighborhood.h
    src/DefaultRealNeighborhood.cpp
    src/DefaultRealNeighborhood.h
        src/Test1EvaluationFunction.cpp
        src/Test1EvaluationFunction.h
        src/Test2EvaluationFunction.cpp
        src/Test2EvaluationFunction.h
)

target_link_libraries(lab_03 PRIVATE common_libs)

target_compile_definitions(lab_03 PRIVATE RESULTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/results")

include(GoogleTest)
