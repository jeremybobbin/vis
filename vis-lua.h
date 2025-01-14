#ifndef VIS_LUA_H
#define VIS_LUA_H

#if CONFIG_LUA
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#else
typedef struct lua_State lua_State;
#endif

#if LUA_VERSION_NUM <= 504
#define lua_pushunsigned lua_pushinteger
#define lua_Unsigned lua_Integer
#define lua_tounsigned lua_tointeger
#define luaL_optunsigned luaL_optinteger
#define lua_pushunsigned lua_pushinteger
#define luaL_checkunsigned luaL_checkinteger
#endif

#if LUA_VERSION_NUM == 501
#define lua_setuservalue lua_setfenv
#define lua_getuservalue lua_getfenv
/*
diff luajit lua5.1:
	-LUA_OK
	-lua_traceback
	-lua_setfuncs
*/
#ifdef LUA_OK
#endif
#endif

#include "vis.h"

/* add a directory to consider when loading lua files */
bool vis_lua_path_add(Vis*, const char *path);
/* get semicolon separated list of paths to load lua files
 * (*lpath = package.path) and Lua C modules (*cpath = package.cpath)
 * both these pointers need to be free(3)-ed by the caller */
bool vis_lua_paths_get(Vis*, char **lpath, char **cpath);

/* various event handlers, triggered by the vis core */
void vis_lua_init(Vis*);
void vis_lua_start(Vis*);
void vis_lua_quit(Vis*);
#if !CONFIG_LUA
#define vis_lua_mode_insert_input vis_insert_key
#define vis_lua_mode_replace_input vis_replace_key
#else
void vis_lua_mode_insert_input(Vis*, const char *key, size_t len);
void vis_lua_mode_replace_input(Vis*, const char *key, size_t len);
#endif
void vis_lua_file_open(Vis*, File*);
bool vis_lua_file_save_pre(Vis*, File*, const char *path);
void vis_lua_file_save_post(Vis*, File*, const char *path);
void vis_lua_file_close(Vis*, File*);
void vis_lua_win_open(Vis*, Win*);
void vis_lua_win_close(Vis*, Win*);
void vis_lua_win_highlight(Vis*, Win*);
void vis_lua_win_status(Vis*, Win*);

#endif
