#pragma once

#include <iostream>
#include <lua/lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <iostream>

using namespace luabridge;

class LuaTest
{
public:
    int a = 100000;
    LuaTest() {};
    void print(){
        std::cout << a << std::endl;
    }

    void set_a(const int _a){
        a = _a;
    }

    int get_a() const {
        return a;
    }

    lua_State* m_luastate;
    void InitLua()
    {

        m_luastate = luaL_newstate();
        luaL_openlibs(m_luastate);

        getGlobalNamespace(m_luastate).
            beginNamespace("game").
            beginClass<LuaTest>("LuaTest").
            addConstructor<void(*)(void)>().
            addProperty("a", &LuaTest::get_a, &LuaTest::set_a).
            addFunction("print", &LuaTest::print).
            endClass().
            endNamespace();
    }


    void Test()
    {
        luaL_openlibs(m_luastate);
        luaL_dofile(m_luastate, "assets/scripts/BlackJack.lua");
        lua_pcall(m_luastate, 0, 0, 0);
        //return 0;
        system("pause");
    }


    void Signal() {
        luaL_openlibs(m_luastate);
        luaL_dofile(m_luastate, "assets/scripts/Signal.lua");
        lua_pcall(m_luastate, 0, 0, 0);
        system("pause");
    }

};

int BlackJack();