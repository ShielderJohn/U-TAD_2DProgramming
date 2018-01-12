#include "../include/map.h"
#include "../include/rectcollision.h"
#include "../include/collision.h"
#include "../include/image.h"
#include "../include/renderer.h"
#include "../include/resourcemanager.h"
#include "../include/string.h"
#include "../lib/rapidxml.hpp"
#include <limits.h>

using namespace rapidxml;


Map::Map(const String &filename, uint16 firstColId) {
	// TAREA: Implementar constructor
	this->filename = filename;
	this->firstColId = firstColId;
	valid = false;

	String fileContent;
	xml_document<> document;
	xml_node<>* map;
	xml_node<>* tileset;
	int firstGid, tilesetTileWidth, tilesetTileHeight;
	int tileOffsetX = 0, tileOffsetY = 0;

	xml_node<>* imageNode;
	int imageWidth, imageHeight;

	xml_node<>* data;
	xml_node<>* tile;

	// Read file
	fileContent = String::Read(filename);

	// Parse to xml document
	document.parse<0>(const_cast<char*>(fileContent.ToCString()));

	// Get map
	map = document.first_node("map");

	// Get map attributes
	width = String(map->first_attribute("width")->value()).ToInt();
	height = String(map->first_attribute("height")->value()).ToInt();
	tileWidth = String(map->first_attribute("tilewidth")->value()).ToInt();
	tileHeight = String(map->first_attribute("tileheight")->value()).ToInt();

	// Get tileset
	tileset = map->first_node("tileset");

	// Get tileset attributes
	firstGid = String(tileset->first_attribute("firstgid")->value()).ToInt();
	tilesetTileWidth = String(tileset->first_attribute("tilewidth")->value()).ToInt();
	tilesetTileHeight = String(tileset->first_attribute("tileheight")->value()).ToInt();

	// Check if tileset has tileoffset element
	if (tileset->first_node("tileoffset") != NULL) {
		tileOffsetX = String(tileset->first_node("tileoffset")->first_attribute("x")->value()).ToInt();
		tileOffsetY = String(tileset->first_node("tileoffset")->first_attribute("y")->value()).ToInt();
	}

	// Get image
	imageNode = tileset->first_node("image");

	// Get image attributes
	imageFile = String(imageNode->first_attribute("source")->value()).StripDir();
	imageWidth = String(imageNode->first_attribute("width")->value()).ToInt();
	imageHeight = String(imageNode->first_attribute("height")->value()).ToInt();

	// Get layer
	data = map->first_node("layer")->first_node("data");

	// Engine does not support encoding nor compression
	if (data->first_attribute("encoding") == NULL && data->first_attribute("compression") == NULL) {
		// Get tile
		tile = data->first_node("tile");

		while (tile != NULL) {
			tileIds.Add(String(tile->first_attribute("gid")->value()).ToInt() - firstGid);
			tile = tile->next_sibling("tile");
		}

		// Load tileset image
		String path = filename.ExtractDir() + "/" + imageFile;
		uint16 hframes = static_cast<uint16>(imageWidth / tilesetTileWidth);
		uint16 vframes = static_cast<uint16>(imageHeight / tilesetTileHeight);

		this->image = ResourceManager::Instance().LoadImage(path, hframes, vframes);
		this->image->SetHandle(tileOffsetX, tileOffsetY);

		valid = true;
	}
}

void Map::Render() const {
	for ( uint16 y = 0; y < GetRows(); y++ ) {
		for ( uint16 x = 0; x < GetColumns(); x++ ) {
			if ( GetTileId(x, y) >= 0 ) {
				Renderer::Instance().DrawImage(image, x*GetTileWidth(), y*GetTileHeight(), GetTileId(x, y));
			}
		}
	}
}

bool Map::CheckCollision(const Collision* collision) const {
	// Creamos caja de colisiones
	double boxX = 0, boxY = 0, boxWidth = GetTileWidth(), boxHeight = GetTileHeight();
	RectCollision boxCol(&boxX, &boxY, &boxWidth, &boxHeight);
	
	// Comprobamos colision con cada tile
	for ( uint16 y = 0; y < GetRows(); y++ ) {
		for ( uint16 x = 0; x < GetColumns(); x++ ) {
			if ( GetTileId(x, y) >= firstColId ) {
				boxX = x * GetTileWidth();
				boxY = y * GetTileHeight();

				if ( collision->DoesCollide(&boxCol) )
					return true;
			}
		}
	}

	return false;
}

double Map::GetGroundY(double x, double y) const {
	double groundY = UINT_MAX;

	if ( x < 0  ||  x >= width*tileWidth  ||  y >= height*tileHeight ) return groundY;
	if ( y < 0 ) y = 0;

	// Buscamos el primer tile en esa columna
	for ( int tiley = int(y/tileHeight); tiley < height; tiley++ ) {
		if ( tileIds[tiley*width + int(x/tileWidth)] >= 0 ) {
			groundY = tiley*tileHeight;
			break;
		}
	}
	return groundY;
}
