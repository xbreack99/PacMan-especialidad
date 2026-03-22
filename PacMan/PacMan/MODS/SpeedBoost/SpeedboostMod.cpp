#include "SpeedboostMod.h"
#include "../FrameworkCore/MovementComponent.h"


static constexpr float SPEED_MULTIPLY = 2.0f;

void SpeedBoostMod::OnActivate(ModContext& ctx)
{
	if (!ctx.pacmanMovement) return;
	mOriginalSpeed = ctx.pacmanMovement->speed;
	ctx.pacmanMovement->speed *= SPEED_MULTIPLY;
}

void SpeedBoostMod::OnDeactivate(ModContext& ctx)
{
	if (!ctx.pacmanMovement) return;
	ctx.pacmanMovement->speed = mOriginalSpeed;
}

extern "C"
{
	MYLIBRARY_API IMod* CreateMod()
	{
		return new SpeedBoostMod();
	}

	void DestroyMod(IMod* mod)
	{
		delete mod;
	}
}

