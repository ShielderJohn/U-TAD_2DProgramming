#ifndef UGINE_ISOMETRICSCENE_H
#define UGINE_ISOMETRICSCENE_H

#include "mapscene.h"
#include "isometricsprite.h"


class IsometricMap;

class IsometricScene : public MapScene {
public:
	IsometricScene(IsometricMap* map, Image* imageBack = 0, Image* imageFront = 0);
	virtual IsometricSprite* CreateSprite(Image* image, Layer layer = LAYER_BACK);
	virtual void Update(double elapsed);

protected:
	static bool CompareSprites(Sprite*& first, Sprite*& second);
};

#endif