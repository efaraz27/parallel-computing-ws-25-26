cmake_minimum_required(VERSION 3.16)
project(ex5 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(pi_seq pi_seq.c)
add_executable(pi_blk pi_blk.c)
add_executable(pi_cyc pi_cyc.c)
add_executable(pi_par_loop pi_par_loop.c)
add_executable(pi_par_critical pi_par_critical.c)

# Try standard OpenMP first (works well with Homebrew GCC)
find_package(OpenMP COMPONENTS C)
if(OpenMP_C_FOUND)
  target_link_libraries(pi_seq PUBLIC OpenMP::OpenMP_C)
  target_link_libraries(pi_blk PUBLIC OpenMP::OpenMP_C)
  target_link_libraries(pi_cyc PUBLIC OpenMP::OpenMP_C)
  target_link_libraries(pi_par_loop PUBLIC OpenMP::OpenMP_C)
  target_link_libraries(pi_par_critical PUBLIC OpenMP::OpenMP_C)
else()
  # AppleClang needs libomp from Homebrew and special flags
  if(APPLE AND CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
    execute_process(
      COMMAND brew --prefix libomp
      OUTPUT_VARIABLE LIBOMP_PREFIX
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_QUIET
    )
    if(LIBOMP_PREFIX)
      target_include_directories(pi_seq PRIVATE "${LIBOMP_PREFIX}/include")
      target_link_directories(pi_seq PRIVATE "${LIBOMP_PREFIX}/lib")
      target_compile_options(pi_seq PRIVATE -Xpreprocessor -fopenmp)
      target_link_libraries(pi_seq PRIVATE omp)
      target_include_directories(pi_blk PRIVATE "${LIBOMP_PREFIX}/include")
      target_link_directories(pi_blk PRIVATE "${LIBOMP_PREFIX}/lib")
      target_compile_options(pi_blk PRIVATE -Xpreprocessor -fopenmp)
      target_link_libraries(pi_blk PRIVATE omp)
      target_include_directories(pi_cyc PRIVATE "${LIBOMP_PREFIX}/include")
      target_link_directories(pi_cyc PRIVATE "${LIBOMP_PREFIX}/lib")
      target_compile_options(pi_cyc PRIVATE -Xpreprocessor -fopenmp)
      target_link_libraries(pi_cyc PRIVATE omp)
      target_include_directories(pi_par_loop PRIVATE "${LIBOMP_PREFIX}/include")
      target_link_directories(pi_par_loop PRIVATE "${LIBOMP_PREFIX}/lib")
      target_compile_options(pi_par_loop PRIVATE -Xpreprocessor -fopenmp)
      target_link_libraries(pi_par_loop PRIVATE omp)
      target_include_directories(pi_par_critical PRIVATE "${LIBOMP_PREFIX}/include")
      target_link_directories(pi_par_critical PRIVATE "${LIBOMP_PREFIX}/lib")
      target_compile_options(pi_par_critical PRIVATE -Xpreprocessor -fopenmp)
      target_link_libraries(pi_par_critical PRIVATE omp)
    else()
      message(FATAL_ERROR "OpenMP not found. On macOS with Apple Clang, run: brew install libomp")
    endif()
  else()
    message(FATAL_ERROR "OpenMP not found. Install OpenMP or use Homebrew GCC and configure with it.")
  endif()
endif()


