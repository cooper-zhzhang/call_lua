# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.16.3)

# 项目信息
project (LuaScript)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall  -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -Wall")

# 指定生成目标
add_executable(main main.cpp LuaScript.cpp LuaScriptManager.cpp VarList.cpp)

target_link_libraries(main lua)
target_link_libraries(main gflags)

add_library(run_lua LuaScript.cpp LuaScriptManager.cpp VarList.cpp)

target_link_libraries(run_lua lua)

add_executable(testlua TestRunLuaLib.cpp)
target_link_libraries(testlua run_lua)
