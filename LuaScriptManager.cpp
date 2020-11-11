#include "LuaScriptManager.h"
#include "VarList.h"

#include <iostream>
#include <mutex>
#include <cassert>
#include <string>

LuaScriptManager *g_LuaScriptManager = nullptr;

std::mutex LuaScriptManagerMutex;

LuaScriptManager *GetOrNewLuaScriptManger()
{
    if (g_LuaScriptManager)
    {
        return g_LuaScriptManager;
    }

    std::lock_guard<std::mutex> guard(LuaScriptManagerMutex);
    if (g_LuaScriptManager)
    {
        return g_LuaScriptManager;
    }
    
    g_LuaScriptManager = new LuaScriptManager;

    return g_LuaScriptManager;
}

LuaScriptManager::LuaScriptManager()
{
    
}

void LuaScriptManager::DeclLuaFunc(const std::string &funcName, LuaFunc func)
{
    auto it = m_Func.find(funcName);
    if (it == m_Func.end())
    {
        m_Func[funcName] = func;
    }
}

void LuaScriptManager::LoadLuaFile(std::string path, std::string fileName)
{
    if (fileName.empty())
    {
        return ;
    }

    if(path.empty())
    {
        path = "./";
    }
    
    if(m_LuaScript.find(fileName) != m_LuaScript.end())
    {
        std::cerr << "mutil load file: " << fileName << std::endl;
        return ;
    }

    LuaScript *pLuaScript = nullptr;
    try
    {
        pLuaScript = new LuaScript(path, fileName);
        if (pLuaScript == nullptr)
        {
            std::cerr << "new luaScript fail file name: " << fileName << std::endl;
            assert(false);
        }
    }
    catch(int e)
    {
        std::cerr << "new luaScript fail error : " << e << ", file name: " << fileName << std::endl;
        assert(false);
    }

    this->RegisLuaFunc(pLuaScript);

   m_LuaScript[fileName] = pLuaScript;
}


bool LuaScriptManager::RunLuaFunc(const std::string &fileName, const std::string &funcName, 
                        const IVarList &varList, int result, IVarList *pResultList)
{
    if (fileName.empty() || funcName.empty())
    {
        return false;
    }

    auto it = m_LuaScript.find(fileName);
    if (it == m_LuaScript.end())
    {
        std::cerr << "not find file: " << fileName << std::endl;
        return false;
    }

    LuaScript* pLuaScript = it->second;
    if (pLuaScript != nullptr 
        && pLuaScript->FuncExists(funcName))
    {
        pLuaScript->ExceLuaScript(funcName, varList, result, pResultList);
    }

    // TODO 返回值 进行修正
    return true;
}


bool LuaScriptManager::RegisLuaFunc(LuaScript *pLuaScript)
{
    if (pLuaScript == nullptr)
    {
        return false;
    }

    for (auto it = m_Func.begin(); it != m_Func.end(); ++it)
    {
        pLuaScript->RegisLuaFunc(it->first, it->second);
    }

    return true;
}
 
bool RunLuaFunc(const std::string &fileName, const std::string &func,
        const IVarList &varList, int resultCount, IVarList *pResultList)
 {
    LuaScriptManager *pManager = GetOrNewLuaScriptManger();

    return pManager->RunLuaFunc(fileName, func, varList, resultCount, pResultList);
 }
