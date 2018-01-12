#include "../include/image.h"
#include "../include/resourcemanager.h"
#include "../lib/rapidxml.hpp"
#include "../include/renderer.h"
#include "../include/isometricscene.h"
#include "../include/isometricsprite.h"
#include "../include/math.h"
#include "../include/isometricmap.h"

using namespace rapidxml;


IsometricMap::IsometricMap(const String& filename, uint16 firstColId) : Map(filename, firstColId) {
	String fileContent;
	xml_document<> document;
	xml_node<>* map;
	xml_node<>* tileset;
	int firstGid, mapWidth, mapHeight;

	xml_node<>* data;
	xml_node<>* tile;

	// Read file
	fileContent = String::Read(filename);

	// Parse to xml document
	document.parse<0>(const_cast<char*>(fileContent.ToCString()));

	// Get map
	map = document.first_node("map");
	mapWidth = String(map->first_attribute("width")->value()).ToInt();
	mapHeight = String(map->first_attribute("height")->value()).ToInt();

	// Get tileset
	tileset = map->first_node("tileset");

	// Get tileset attributes
	firstGid = String(tileset->first_attribute("firstgid")->value()).ToInt();

	// Get layer
	data = map->first_node("layer")->next_sibling("layer")->first_node("data");

	// Engine does not support encoding nor compression
	if (data->first_attribute("encoding") == NULL && data->first_attribute("compression") == NULL) {
		// Get tile
		tile = data->first_node("tile");

		while (tile != NULL) {
			topLayerIds.Add(String(tile->first_attribute("gid")->value()).ToInt() - firstGid);
			tile = tile->next_sibling("tile");
		}

		// Set image handle
		int handlex = GetImage()->GetHandleX() + mapWidth;
		int handley = GetImage()->GetHeight() - GetImage()->GetHandleY() - mapHeight;
		GetImage()->SetHandle(handlex, handley);
	}
}

void IsometricMap::GenerateLayerSprites(IsometricScene* scene) {
	for (int i = 0; i < topLayerIds.Size(); i++) {
		if (topLayerIds[i] != -1) {
			IsometricSprite* sprite = scene->CreateSprite(GetImage());
			sprite->SetCurrentFrame(topLayerIds[i]);

			if (topLayerIds[i] >= GetFirstColId())
				sprite->SetCollision(Sprite::COLLISION_RECT);

			sprite->SetPosition((i % GetColumns()) * GetTileWidth(), (i / GetColumns()) * GetTileHeight(), 0);
		}
	}
}

void IsometricMap::Render() const {
	double screenX = 0;
	double screenY = 0;

	for (uint16 y = 0; y < GetRows(); y++) {
		for (uint16 x = 0; x < GetColumns(); x++) {
			if (GetTileId(x, y) >= 0) {
				TransformIsoCoords(x, y, 0, &screenX, &screenY);
				Renderer::Instance().DrawImage(GetImage(), screenX * GetTileWidth(), screenY * GetTileHeight(), GetTileId(x, y));
			}
		}
	}
}

