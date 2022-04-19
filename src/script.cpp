#include "script.h"

using namespace luabridge;

void printMessage(const std::string& s) {
    std::cout << s << std::endl;
}

//class test_lua
//{
//public:
//    test_lua()
//    {
//        m_test_string = "c++ test string";
//    }
//    ~test_lua()
//    {
//    }
//  
//    void test(int a, int b)
//    {
//        printf("c++ test function %d+%d=%d\n", a, b, a + b);
//    }
//
//
//    void SetName(std::string name)
//    {
//        m_name = name;
//    }
//
//    std::string GetName() const
//    {
//        return m_name;
//    }
//    int cFunc(lua_State* L)
//    {
//
//        lua_pushstring(L, "str1");
//
//        lua_pushstring(L, "str2");
//
//        return 2;
//    }
//    std::string m_test_string;
//    std::string m_name;
//    static int m_static_data;
//};
//
//int test_lua::m_static_data;
//class test_lua_child :public test_lua
//{
//public:
//    test_lua_child(std::string test)
//        :m_test_child_string(test)
//    {
//        printf("call test_lua_child constructor\n");
//    }
//    ~test_lua_child()
//    {
//    }
//    std::string m_test_child_string;
//};

int lua() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    getGlobalNamespace(L).addFunction("printMessage", printMessage);
    luaL_dofile(L, "script.lua");
    lua_pcall(L, 0, 0, 0);
    LuaRef sumNumbers = getGlobal(L, "sumNumbers");
    int result = sumNumbers(5, 4);
    std::cout << "Result:" << result << std::endl;
    system("pause");
    return 0;
}