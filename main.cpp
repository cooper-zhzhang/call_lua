#include "LuaScript.h"
#include "LuaScriptManager.h"
#include "VarList.h"

#include <iostream>
#include <gflags/gflags.h>

extern "C"
{
#include <stdio.h>
}

DEFINE_string(path, "../", "path of lua file");
DEFINE_string(file_name, "lua_script/Data.lua", "lua file name");
DEFINE_string(func_name, "echo_hello", "lau func name");
DEFINE_int32(var_list_len, 100, "");

//TODO:"2.0版本解决问题:不能加载相对路径的文件 需要更改加载lua的接口 自己对文件 将file 传给lua虚拟机"

extern "C"
{
    int Print(lua_State *l)
    {
        printf("hello, no wolrd\n");
        return 0;
    }

    int PrintX(lua_State *l)
    {
        printf("hello, world x\n");
        return 0;
    }
}


int TestLuaScript()
{
    LuaScript *pLuaScript = NULL;
    try
    {
        std::string path = FLAGS_path;
        std::string file = FLAGS_file_name;
        pLuaScript = new LuaScript(path, file);
        if (pLuaScript == NULL)
        {
            std::cerr << "new luaScript fail file name: " << FLAGS_file_name << std::endl;
            return 0;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;;
        return 0;
    }
    catch(int e)
    {
        std::cerr << "new luaScript fail error : " << e 
            << ", file name: " << FLAGS_file_name << std::endl;

        return 0;
    }

    std::string funcName = FLAGS_func_name;
    if (!pLuaScript->FuncExists(funcName))
    {
        std::cerr << "func not exists " << funcName << std::endl;
        return 0;
    }

    //pLuaScript->ExceLuaScript(funcName);

    return 0;
}


void TestLuaScriptManager()
{
    DECL_LUA_FUNC(Print);
    DECL_LUA_FUNC(PrintX);
    DECL_LUA_FUNC(PrintX);

    LOAD_LUA_FILE(FLAGS_path, FLAGS_file_name);

    // 直接运行一个lua中的函数
    RUN_LUA_FUNC(FLAGS_file_name.c_str(), FLAGS_func_name.c_str());

    RUN_LUA_FUNC(FLAGS_file_name.c_str(), "hello");
    CVarList varList;
    varList << "wahaha";

    RunLuaFunc(FLAGS_file_name.c_str(), "hello", CVarList() << "world");
    RunLuaFunc(FLAGS_file_name.c_str(), "hello", varList);


    CVarList result;
    bool runResult = RunLuaFunc(FLAGS_file_name.c_str(), "add_three", CVarList() << 1 << 2 << 3, 1, &result);
    if (runResult)
    {
        std::cout << result.IntValue(0) << std::endl;
    }
}

void Tranfter(IVarList &msg)
{   
    msg.AddDouble(11);
}

void TranfterPtr1(IVarList *p)
{
    Tranfter(*p);
}

void TranfterPtr(IVarList *p)
{
    TranfterPtr1(p);
}

void TestVarLIst()
{
    
    //CVarList();
    CVarList varList;
    //TVarList<64> ;
    //CVarList varList();

    for (int i = 0; i < FLAGS_var_list_len; i++)
    {
        varList.AddInt(1);
        varList.AddInt64(100L);
        varList.AddStr("cccc");
    }

    std::cout << "value count: " << varList.GetCount() << std::endl;

    for (int i = 0; i < FLAGS_var_list_len; i=i+3)
    {
        std::cout << varList.IntValue(i) << " " << 
        varList.Int64Value(i+1) << " " << 
        varList.StrValue(i+2) << std::endl;
    }

    IVarList *p = &varList;
    IVarList &ref = *p;

    std::cout << ref.AddDouble(11.0);

    TranfterPtr(p);
    //Tranfter(*p);
}

int main(int argc, char **argv)
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    TestLuaScriptManager();

    //TestVarLIst();

    return 0;
}
