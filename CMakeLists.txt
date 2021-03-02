cmake_minimum_required (VERSION 2.8.11)
project (BENCHMARK)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# Add the module directory to the list of paths
#list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules")
#find_package(Unicorn REQUIRED)

add_executable (tester src/tester.cpp)
target_link_libraries(tester PUBLIC -static -static-libstdc++)

add_executable (benchmark_dynarmic src/benchmark_dynarmic.cpp)
target_link_libraries(benchmark_dynarmic PUBLIC -static -static-libstdc++)
target_link_libraries(benchmark_dynarmic PUBLIC dynarmic fmt)


#target_link_libraries(hello PUBLIC unicorn pthread)
