cmake_minimum_required(VERSION 3.10)

project(Flexomata CXX)

add_library(${PROJECT_NAME} STATIC
    src/flexomata.cpp
    src/grid.cpp
    src/configloader.cpp
    src/deferred_configloader.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
