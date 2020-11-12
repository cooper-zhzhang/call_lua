# CMake 最低版本号要求 最低版本非必须
CMAKE_MINIMUM_REQUIRED (VERSION 3.16.3)

# 项目信息
PROJECT(LuaScript)

SET(CMAKE_C_COMPILER "/usr/bin/gcc")
SET(CMAKE_CXX_COMPILER "/usr/bin/g++")

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -w")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall  -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -Wall")

# 指定生成目标
ADD_EXECUTABLE(main main.cpp LuaScript.cpp LuaScriptManager.cpp VarList.cpp)

TARGET_LINK_LIBRARIES(main lua)
TARGET_LINK_LIBRARIES(main gflags)

ADD_LIBRARY(run_lua LuaScript.cpp LuaScriptManager.cpp VarList.cpp)

TARGET_LINK_LIBRARIES(run_lua lua)

ADD_EXECUTABLE(testlua TestRunLuaLib.cpp)
TARGET_LINK_LIBRARIES(testlua run_lua)

# 安装库到系统中
SET(CMAKE_INSTALL_PREFIX /usr)
INSTALL(FILES LuaScript.h VarList.h IVarList.h LuaScriptManager.h DESTINATION include)
INSTALL(TARGETS run_lua LIBRARY DESTINATION lib)