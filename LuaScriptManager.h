#pragma once

#include <map>
#include <string>
#include "LuaScript.h"
#include "VarList.h"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}


 bool RunLuaFunc(const std::string &fileName, const std::string &func,
const IVarList &varList = CVarList(), int resultCount = 0, IVarList *pResultList = nullptr);

class LuaScriptManager
{
public:
    LuaScriptManager();

    void DeclLuaFunc(const std::string &funcName, LuaFunc func);

    void LoadLuaFile(std::string path, std::string fileName);

    bool RunLuaFunc(const std::string &fileName, const std::string &funcName,
    const IVarList &varList = CVarList(), int result = 0, IVarList *pResultList = nullptr);

private:

    bool RegisLuaFunc(LuaScript *pLuaScript);

    std::map<std::string, LuaFunc> m_Func;

    std::map<std::string, LuaScript*> m_LuaScript;
};

LuaScriptManager *GetOrNewLuaScriptManger();

#define DECL_LUA_FUNC(func)   \
    do                          \
    {                           \
        LuaScriptManager *pManager = GetOrNewLuaScriptManger(); \
        pManager->DeclLuaFunc(#func, func);\
    } while (false);


#define LOAD_LUA_FILE(path, file)     \
    do                          \
    {                           \
    LuaScriptManager *pManager = GetOrNewLuaScriptManger();\
    pManager->LoadLuaFile(path, file);    \
    } while (false);

#define RUN_LUA_FUNC(fileName, func)    \
    do                                  \
    {                                   \
    LuaScriptManager *pManager = GetOrNewLuaScriptManger();\
    pManager->RunLuaFunc(fileName, func);   \
    } while (false);
