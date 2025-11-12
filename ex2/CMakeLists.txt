cmake_minimum_required(VERSION 3.16)
project(ex2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(hello hello.c)
add_executable(counts3s counts3s.c)
add_executable(matrix_vector matrix_vector.c)

# Try standard OpenMP first (works well with Homebrew GCC)
find_package(OpenMP COMPONENTS C)
if(OpenMP_C_FOUND)
  target_link_libraries(hello PUBLIC OpenMP::OpenMP_C)
  target_link_libraries(counts3s PUBLIC OpenMP::OpenMP_C)
  target_link_libraries(matrix_vector PUBLIC OpenMP::OpenMP_C)
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
      target_include_directories(hello PRIVATE "${LIBOMP_PREFIX}/include")
      target_include_directories(counts3s PRIVATE "${LIBOMP_PREFIX}/include")
      target_include_directories(matrix_vector PRIVATE "${LIBOMP_PREFIX}/include")
      target_link_directories(hello PRIVATE "${LIBOMP_PREFIX}/lib")
      target_link_directories(counts3s PRIVATE "${LIBOMP_PREFIX}/lib")
      target_link_directories(matrix_vector PRIVATE "${LIBOMP_PREFIX}/lib")
      target_compile_options(hello PRIVATE -Xpreprocessor -fopenmp)
      target_compile_options(counts3s PRIVATE -Xpreprocessor -fopenmp)
      target_compile_options(matrix_vector PRIVATE -Xpreprocessor -fopenmp)
      target_link_libraries(hello PRIVATE omp)
      target_link_libraries(counts3s PRIVATE omp)
      target_link_libraries(matrix_vector PRIVATE omp)
    else()
      message(FATAL_ERROR "OpenMP not found. On macOS with Apple Clang, run: brew install libomp")
    endif()
  else()
    message(FATAL_ERROR "OpenMP not found. Install OpenMP or use Homebrew GCC and configure with it.")
  endif()
endif()


