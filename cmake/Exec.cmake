# Interpolatiopn tests
add_executable(test1 ${CMAKE_CURRENT_SOURCE_DIR}/tests/test1.cpp)
target_link_libraries(test1 PUBLIC cpp-math)

# Quadrature tests
add_executable(test2 ${CMAKE_CURRENT_SOURCE_DIR}/tests/test2.cpp)
target_link_libraries(test2 PUBLIC cpp-math)

# Optimization tests
add_executable(test3 ${CMAKE_CURRENT_SOURCE_DIR}/tests/test3.cpp)
target_link_libraries(test3 PUBLIC cpp-math)

# Probability distribution tests
add_executable(test4 ${CMAKE_CURRENT_SOURCE_DIR}/tests/test4.cpp)
target_link_libraries(test4 PUBLIC cpp-math)

# Sampling distribution tests
add_executable(test5 ${CMAKE_CURRENT_SOURCE_DIR}/tests/test5.cpp)
target_link_libraries(test5 PUBLIC cpp-math)