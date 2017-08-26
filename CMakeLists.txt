cmake_minimum_required (VERSION 2.6)
project (PokerBot)

include_directories("${PROJECT_SOURCE_DIR}/Include")
include_directories("${PROJECT_SOURCE_DIR}/project_acpc_server")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")

subdirs(src)
subdirs(project_acpc_server)
