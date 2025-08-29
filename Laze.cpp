#include "Laze.h"

Laze ::Laze()
{
    name = "laser";
}
void Laze::update()
{
	y += dy;
    if (y >= H) {
        life = false;
    }
}