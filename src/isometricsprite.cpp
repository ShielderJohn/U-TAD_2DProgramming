#include <cmath>
#include "../include/math.h"
#include "../include/image.h"
#include "../include/isometricsprite.h"


IsometricSprite::IsometricSprite(Image* image) : Sprite(image) {
	screenX = 0;
	screenY = 0;
}

void IsometricSprite::SetCollision(CollisionMode mode) {
	if (mode == Sprite::COLLISION_PIXEL)
		Sprite::SetCollision(Sprite::COLLISION_RECT);
	else
		Sprite::SetCollision(mode);
}

void IsometricSprite::Update(double elapsed, const Map* map) {
	Sprite::Update(elapsed, map);
	TransformIsoCoords(GetX(), GetY(), GetZ(), &screenX, &screenY);
}

void IsometricSprite::UpdateCollisionBox() {
	double x = GetX() - GetImage()->GetHandleX() * fabs(GetScaleX());
	double y = GetY() - GetImage()->GetHandleY() * fabs(GetScaleX());
	double w = GetImage()->GetWidth() * fabs(GetScaleX());
	double h = GetImage()->GetHeight() * fabs(GetScaleX());

	Sprite::UpdateCollisionBox(x, y, w, h);
}