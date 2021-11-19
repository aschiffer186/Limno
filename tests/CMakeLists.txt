# Ndarray tests 
set(MatrixTestFiles Matrix/TestMatrixBase.cpp)
# C++ 20 Test
add_executable(TestMatrixBaseExec ${MatrixTestFiles})
target_compile_features(TestMatrixBaseExec PRIVATE cxx_std_20)
target_link_libraries(TestMatrixBaseExec PRIVATE gtest_main)
target_include_directories(TestMatrixBaseExec PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_compile_options(TestMatrixBaseExec PRIVATE "-g" "-pedantic" "-Wall" "-Werror" "-fconcepts-diagnostics-depth=2")
add_test(NAME TestMatrixBase COMMAND TestMatrixBaseExec)
# C++ 17 Test
add_executable(TestMatrixBaseExec17 ${MatrixTestFiles})
target_compile_features(TestMatrixBaseExec17 PRIVATE cxx_std_17)
target_link_libraries(TestMatrixBaseExec17 PRIVATE gtest_main)
target_include_directories(TestMatrixBaseExec17 PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_compile_options(TestMatrixBaseExec17 PRIVATE "-g" "-pedantic" "-Wall" "-Werror")
add_test(NAME TestMatrixBase17 COMMAND TestMatrixBaseExec17)
