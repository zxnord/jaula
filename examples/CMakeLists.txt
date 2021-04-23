cmake_minimum_required(VERSION 3.0)

include_directories(${jaula_SOURCE_DIR})

add_executable(jparse jparse.cc)
target_link_libraries(jparse PUBLIC jaula)

add_executable(lextst lextst.cc)
target_link_libraries(lextst PUBLIC jaula)