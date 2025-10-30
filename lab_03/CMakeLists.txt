add_executable(lab_03
        src/main.cpp
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
)

target_link_libraries(lab_03 PRIVATE common_libs)

target_compile_definitions(lab_03 PRIVATE RESULTS_DIR="/results")

include(GoogleTest)
