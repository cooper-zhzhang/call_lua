#include "LuaScript.h"
#include <stdexcept>
#include <system_error>

LuaScript::LuaScript(const std::string &path, const std::string &fileName)
{
    if (fileName.empty())
    {
        // 这里应该抛出异常

        throw  std::invalid_argument("file name is empty");
    }

    m_LuaState = luaL_newstate();
    if (m_LuaState == NULL)
    {
        throw std::runtime_error("new state fail");
    }

    std::string file = path + fileName;
    if (luaL_dofile(m_LuaState, file.c_str()) != LUA_OK)
    {
        lua_close(m_LuaState);
        throw  std::invalid_argument("do file lua fail");
    }

    luaopen_base(m_LuaState);

    m_LuaFile = fileName;
}

bool LuaScript::ExceLuaScript(std::string funName, const IVarList &varList, int resultCount, IVarList *pResultList)
{
    lua_getglobal(m_LuaState, funName.c_str());

    // 开始压栈
    for (size_t i = 0; i < varList.GetCount(); i++)
    {
        VAR_TYPE type = varList.GetType(i);
        switch ((int)type)
        {
        case (int)VAR_TYPE_INT:
            lua_pushnumber(m_LuaState, varList.IntValue(i));
            break;
        case (int)VAR_TYPE_INT64:
            //lua_pushint64(m_LuaState, varList.Int64Value(i));
            lua_pushnumber(m_LuaState, varList.Int64Value(i));
            break;
        case (int)VAR_TYPE_FLOAT:
            lua_pushnumber(m_LuaState, varList.FloatValue(i));
            break;
        case (int)VAR_TYPE_DOUBLE:
            lua_pushnumber(m_LuaState, varList.DoubleValue(i));
            break;
        case (int)VAR_TYPE_STRING:
            lua_pushstring(m_LuaState, varList.StrValue(i).c_str());
            break;
        default:
            break;
        }
    }

    bool luaRunResult = false;
    if (resultCount > 0)
    {
        luaRunResult = lua_pcall(m_LuaState, varList.GetCount(), resultCount, 0) == 0;
    }
    else
    {
        luaRunResult = lua_pcall(m_LuaState, varList.GetCount(), LUA_MULTRET, 0) == 0;
    }
    
    if (!luaRunResult)
    {
        lua_settop(m_LuaState, 0);

        return false;
    }
    
    /*
    if (lua_pcall(m_LuaState, 1, 0, 0) != 0)
    {
        lua_settop(m_LuaState, 0);
        std::cerr << "call func " << funName.c_str() << " failed " << lua_tostring(m_LuaState, -1) << std::endl;

        return false;
    }*/

    if (pResultList != NULL)
    {
        pResultList->Clear(); // TODO 实现这个接口

        int size = lua_gettop(m_LuaState);

        LuaGetVarList(m_LuaState, 1, size, *pResultList);// 编译器认为*pResultList是一个IVarList的实例 这不是扯淡吗
    }

    lua_settop(m_LuaState, 0);
    // 1 将参数压进去
    // 2 执行
    // 3 取出参数
    //m_LuaState

    return 0;
}

bool LuaScript::FuncExists(std::string funcName)
{ // TODO 这个需要进行进行检测函数是否存在

    if (funcName.empty())
    {
        return false;
    }

    lua_getglobal(m_LuaState, funcName.c_str());

    int result = lua_isfunction(m_LuaState, -1);

    lua_pop(m_LuaState, -1);

    return result;
}

bool LuaScript::RegisLuaFunc(std::string funcName, LuaFunc func)
{
    lua_pushcfunction(m_LuaState, func);
    lua_setglobal(m_LuaState, funcName.c_str());

    return true;
}

lua_State *LuaScript::GetLuaState()
{
    return m_LuaState;
}

bool LuaGetVarList(lua_State *state, int beg, int end, IVarList &msg)
{
    //((CVarList)(msg)).AddDouble(11);
    //IVarList *p = &msg;
    //msg.AddDouble(11);

    for (int i = beg; i <= end; ++i)
    {
        switch (lua_type(state, i))
        {
        case LUA_TNUMBER:
            msg.AddDouble(lua_tonumber(state, i));
            break;
        case LUA_TBOOLEAN:
            msg.AddInt(int(lua_tonumber(state, i)));
            break;
        case LUA_TSTRING:
            msg.AddStr(lua_tostring(state, i));
            break;
        case LUA_TUSERDATA:
        {// TODO 后续需要支持自定义数据
            int *p = (int *)lua_touserdata(state, i);
            (void)p;

            /*
				switch (*p)
				{
				case VTYPE_INT:
					msg.AddInt(*(int*)(p + 1));
					break;
				case VTYPE_INT64:
					msg.AddInt64(*(int64_t*)(p + 1));
					break;
				case VTYPE_FLOAT:
					msg.AddFloat(*(float*)(p + 1));
					break;
				case VTYPE_DOUBLE:
					msg.AddDouble(*(double*)(p + 1));
					break;
				case VTYPE_STRING:
					msg.AddString((const char*)(p + 1));
					break;
				case VTYPE_WIDESTR:
					msg.AddWideStr((const wchar_t*)(p + 1));
					break;
				case VTYPE_OBJECT:
					msg.AddObject(*(PERSISTID*)(p + 1));
					break;
				default:
					return false;
				}*/
        }
        break;
        default:
            return false;
        }
    }

    return true;
}