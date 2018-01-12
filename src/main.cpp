#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "../include/u-gine.h"


int main(int argc, char* argv[]) {

	Screen::Instance().Open(1280, 720, false);

	uint16 screenWidth = Screen::Instance().GetWidth();
	uint16 screenHeight = Screen::Instance().GetHeight();

	IsometricMap* map = ResourceManager::Instance().LoadIsometricMap("data/isometric.tmx", 5);
	IsometricScene* scene = new IsometricScene(map);

	Image* playerImage = ResourceManager::Instance().LoadImage("data/isoplayer.png", 8, 8);
	playerImage->SetHandle(playerImage->GetWidth() / 2, playerImage->GetHeight());

	IsometricSprite* playerSprite = scene->CreateSprite(playerImage);
	// If multiplied by 1.1 instead of 1.5, it gets a better look (not walking through bushes)
	playerSprite->SetPosition(map->GetTileWidth() * 1.5, map->GetTileHeight() * 1.5, 0);

	scene->GetCamera().FollowSprite(playerSprite);

	playerSprite->SetFPS(4);
	playerSprite->SetFrameRange(0, 0);

	double moveX = map->GetTileWidth();
	double moveY = map->GetTileHeight();

	double spriteX, spriteY;
	double newX, newY;
	int tileID;

	uint16 left = 0;
	uint16 up = 24;
	uint16 right = 40;
	uint16 down = 56;
	uint16 frames = 3;
	uint16 lastDirection = left;

	while (Screen::Instance().IsOpened() && !Screen::Instance().KeyPressed(GLFW_KEY_ESC)) {

		Renderer::Instance().Clear(0, 0, 0);

		spriteX = playerSprite->GetX();
		spriteY = playerSprite->GetY();

		newX = spriteX;
		newY = spriteY;

		// Movement
		if (Screen::Instance().KeyPressed(GLFW_KEY_LEFT) && !playerSprite->IsMoving()) {
			lastDirection = left;
			newX = playerSprite->GetX() - moveX;
			playerSprite->SetFrameRange(left, left + frames);
			playerSprite->SetCurrentFrame(left);
			playerSprite->MoveTo(newX, playerSprite->GetY(), moveX);
		}
		else if (Screen::Instance().KeyPressed(GLFW_KEY_UP) && !playerSprite->IsMoving()) {
			lastDirection = up;
			newY = playerSprite->GetY() - moveY;
			playerSprite->SetFrameRange(up, up + frames);
			playerSprite->SetCurrentFrame(up);
			playerSprite->MoveTo(playerSprite->GetX(), newY, moveY);
		}
		else if (Screen::Instance().KeyPressed(GLFW_KEY_RIGHT) && !playerSprite->IsMoving()) {
			lastDirection = right;
			newX = playerSprite->GetX() + moveX;
			playerSprite->SetFrameRange(right, right + frames);
			playerSprite->SetCurrentFrame(right);
			playerSprite->MoveTo(newX, playerSprite->GetY(), moveX);
		}
		else if (Screen::Instance().KeyPressed(GLFW_KEY_DOWN) && !playerSprite->IsMoving()) {
			lastDirection = down;
			newY = playerSprite->GetY() + moveY;
			playerSprite->SetFrameRange(down, down + frames);
			playerSprite->SetCurrentFrame(down);
			playerSprite->MoveTo(playerSprite->GetX(), newY, moveY);
		}
		else if (!playerSprite->IsMoving()) {
			playerSprite->SetFrameRange(lastDirection, lastDirection);
			playerSprite->SetCurrentFrame(lastDirection);
		}

		// Check if movement is correct
		if (newX != spriteX || newY != spriteY) {
			tileID = map->GetLayerId(newX / map->GetTileWidth(), newY / map->GetTileHeight());

			if (tileID > map->GetFirstColId())
				playerSprite->MoveTo(spriteX, spriteY, 0);
		}

		scene->Update(Screen::Instance().ElapsedTime());
		scene->Render();

		// Refrescamos la pantalla
		Screen::Instance().Refresh();
	}

	ResourceManager::Instance().FreeResources();

	return 0;
}