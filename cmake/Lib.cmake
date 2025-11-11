add_library(
    core-math 
    STATIC 
        src/curveinterpolation.cpp 
        src/quadratures.cpp 
        src/optim.cpp
        src/errors.cpp
        src/probability/distributions.cpp
        src/probability/sampling.cpp
        src/loss.cpp
        src/regression.cpp
        src/tools.cpp)
target_link_libraries(core-math PUBLIC Eigen3::Eigen)
target_include_directories(core-math PUBLIC include)
