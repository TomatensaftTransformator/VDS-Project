project(VDSProject_test CXX C)
cmake_minimum_required(VERSION 2.8)


add_executable(VDSProject_test main_test.cpp)
target_link_libraries(VDSProject_test Manager)
target_link_libraries(VDSProject_test gtest gtest_main pthread)



include(CTest)
enable_testing()

add_test(unit ${PROJECT_BINARY_DIR}/VDSProject_test)
