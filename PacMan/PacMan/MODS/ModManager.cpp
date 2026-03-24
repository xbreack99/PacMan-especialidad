#include "ModManager.h"
#include <iostream>

#ifdef WIN32
	#include <windows.h>
#else
	#include <dlfcn.h>
#endif

ModManager::ModManager()
{
}

ModManager::~ModManager()
{
	ModContext emptyCtx{};

	for (auto& mod : mMods)
	{
		if (mod.active && mod.instance)
		{
			mod.instance->OnDeactivate(emptyCtx);
		}

		if (mod.instance && mod.destroy)
		{
			mod.destroy(mod.instance);
		}

		CloseLib(mod.handle);
	}

	mMods.clear();
}

bool ModManager::LoadMod(const std::string& path)
{
	LibHandle handle = OpenLib(path);
	if (!handle)
	{
		// add logs
		return false;
	}

	auto create = reinterpret_cast<CreateModFn> (GetSym(handle, "CreateMod"));
	auto destroy = reinterpret_cast<DestroyModFn>(GetSym(handle, "DestroyMod"));

	if (!create || !destroy)
	{
		//add logs
		return false;
	}

	IMod* instance = create();
	if (!instance)
	{
		//add logs
		CloseLib(handle);
		return false;
	}

	mMods.push_back({ handle, instance, destroy, false });
	//success
	return true;
}

void ModManager::ToogleMod(std::size_t index, ModContext& ctx)
{
	if (index >= mMods.size()) return;
	ModInfo& mod = mMods[index];

	if (!mod.active)
	{
		mod.instance->OnActivate(ctx);
		mod.active = true;

		if (!mod.instance->IsToggle())
		{
			mod.instance->OnDeactivate(ctx);
			mod.active = false;
		}
	}
	else
	{
		mod.instance->OnDeactivate(ctx);
		mod.active = false;
	}
}

void ModManager::Update(float deltaTime, ModContext& ctx)
{
	for (auto& mod : mMods)
	{
		if (mod.active)
		{
			mod.instance->Update(deltaTime, ctx);
		}
	}
}

const IMod* ModManager::GetMod(std::size_t index) const
{
	if (index >= mMods.size())
	{
		return nullptr;
	}

	return mMods[index].instance;
}

bool ModManager::IsActive(std::size_t index) const
{
	if (index >= mMods.size())
	{
		return false;
	}

	return mMods[index].active;
}

LibHandle ModManager::OpenLib(const std::string& path)
{

#ifdef WIN32
	return LoadLibraryA(path.c_str());
#else
	return dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif

}

void* ModManager::GetSym(LibHandle handle, const std::string& name)
{

#ifdef WIN32
	return reinterpret_cast<void*>(GetProcAddress(handle, name.c_str()));
#else
	return dlsym(handle, name.c_str());
#endif

}

void ModManager::CloseLib(LibHandle handle)
{

	if (!handle) return;
#ifdef WIN32
	FreeLibrary(handle);
#else
	dlclose(handle);
#endif

}
