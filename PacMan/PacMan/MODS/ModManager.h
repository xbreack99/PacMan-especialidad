#pragma once
#include "IMod.h"

#include <string>
#include <vector>
#include <memory>

#ifdef WIN32
#include <Windows.h>
using LibHandle = HMODULE;
#else
using LibHandle = void*;
#endif

using DestroyModFn = void(*)(IMod*);
using CreateModFn = IMod * (*)();

struct ModInfo
{
	LibHandle handle = nullptr;
	IMod* instance = nullptr;
	DestroyModFn destroy = nullptr;
	bool active = false;
};

class ModManager
{
public:

	ModManager();
	~ModManager();

	bool LoadMod(const std::string& path);

	void ToogleMod(std::size_t index, ModContext& ctx);

	void Update(float deltaTime, ModContext& ctx);

	const IMod* GetMod(std::size_t index) const;
	bool IsActive(std::size_t index) const;

private:

	std::vector<ModInfo> mMods;


	static LibHandle OpenLib(const std::string& path);
	static void* GetSym(LibHandle handle, const std::string& name);
	static void CloseLib(LibHandle handle);

};