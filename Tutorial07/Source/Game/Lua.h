#pragma once

#define LUA_COMPAT_MODULE
#include "../Lua/include/Lua.hpp"
#include "../Lua/include/LuaLib.h"
#include "../Lua/include/LauxLib.h"

#pragma comment(lib,"lua/liblua53.a")


/* �����Z���s���֐� */
int c_add(lua_State *L){
	// �����𓾂�
	int a1 = lua_tointeger(L, 1); // �� 1 �����𓾂�
	int a2 = lua_tointeger(L, 2); // �� 2 �����𓾂�
	// �v�Z
	float r = a1 + a2;
	printf("[%d + %d]\n", a1, a2);
	lua_pop(L, lua_gettop(L));
	// ���ʂ��X�^�b�N�ɖ߂�
	lua_pushnumber(L, r);
	return 1; // �߂�l�̐���Ԃ�
}

static const struct luaL_Reg func[] = {
		{ "c_add", c_add },
		{ NULL, NULL }
};


typedef int(*FUNC)(int *);

class Lua{
public:
	Lua(){
		L = luaL_newstate();
		luaL_openlibs(L); // Lua �̃��C�u�������g����悤�ɂ���

		// Lua �̃X�N���v�g��ǂݍ���
		if (luaL_loadfile(L, "lua/call_c_func.lua") || lua_pcall(L, 0, 0, 0)){
			printf("call_c_func.lua ���ǂ߂Ȃ�����\n");
			show_error(L);
			return;
		}
		luaL_register(L, "func", func);


		dllLoad();
	}
	~Lua(){
		lua_close(L); // ����
		FreeLibrary(hModule);
	}

	void dllClose(){
		FreeLibrary(hModule);
	}

	void Reload(){
		// Lua �̃X�N���v�g��ǂݍ���
		if (luaL_loadfile(L, "lua/call_c_func.lua") || lua_pcall(L, 0, 0, 0)){
			printf("call_c_func.lua ���ǂ߂Ȃ�����\n");
			show_error(L);
			return;
		}
		luaL_register(L, "func", func);


		dllLoad();

	}
	void LuaTest()
	{
		//lua_State *L = luaL_newstate();
		//lua_close(L);
		main();
	}




	/* �G���[��\�����邽�߂̊֐� */
	void show_error(lua_State *L){
		const char *err = lua_tostring(L, -1);
		printf("ERROR: %s\n", err);
	}

	/* ���C�� */
	int main(){


		lua_getglobal(L, "c_func");
		lua_pushinteger(L, 5);
		lua_pushinteger(L, 2);
		if (lua_pcall(L, 2, 1, 0) != 0) {
			lua_close(L);
			return 0;
		}
		int num = lua_gettop(L);
		if (num > 1){
			int type = lua_type(L, -1);
			float a = (float)lua_tonumber(L, -1);

			std::string out = "Lua :"+ std::to_string(a) + "\n";
			OutputDebugString(out.c_str());

			//LUA_TBOOLEAN
			
		}


		//int tmp=0;
		//int ret = (*lpFunc)(&tmp);

		//if (ret != 0)
		//{
		//	printf("%s", "�֐��̌Ăяo���Ɏ��s���܂����B");
		//	FreeLibrary(hModule);
		//	return 0;
		//}


		return 0;
	}
	void dllLoad(){
		// DLL�̃��[�h
		//hModule = LoadLibrary("../SCr/Release/ScriptComponent.dll");
		//if (hModule == NULL)
		//{
		//	return;
		//}
		// �֐��̃A�h���X�擾
		//lpFunc = (FUNC)GetProcAddress(hModule, "GetNum");
		//if (lpFunc == NULL)
		//{
		//	FreeLibrary(hModule);
		//	return;
		//}


	}
	FUNC lpFunc;
	HMODULE hModule;

	lua_State *L;
};
