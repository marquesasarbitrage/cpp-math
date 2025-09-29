add_library(
    cpp-math 
    STATIC 
        src/curveinterpolation.cpp 
        src/quadratures.cpp 
        src/optim.cpp
        src/errors.cpp)
target_link_libraries(cpp-math PUBLIC Eigen3::Eigen)
target_include_directories(cpp-math PUBLIC include)
