add_executable(coremath-curveinterpolation ${CMAKE_CURRENT_SOURCE_DIR}/tests/curveinterpolation.cpp)
target_link_libraries(coremath-curveinterpolation PUBLIC core-math)

add_executable(coremath-optim ${CMAKE_CURRENT_SOURCE_DIR}/tests/optim.cpp)
target_link_libraries(coremath-optim PUBLIC core-math)

add_executable(coremath-probability-distributions ${CMAKE_CURRENT_SOURCE_DIR}/tests/probability_distributions.cpp)
target_link_libraries(coremath-probability-distributions PUBLIC core-math)

add_executable(coremath-probability-sampling ${CMAKE_CURRENT_SOURCE_DIR}/tests/probability_sampling.cpp)
target_link_libraries(coremath-probability-sampling PUBLIC core-math)

add_executable(coremath-quadratures ${CMAKE_CURRENT_SOURCE_DIR}/tests/quadratures.cpp)
target_link_libraries(coremath-quadratures PUBLIC core-math)

add_executable(coremath-regression ${CMAKE_CURRENT_SOURCE_DIR}/tests/regression.cpp)
target_link_libraries(coremath-regression PUBLIC core-math)