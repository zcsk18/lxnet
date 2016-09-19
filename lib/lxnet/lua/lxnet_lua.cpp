#include <lua.hpp>  
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "lxnet.h"
#include "msgbase.h"
#include "log.h"
#include "LuaBinder.h"

static LuaBinder binder;

static int net_init(lua_State *L)
{
    double big_buf_size = binder.checknumber(1);
    double big_buf_num = binder.checknumber(2);
    double small_buf_size = binder.checknumber(3);
    double small_buf_num = binder.checknumber(4);
    double listener_num = binder.checknumber(5);
    double socketer_num = binder.checknumber(6);
    double thread_num = binder.checknumber(7);
    bool ret = lxnet::net_init(big_buf_size,big_buf_num,small_buf_size,small_buf_num,listener_num,socketer_num,thread_num);
    binder.pushbool(ret);
    return 1;
}

static int net_release(lua_State *L)
{
    lxnet::net_release();
    return 0;
}

static int net_run(lua_State *L)
{
    lxnet::net_run();
    return 0;
}

static int GetHostIPByName(lua_State *L)
{
    static char buff[25];
    const char* host_name = binder.checkstring(1);
    lxnet::GetHostIPByName(host_name,buff,25);
    binder.pushstring(buff);
    return 1;
}



static const struct luaL_Reg lxnet_lib[] = {
    {"net_init",net_init},
    {"net_release",net_release},
    {"net_run",net_run},
    {"GetHostIPByName",GetHostIPByName},
    {NULL, NULL}
};
//------------------------------------------------------------------------
static int lis_Create(lua_State *L)
{
    lxnet::Listener *list = (lxnet::Listener*)lxnet::Listener::Create();
    binder.pushusertype(list,"Listener");
    return 1;
}

static int lis_Release(lua_State *L)
{
    lxnet::Listener *list = (lxnet::Listener*)binder.checkusertype(1,"Listener");
    lxnet::Listener::Release(list);
    return 0;
}

static int lis_Listen(lua_State *L)
{
    lxnet::Listener *list = (lxnet::Listener*)binder.checkusertype(1,"Listener");
    double port = binder.checknumber(2);
    double backlog = binder.checknumber(3);
    bool ret = list->Listen(port, backlog);
    binder.pushbool(ret);
    return 1;
}

static int lis_Close(lua_State *L)
{
    lxnet::Listener *list = (lxnet::Listener*)binder.checkusertype(1,"Listener");
    list->Close();
    return 0;
}

static int lis_IsClose(lua_State *L)
{
    lxnet::Listener *list = (lxnet::Listener*)binder.checkusertype(1,"Listener");
    bool ret = list->IsClose();
    binder.pushbool(ret);
    return 1;
}

static int lis_Accept(lua_State *L)
{
    lxnet::Listener *list = (lxnet::Listener*)binder.checkusertype(1,"Listener");
    lxnet::Socketer *clt = list->Accept();
    binder.pushusertype(clt,"Socketer");
    return 1;
}

static int lis_CanAccept(lua_State *L)
{
    lxnet::Listener *list = (lxnet::Listener*)binder.checkusertype(1,"Listener");
    bool ret = list->CanAccept();
    binder.pushbool(ret);
    return 1;
}


static const struct luaL_Reg lxnet_listener_lib[] = {
    {"Create",lis_Create},
    {"Release",lis_Release},
    {"Listen",lis_Listen},
    {"Close",lis_Close},
    {"IsClose",lis_IsClose},
    {"Accept",lis_Accept},
    {"CanAccept",lis_CanAccept},
    {NULL, NULL}
};

//------------------------------------------------------------------------------------
static int sock_Create(lua_State *L)
{
    lxnet::Socketer* sock = lxnet::Socketer::Create();
    binder.pushusertype(sock,"Socketer");
    return 1;
}

static int sock_Release(lua_State *L)
{
    lxnet::Socketer* sock = (lxnet::Socketer*)binder.checkusertype(1,"Socketer");
    lxnet::Socketer::Release(sock);
    return 0;
}

static int sock_Connect(lua_State *L)
{
    lxnet::Socketer* sock = (lxnet::Socketer*)binder.checkusertype(1,"Socketer");
    const char* ip = binder.checkstring(2);
    double port = binder.checknumber(3);
    bool ret = sock->Connect(ip,port);
    binder.pushbool(ret);
    return 1;
}

static int sock_Close(lua_State *L)
{
    lxnet::Socketer* sock = (lxnet::Socketer*)binder.checkusertype(1,"Socketer");
    sock->Close();
    return 0;
}

static int sock_IsClose(lua_State *L)
{
    lxnet::Socketer* sock = (lxnet::Socketer*)binder.checkusertype(1,"Socketer");
    bool ret = sock->IsClose();
    binder.pushbool(ret);
    return 1;
}

static int sock_GetIP(lua_State *L)
{
    lxnet::Socketer* sock = (lxnet::Socketer*)binder.checkusertype(1,"Socketer");
    static char ip[25];
    sock->GetIP(ip,25);
    binder.pushstring(ip);
    return 1;
}

static int sock_SendData(lua_State *L)
{
    lxnet::Socketer* sock = (lxnet::Socketer*)binder.checkusertype(1,"Socketer");
    const char* data = binder.checkstring(2);
    double len = binder.checknumber(3);
    bool ret = sock->SendData(data,len);
    binder.pushbool(ret);
    return 1;
}

static int sock_GetData(lua_State *L)
{
    lxnet::Socketer* sock = (lxnet::Socketer*)binder.checkusertype(1,"Socketer");
    static int len;
    static int data_len = 1024*1024*4;
    static char* data = new char[data_len];
    if(sock->GetRecvBufferByteSize()>0)
    {
        sock->GetData(data,data_len,&len);
        data[len] = 0;
        binder.pushstring(data);
        binder.pushnumber(len);
    }
    else
    {
        binder.pushstring("");
        binder.pushnumber(0);
    }
    return 2;
}

static int sock_CheckSend(lua_State *L)
{
    lxnet::Socketer* sock = (lxnet::Socketer*)binder.checkusertype(1,"Socketer");
    sock->CheckSend();
    return 0;
}

static int sock_CheckRecv(lua_State *L)
{
    lxnet::Socketer* sock = (lxnet::Socketer*)binder.checkusertype(1,"Socketer");
    sock->CheckRecv();
    return 0;
}


static const struct luaL_Reg lxnet_socketer_lib[] = {
    {"Create",sock_Create},
    {"Release",sock_Release},
    {"Connect",sock_Connect},
    {"Close",sock_Close},
    {"IsClose",sock_IsClose},
    {"GetIP",sock_GetIP},
    {"SendData",sock_SendData},
    {"GetData",sock_GetData},
    {"CheckSend",sock_CheckSend},
    {"CheckRecv",sock_CheckRecv},
    {NULL, NULL}
};




extern "C" LUA_API int luaopen_lxnet_lib(lua_State *L)
{  
    //luaL_newlib(L, test_lib); // 5.2  
    binder.SetState(L);
    binder.init("lxnet",lxnet_lib);
    binder.init("Listener",lxnet_listener_lib);
    binder.init("Socketer",lxnet_socketer_lib);
    //luaL_register(L, "lxnet",lxnet_lib); // lua 5.1  
    //luaL_register(L, "Listener",lxnet_listener_lib);
    return 1;  
}  


