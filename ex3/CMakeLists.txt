cmake_minimum_required(VERSION 3.16)
project(ex3 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(exercise_three exeercise_three.c)

# Try standard OpenMP first (works well with Homebrew GCC)
find_package(OpenMP COMPONENTS C)
if(OpenMP_C_FOUND)
  target_link_libraries(exercise_three PUBLIC OpenMP::OpenMP_C)
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
      target_include_directories(exercise_three PRIVATE "${LIBOMP_PREFIX}/include")
      target_link_directories(exercise_three PRIVATE "${LIBOMP_PREFIX}/lib")
      target_compile_options(exercise_three PRIVATE -Xpreprocessor -fopenmp)
      target_link_libraries(exercise_three PRIVATE omp)
    else()
      message(FATAL_ERROR "OpenMP not found. On macOS with Apple Clang, run: brew install libomp")
    endif()
  else()
    message(FATAL_ERROR "OpenMP not found. Install OpenMP or use Homebrew GCC and configure with it.")
  endif()
endif()


