#include <iostream>
#include <string.h>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>
}


#define RegisLuaFunc(L, func)   \
    do                          \
    {                           \
    lua_pushcfunction(L, func); \
    lua_setglobal(L, #func);    \
    } while (false);


const char *lua_script = R"(
version = '1.0.0'
function add(a, b)
    foo(a)
    return a + b;
end
)";

extern "C"
{
    int foo(lua_State *l)
    {
        if (lua_gettop(l) != 1)
        {
            printf("args error\n");
        }
        
        //int value = luaL_checknumber(l, 1);
        int value = lua_tonumber(l, -1);
        printf("args is %d\n", value);

        printf("lua state\n");
        lua_pushnumber(l, value + 222);

        return 1;
    }
}

void LoadFile(lua_State *L)
{
    std::cout << "dofile " << luaL_dofile(L, "Data.lua") << std::endl;
    
    luaopen_base(L);
    lua_getglobal(L, "echo_hello");
    if (lua_pcall(L, 0, 0, 0) != 0)
    {
         std::cerr << "call func failed" << lua_tostring(L, -1) << std::endl;
    }

}


void TestFunc(lua_State *L)
{
    int result = 0;

    RegisLuaFunc(L, foo)

    std::cout << "dofile " << luaL_dofile(L, "Data.lua") << std::endl;

    if (luaL_dostring(L, lua_script) != 0)
    {
        std::cerr << "cannot run lua script" << std::endl;
        return ;
    }

    lua_getglobal(L, "version");
    const char *version = lua_tostring(L, -1);
    std::cout << "version: " << version << std::endl;
    lua_pop(L, 1);

    {
        lua_getglobal(L, "add_three");
        int a = 100;
        int b = 200;
        int c = 300;

        lua_pushnumber(L, a);
        lua_pushnumber(L, b);
        lua_pushnumber(L, c);

        if (lua_pcall(L, 3, 1, 0) != 0)
        {
            std::cerr << "call func failed" << lua_tostring(L, -1) << std::endl;
            lua_pop(L, -1);

            return ;
        }
        
        int sum = lua_tonumber(L, -1);

        lua_pop(L, -1);

        std::cout << a << " + " << b << " + " << c << " = " << sum << std::endl;
    }
    


    lua_getglobal(L, "add");
    int a = 100;
    int b = 200;
    lua_pushnumber(L, a);
    lua_pushnumber(L, b);

    if(lua_pcall(L, 2, 1, 0) != 0)
    {
        std::cerr << "call func failed" << lua_tostring(L, -1) << std::endl;
        lua_pop(L, -1);
        return ;
    }

    int sum = lua_tonumber(L, -1);

    lua_pop(L, -1);

    std::cout << a << " + " << b << " = " << sum << std::endl;

    lua_getglobal(L, "foo");
    lua_pushnumber(L, 1111);
    lua_pcall(L, 1, 1, 0);

    int value = lua_tonumber(L, -1);// 取出
    std::cout << "foo return " << value << std::endl;

    lua_pop(L, -1);

    luaopen_base(L);
    lua_getglobal(L, "echo_hello");
    if (lua_pcall(L, 0, 0, 0) != 0)
    {
         std::cerr << "call func failed" << lua_tostring(L, -1) << std::endl;
    }
    
}

int main()
{
    lua_State *L = luaL_newstate();
    if (L == NULL)
    {
        return 1;
    }

    //TestFunc(L);

    LoadFile(L);

    lua_close(L);

    return 0;
}



void callLuaFunc()
{

}