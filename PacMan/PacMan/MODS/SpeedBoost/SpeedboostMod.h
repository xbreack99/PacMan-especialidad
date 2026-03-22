#pragma once
#include "IMod.h"

class SpeedBoostMod : public IMod
{
public:
	

	// Inherited via IMod
	void OnActivate(ModContext& ctx) override;

	void OnDeactivate(ModContext& ctx) override;

	void Update(float deltaTime, ModContext& ctx) override {};

	bool IsToggle() const override { return true;  };

private:
	
	float mOriginalSpeed = 0.f;
};

extern "C"
{
	MYLIBRARY_API IMod* CreateMod();
	MYLIBRARY_API void DestroyMod(IMod* mod);
}