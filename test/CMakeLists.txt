find_package(GTest REQUIRED)

add_executable(test_ni main.cpp)
target_link_libraries(test_ni ni::ni TBB::tbb GTest::gtest_main)
target_compile_options(test_ni PRIVATE -Wall -Wextra -pedantic)
