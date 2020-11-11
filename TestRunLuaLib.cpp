#include <iostream>

#include "LuaScriptManager.h"

using namespace std;

int main()
{
    LOAD_LUA_FILE("../lua_script/", "Data.lua");

    RunLuaFunc("Data.lua", "hello");

    return 0;
}
