cmake_minimum_required (VERSION 2.8.11)
project (BENCHMARK)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)


add_executable (tester src/tester.cpp)
target_link_libraries(tester PUBLIC -static -static-libstdc++)

add_executable (benchmark_dynarmic src/benchmark_dynarmic.cpp)
target_link_libraries(benchmark_dynarmic PUBLIC dynarmic fmt)

add_executable (benchmark_unicorn src/benchmark_unicorn.cpp)
target_link_libraries(benchmark_unicorn PUBLIC -pthread)
target_link_libraries(benchmark_unicorn PUBLIC unicorn)

