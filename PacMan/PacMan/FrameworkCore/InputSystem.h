#pragma once
#include "ISystem.h"

class InputSystem : public ISystem 
{
public:
	virtual ~InputSystem() override;
	virtual void Update(float deltaTime) override;
private:

};