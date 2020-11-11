#pragma once

#include "VarList.h"

#include <iostream>
#include <string>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>
}

typedef int (*LuaFunc)(lua_State *L) ;

class LuaScript
{
public:
    LuaScript(const std::string &path, const std::string &fileName);
    // TODO 参数和返回值都要使用变长的list
    bool ExceLuaScript(std::string funName, const IVarList &varList, int resultCount, IVarList *pResultList);

    bool FuncExists(std::string funcName);

    bool RegisLuaFunc(std::string funcName, LuaFunc func);

    lua_State* GetLuaState();


private:

    lua_State *m_LuaState; 
    std::string m_LuaFile;
};

bool LuaGetVarList(lua_State *state, int beg, int end, IVarList &msg);

