cmake_minimum_required(VERSION 2.4)

project(RubikSolver)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(HEADER_FILES src/Cube.h src/Face.h src/Constants.h src/ActionChain.h src/rlutil.h)
set(SOURCE_FILES src/Cube.cpp src/Face.cpp src/ActionChain.cpp)

add_executable(RubikSolver src/Main.cpp ${HEADER_FILES} ${SOURCE_FILES})