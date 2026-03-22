#pragma once

#ifdef MYLIBRARY_EXPORTS
#define MYLIBRARY_API __declspec(dllexport)
#else
#define MYLIBRARY_API __declspec(dllimport)
#endif

class MovementComponent;
class GhostAISystem;

struct ModContext
{
	MovementComponent* pacmanMovement = nullptr;

	GhostAISystem* ghostAISystem = nullptr;

	int* lives = nullptr;

	int* score = nullptr;
};

class IMod
{
public:
	virtual void OnActivate(ModContext& ctx) = 0;

	virtual void OnDeactivate(ModContext& ctx) = 0;

	virtual void Update(float deltaTime, ModContext& ctx) = 0;

	virtual bool IsToggle() const = 0;
};