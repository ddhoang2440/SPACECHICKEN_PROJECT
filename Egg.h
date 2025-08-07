#pragma once
#include "Entity.h"
#include "Constants.h"

class Egg : public Entity
{
public:
    Egg();
    void update() override;
};