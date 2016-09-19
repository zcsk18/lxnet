#ifndef __LUA_BINDER__
#define __LUA_BINDER__


#include "lua.hpp"

class LuaBinder{
public:
	~LuaBinder(){};
    LuaBinder(){this->m_pState=NULL;}
    LuaBinder(lua_State* L){this->m_pState=L;} 
	void SetState(lua_State* L){this->m_pState=L;}


	static LuaBinder* GetInstance()
	{
		static LuaBinder* m_pInstance = NULL;
		if(m_pInstance==NULL)
		{
			lua_State* L;
			/*initialize Lua*/
			L = lua_open();
			/*load Lua base libraries*/
			luaL_openlibs(L);
			
			m_pInstance = new LuaBinder(L);
		}
		return m_pInstance;
	}
	
	//init lua lib
	void init(const char* tname,const luaL_reg* flist);

	// push nil
	void pushnil();

	// push&get number
	void pushnumber(double v);
	double checknumber(int index);
	
	// push & get string
	void pushstring(const char *s);
	const char* checkstring(int index);
	
	// push & get bool
	void pushbool(const bool b);
	bool checkbool(int index);

	// push & get user data
	void pushusertype(void* udata,const char* tname);
	void* checkusertype(int index,const char* tname);
	
	int doFile(char* fname);
	int doString(char* str);
	
	int setRef();
	
	
	
private:
	lua_State* m_pState;
};

#endif

void LuaBinder::pushnil()
{
    lua_pushnil(m_pState);
}

void LuaBinder::pushnumber(double v)
{
    lua_pushnumber(m_pState, v); 
}
double LuaBinder::checknumber(int index)
{
    return luaL_checknumber(m_pState,index);
}

void LuaBinder::pushstring(const char *s) 
{
    lua_pushstring(m_pState, s); 
}
const char* LuaBinder::checkstring(int index)
{
    return luaL_checkstring(m_pState,index);
}

void LuaBinder::pushbool(const bool b)
{
    lua_pushboolean(m_pState,b);
}

bool LuaBinder::checkbool(int index)
{
    return luaL_checknumber(m_pState,index);
}


void LuaBinder::init(const char* tname,const luaL_reg* flist)
{
    lua_newtable(m_pState);
    lua_pushstring(m_pState,"v");
    lua_setfield(m_pState,-2,"__mode");
    lua_pushvalue(m_pState,-1);
    lua_setmetatable(m_pState,-2);
    luaL_register(m_pState,tname,flist);
    luaL_newmetatable(m_pState,tname);
    lua_pushvalue(m_pState,-2);
    lua_setfield(m_pState,-2,"__index");
    lua_pop(m_pState,1);
}
void LuaBinder::pushusertype(void* udata,const char* tname)
{
    lua_pushlightuserdata(m_pState,udata);
    lua_rawget(m_pState,LUA_ENVIRONINDEX);
    if(lua_isnil(m_pState,-1))
    {
        lua_newtable(m_pState);
        lua_pushlightuserdata(m_pState,udata);
        lua_setfield(m_pState,-2,"_pointer");
        luaL_getmetatable(m_pState,tname);
        lua_setmetatable(m_pState,-2);
        lua_pushlightuserdata(m_pState,udata);
        lua_pushvalue(m_pState,-2);
        lua_rawset(m_pState,LUA_ENVIRONINDEX);
    }
}
void* LuaBinder::checkusertype(int index,const char* tname)
{
    lua_getfield(m_pState,LUA_REGISTRYINDEX,tname);
    luaL_getmetatable(m_pState,tname);
    while(lua_istable(m_pState,-1))
    {
        if(lua_rawequal(m_pState,-1,-2))
        {
            lua_pop(m_pState,2);
            lua_getfield(m_pState,index,"_pointer");
            void* udata = lua_touserdata(m_pState,-1);
            return udata;
        }
        lua_getfield(m_pState,-1,"_base");
        lua_replace(m_pState,-2);
    }
    luaL_typerror(m_pState,index,tname);
    return NULL;
}

int LuaBinder::doFile(char* fname)
{
    return luaL_dofile(m_pState,fname);
}

int LuaBinder::doString(char* str)
{
    return luaL_dostring(m_pState,str);
}

int LuaBinder::setRef()
{
    return lua_ref(m_pState, 1);
}






