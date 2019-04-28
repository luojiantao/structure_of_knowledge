cmake_minimum_required(VERSION 2.8)
project(hello_world)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++0x -g")
#SET(CMAKE_BUILD_TYPE Release)
#配置安装目录(make install)
#file(GLOB INCLUDE_FILES "json/*.h")
#install(FILES ${INCLUDE_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/json)

#指定执行路径
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

#指定库输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/Thread
        )
add_executable(hello_world Threaddemo.cpp Thread/Queue.cpp Thread/ThreadPool.cpp)
target_link_libraries(hello_world pthread)
#target_link_libraries(hello_world profiler tcmalloc pthread)

