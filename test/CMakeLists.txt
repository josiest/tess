# compile and run tests

enable_testing()
find_package(GTest REQUIRED)
if (NOT GTest_FOUND)
    message(FATAL_ERROR "google-test wasn't found")
endif()

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(run_tests ${TEST_SOURCES})
include_directories(../src)
target_link_libraries(run_tests GTest::GTest GTest::Main)
