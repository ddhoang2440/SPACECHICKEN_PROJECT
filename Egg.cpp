#include "Egg.h"

Egg::Egg()
{
    name = "egg";
}

void Egg::update()
{
    dy = 2;// toc do roi co dinh
    y += dy;//vi tri hien tai roi
    if (y > H) { // chieu cao cua cua so 
        life = false;
    }
}