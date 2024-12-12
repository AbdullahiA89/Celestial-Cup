cmake_minimum_required(VERSION 3.11)

# Require modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Celestial_Cup)

#### Setup Directories ####
# Main output directory
set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

#### Copy Resources to Build Directory ####
add_custom_target(copy_resources ALL 
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/res"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res"
)

#### Source Code ####
file(GLOB_RECURSE SOURCES sourcecode/*.cpp sourcecode/*.h)
add_executable(SOURCECODE ${SOURCES})
target_include_directories(SOURCECODE PRIVATE ${SFML_INCS})
target_link_libraries(SOURCECODE PRIVATE sfml-graphics sfml-window sfml-system)
target_link_libraries(SOURCECODE PRIVATE sfml-graphics sfml-window sfml-system)

# Ensure resources are copied for runtime
add_dependencies(SOURCECODE copy_resources)

