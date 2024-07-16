cmake_minimum_required(VERSION 3.10)
project(testproject)

# 设置默认的编译标志
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

#c++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#包含目录
include_directories(include)

#源文件
set(SOURCE_FILES_COMMON src/AES.cpp
			src/diff_hellman.cpp
			src/exchange.cpp
			src/func.cpp)

#可执行文件
add_executable(server src/server.cpp ${SOURCE_FILES_COMMON})

add_executable(client src/client.cpp ${SOURCE_FILES_COMMON})

add_executable(midman src/midman.cpp ${SOURCE_FILES_COMMON})
