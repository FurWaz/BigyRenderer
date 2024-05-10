cmake_minimum_required(VERSION 3.12)
project(BigyRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add your source files here
file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS src/*.hpp)

# Add the include directory
include_directories(include)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link any libraries or dependencies
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
