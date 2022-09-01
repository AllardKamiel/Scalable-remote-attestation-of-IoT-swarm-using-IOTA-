cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial LANGUAGES C)

include(FetchContent)
# add the executable
add_executable(${PROJECT_NAME} main.c)
set(SHARED_OBJECT_FILE libiota_streams_c.so)
target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/${SHARED_OBJECT_FILE})