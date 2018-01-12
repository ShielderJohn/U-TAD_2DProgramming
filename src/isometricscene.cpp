#include "../include/isometricsprite.h"
#include "../include/isometricmap.h"
#include "../include/isometricscene.h"


IsometricScene::IsometricScene(IsometricMap* map, Image* imageBack, Image* imageFront) : MapScene(map, imageBack, imageFront) {
	map->GenerateLayerSprites(this);
}

IsometricSprite* IsometricScene::CreateSprite(Image* image, Layer layer) {
	IsometricSprite* sprite = new IsometricSprite(image);
	AddSprite(sprite, layer);

	return sprite;
}

void IsometricScene::Update(double elapsed) {
	MapScene::Update(elapsed);

	for (int i = 0; i < LAYER_COUNT; i++) {
		GetSprites(static_cast<Layer>(i)).Sort(CompareSprites);
	}
}

bool IsometricScene::CompareSprites(Sprite*& first, Sprite*& second) {
	bool returnValue = false;

	if (first->GetScreenY() < second->GetScreenY())
		returnValue = true;
	else if (first->GetScreenY() == second->GetScreenY())
		returnValue = first->GetScreenX() < second->GetScreenX();

	return returnValue;
}